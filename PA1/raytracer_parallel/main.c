#include <stdio.h>
#ifdef __APPLE__ 
#include <OpenCL/cl.h>
#else 
#include <CL/cl.h>
#endif
#include <time.h>


#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "lib/stb_image_write.h"

const cl_int IMG_SIZE = 1024;
const size_t GLOBAL_SIZE = IMG_SIZE * IMG_SIZE;
const size_t LOCAL_SIZE = 32;
const float FOV = 3.14159265359/3.0;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "ERROR: Incorrect usage! Example usage: make gpu\n");
        return 1;
    }
    // Time measurement variables
    clock_t start, end;
    double cpu_time_used;

    // Start measuring host execution time
    start = clock();

    // OpenCL Initialization
    cl_platform_id platform[10];
    cl_device_id all_devices[2];
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;

    // Get platform and GPU device on platform
    cl_uint num_devices, num_platforms;
    cl_int err;

    // Run kernel on CPU or GPU depending on command line arg
    char* str_end;
    int device_choice = strtol(argv[1], &str_end, 10);
    if (*str_end != '\0') {
        fprintf(stderr, "Invalid device id. Expected an integer. Received %s.\n", argv[1]);
        return 1;
    }
    err = clGetPlatformIDs(2, platform, &num_platforms);
    err |= clGetDeviceIDs(platform[0], CL_DEVICE_TYPE_ALL, 2, all_devices, &num_devices);
    device = all_devices[device_choice];

    printf("\n========================================================\n");
    char device_name[256];
    clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(device_name), device_name, NULL); // Name of device
    printf("Device: %s\n", device_name);
    if (err != CL_SUCCESS) {
        fprintf(stderr, "Error getting platform and device\n");
        return 1;
    }

    // Create OpenCL context
    context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    if (err != CL_SUCCESS) {
        fprintf(stderr, "Error creating context\n");
        return 1;
    }

    // Create command queue
    queue = clCreateCommandQueue(context, device, 0, &err);
    if (err != CL_SUCCESS) {
        fprintf(stderr, "Error creating queue\n");
        return 1;
    }

    // Create memory buffer for output
    size_t pixel_size = IMG_SIZE * IMG_SIZE * 3 * sizeof(unsigned char);
    unsigned char pixels_h[IMG_SIZE * IMG_SIZE * 3];

    cl_mem pixels_d = clCreateBuffer(context, CL_MEM_WRITE_ONLY, pixel_size, NULL, &err);
    if (err != CL_SUCCESS) {
        fprintf(stderr, "Error creating pixels_d\n");
    }

    // Read kernel and instantiate it
    FILE* fp;
    char* kernelSrc;
    size_t kernelSize;

    fp = fopen("kernel.cl", "rb");

    if (!fp) {
        printf("Error reading kernel");
        exit(-1);
    }

    fseek(fp, 0, SEEK_END);
    kernelSize = ftell(fp);
    rewind(fp);

    // Allocate memory to read in source of kernel
    kernelSrc = (char*)malloc(kernelSize + 1);
    fread(kernelSrc, sizeof(char), kernelSize, fp);
    kernelSrc[kernelSize] = '\0';
    fclose(fp);

    // Create program now that we have kernel source
    program = clCreateProgramWithSource(context, 1, (const char**)&kernelSrc, NULL, &err);
    if (err != CL_SUCCESS) {
        fprintf(stderr, "Error creating program\n");
    }

    // Build program
    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);

    if (err != CL_SUCCESS) {
        char *buff_erro;
        cl_int errcode;
        size_t build_log_len;
        errcode = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &build_log_len);
        if (errcode) {
            printf("clGetProgramBuildInfo failed at line %d\n", __LINE__);
            exit(-1);
        }

        buff_erro = malloc(build_log_len);
        if (!buff_erro) {
            printf("malloc failed at line %d\n", __LINE__);
            exit(-2);
        }

        errcode = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, build_log_len, buff_erro, NULL);
        if (errcode) {
            printf("clGetProgramBuildInfo failed at line %d\n", __LINE__);
            exit(-3);
        }

        fprintf(stderr,"Build log: \n%s\n", buff_erro); //Be careful with  the fprint
        free(buff_erro);
        fprintf(stderr,"clBuildProgram failed\n");
        exit(EXIT_FAILURE);
    }

    // Build kernel
    kernel = clCreateKernel(program, "renderColor", &err);
    if (err != CL_SUCCESS) {
        fprintf(stderr, "Error creating kernel\n");
    }


    cl_float half_height = tan(FOV * 0.5);

    // Set kernel arguments
    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &pixels_d);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_int), &IMG_SIZE);
    err |= clSetKernelArg(kernel, 2, sizeof(cl_float), &half_height);
    if (err != CL_SUCCESS) {
        fprintf(stderr, "Error setting kernel arguments\n");
    }

    // Execute kernel on data
    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &GLOBAL_SIZE, &LOCAL_SIZE, 0, NULL, NULL);

    // Wait for kernel to finish
    clFinish(queue);

    // Read pixels results from device
    clEnqueueReadBuffer(queue, pixels_d, CL_TRUE, 0, pixel_size, pixels_h, 0, NULL, NULL);

    // Save the result to a PNG file
    char* out_img_name;
    if (device_choice == 1) {
        out_img_name = "output_gpu.png";
    } else {
        out_img_name = "output_cpu.png";
    }
    stbi_write_png(out_img_name, IMG_SIZE, IMG_SIZE, 3, pixels_h, IMG_SIZE * 3);

    // Release OpenCL resources
    clReleaseMemObject(pixels_d);

    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);


    // Free memory allocated to kernel source
    free(kernelSrc);

    // Stop measuring host execution time
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000; // Convert to milliseconds

    printf("Total execution time (host + kernel): %.3f ms\n", cpu_time_used);
    printf("Image titled %s has been created/modified and can now be viewed!\n", out_img_name);
    printf("========================================================\n\n");
}
