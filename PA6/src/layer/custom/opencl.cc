#include <stdio.h>
#include <iostream>

#include "opencl.h"

#include "kernel.h"
#include "device.h"

#define CHECK_ERR(err, msg)                           \
    if (err != CL_SUCCESS)                            \
    {                                                 \
        if (err == CL_BUILD_PROGRAM_FAILURE)          \
        {                                             \
            size_t log_size;                          \
            clGetProgramBuildInfo(program, device_id, \
                                  CL_PROGRAM_BUILD_LOG, 0, nullptr, &log_size); \
            char *log = (char *)malloc(log_size);     \
            clGetProgramBuildInfo(program, device_id, \
                                  CL_PROGRAM_BUILD_LOG, log_size, log, nullptr); \
            fprintf(stderr, "%s failed: %d\n", msg, err); \
            fprintf(stderr, "Build log:\n%s\n", log); \
            free(log);                               \
        }                                             \
        else                                          \
        fprintf(stderr, "%s failed: %d\n", msg, err); \
        exit(EXIT_FAILURE);                           \
    }

void OpenCL::setup(cl_device_type device_type)
{
    // Load external OpenCL kernel code
    char *kernel_source = OclLoadKernel(KERNEL_PATH); // Load kernel source

    cl_int err;

    cl_device_id device_id; // device ID

    // Find platforms and devices
    OclPlatformProp *platforms = nullptr;
    cl_uint num_platforms;

    err = OclFindPlatforms((const OclPlatformProp **)&platforms, &num_platforms);
    CHECK_ERR(err, "OclFindPlatforms");

    int platform_index, device_index;

    // Get the device subject to the device_type.
    err = OclGetDeviceInfoWithFallback(&device_id, &platform_index, &device_index, device_type);
    CHECK_ERR(err, "OclGetDeviceWithFallback");

    // Get the platform and device properties.
    platform = &platforms[platform_index];
    device = &platform->devices[device_index];

    // Create a context
    context = clCreateContext(0, 1, &device_id, nullptr, nullptr, &err);
    CHECK_ERR(err, "clCreateContext");

    // Create a command queue
#ifdef __APPLE__
    queue = clCreateCommandQueue(context, device_id, 0, &err);
#else
    queue = clCreateCommandQueueWithProperties(context, device_id, 0, &err);
#endif
    CHECK_ERR(err, "clCreateCommandQueueWithProperties");

    // Create the program from the source buffer
    program = clCreateProgramWithSource(context, 1, (const char **)&kernel_source, nullptr, &err);
    CHECK_ERR(err, "clCreateProgramWithSource");

    // Build the program executable
    err = clBuildProgram(program, 0, nullptr, nullptr, nullptr, nullptr);
    CHECK_ERR(err, "clBuildProgram");

    // Create the compute kernel in the program we wish to run
    kernel = clCreateKernel(program, "conv_forward_kernel", &err);
    CHECK_ERR(err, "clCreateKernel");
}

void OpenCL::teardown()
{
    clReleaseProgram(this->program);
    clReleaseKernel(this->kernel);
    clReleaseCommandQueue(this->queue);
    clReleaseContext(this->context);
}