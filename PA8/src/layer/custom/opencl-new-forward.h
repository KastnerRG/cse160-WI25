#ifndef SRC_LAYER_OPENCL_NEW_FORWARD_H
#define SRC_LAYER_OPENCL_NEW_FORWARD_H

#include "device.h"
#include "opencl.h"

class OpenCLInterface
{
public:
    OpenCL *opencl;

    void conv_forward_gemm_opencl_prolog(const float *host_y, const float *host_x, const float *host_k, cl_mem *device_y, cl_mem *device_x, cl_mem *device_k, cl_mem *device_x_unroll, cl_mem *device_k_unroll, const int B, const int M, const int C, const int H, const int W, const int K);
    void conv_forward_gemm_opencl(cl_mem device_y, const cl_mem device_x, const cl_mem device_k, const cl_mem device_x_unroll, const cl_mem device_k_unroll, const int B, const int M, const int C, const int H, const int W, const int K);
    void conv_forward_gemm_opencl_epilog(float *host_y, cl_mem device_y, cl_mem device_x, cl_mem device_k, cl_mem device_x_unroll, cl_mem device_k_unroll, const int B, const int M, const int C, const int H, const int W, const int K);
};

#endif
