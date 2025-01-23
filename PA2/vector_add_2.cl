__kernel void vectorAdd(__global const int *a, __global const int *b,
                        __global int *result, const unsigned int size) {
        int i = get_global_id(0);

        if (i < size)
                result[i] = a[i] + b[i];
}
