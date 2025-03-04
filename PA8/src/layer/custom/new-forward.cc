#include <cmath>
#include <iostream>
#include <vector>

#include <clblast.h>

#include "kernel.h"
#include "device.h"

#include "opencl-new-forward.h"

#define CHECK_ERR(err, msg)                            \
    if (err != CL_SUCCESS)                             \
    {                                                  \
        fprintf(stderr, "%s failed: %d.\n", msg, err); \
        exit(EXIT_FAILURE);                            \
    }

void OpenCLInterface::conv_forward_gemm_opencl_prolog(const float *host_y, const float *host_x, const float *host_k, cl_mem *device_y, cl_mem *device_x, cl_mem *device_k, cl_mem *device_x_unroll, cl_mem *device_k_unroll, const int B, const int M, const int C, const int H, const int W, const int K)
{
    //@@ Allocate GPU memory here (don't forget batch sizes!)

    //@@ Copy memory to the GPU here
}

void OpenCLInterface::conv_forward_gemm_opencl(cl_mem device_y, const cl_mem device_x, const cl_mem device_k, const cl_mem device_x_unroll, const cl_mem device_k_unroll, const int B, const int M, const int C, const int H, const int W, const int K)
{
    //@@ ====== Start im2col =====

    // @@ define local and global work sizes

    //@@ Launch the im2col kernel here

    //@@ ====== End im2col =====

    //@@ ====== Start gemm =====

    // @@ Call clblast::GemmBatched here

    //@@ ====== End gemm =====
}

void OpenCLInterface::conv_forward_gemm_opencl_epilog(float *host_y, cl_mem device_y, cl_mem device_x, cl_mem device_k, cl_mem device_x_unroll, cl_mem device_k_unroll, const int B, const int M, const int C, const int H, const int W, const int K)
{
    //@@ Copy the output back to host

    //@@ Free the GPU memory here
}
