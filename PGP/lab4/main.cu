#include <cmath>
#include <iostream>

#include <thrust/extrema.h>
#include <thrust/device_vector.h>


#define HANDLE_ERROR(err)                             \
    do { if (err != cudaSuccess) { printf("ERROR: %s\n", cudaGetErrorString(err)); exit(0);} } while (0)


struct comparator
{
    __host__ __device__ bool operator()(double a, double b)
    {
        return std::fabs(a) < std::fabs(b);
    }
};


__global__ void swapRows(double *data, int n, int i, int i_max)
{
    int idx = blockDim.x * blockIdx.x + threadIdx.x;
    int offset = blockDim.x * gridDim.x;

    for (int j = idx; j < n; j += offset) {
        double tmp = data[j * n + i];
        data[j * n + i] = data[j * n + i_max];
        data[j * n + i_max] = tmp;
    }
}


__global__ void divide(double *data, int n, int i)
{
    int idx = blockDim.x * blockIdx.x + threadIdx.x;
    int offset = blockDim.x * gridDim.x;

    for (int j = idx + i + 1; j < n; j += offset) {
        data[i * n + j] /= data[i * n + i];
    }
}


__global__ void kernel(double *data, int n, int i)
{
    int idx = blockDim.x * blockIdx.x + threadIdx.x;
    int idy = blockDim.y * blockIdx.y + threadIdx.y;
    int offsetx = blockDim.x * gridDim.x;
    int offsety = blockDim.y * gridDim.y;

    for (int j = idx + i + 1; j < n; j += offsetx) {
        for (int k = idy + i + 1; k < n; k += offsety) {
            data[k * n + j] -= data[i * n + j] * data[k * n + i];
        }
    }
}


int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;
    double *mat = new double[n * n];
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cin >> mat[i + j * n];
        }
    }

    int *p = new int[n];
    for (int i = 0; i < n; ++i) {
        p[i] = i;
    }

    double *data;
    HANDLE_ERROR(cudaMalloc((void **) &data, sizeof(double) * n * n));
    HANDLE_ERROR(cudaMemcpy(data, mat, sizeof(double) * n * n, cudaMemcpyHostToDevice));

    comparator comp;
    thrust::device_ptr<double> i_ptr, i_max_ptr;
    for (int i = 0; i < n - 1; ++i) {
        int i_max = i;
        i_ptr = thrust::device_pointer_cast(data + i * n);
        i_max_ptr = thrust::max_element(i_ptr + i, i_ptr + n, comp);
        i_max = i_max_ptr - i_ptr;

        if (i_max != i) {
            p[i] = i_max;
            swapRows<<<256, 256>>>(data, n, i, i_max);
        }

        divide<<<256, 256>>>(data, n, i);
        kernel<<<dim3(32, 32), dim3(32, 32)>>>(data, n, i);
    }
    cudaMemcpy(mat, data, sizeof(double) * n * n, cudaMemcpyDeviceToHost);

    std::cout.precision(10);
    std::cout.setf(std::ios::scientific);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cout << mat[i + j * n] << " ";
        }
        std::cout << "\n";
    }

    for (int i = 0; i < n; ++i) {
        std::cout << p[i] << " ";
    }
    std::cout << "\n";

    HANDLE_ERROR(cudaFree(data));
    delete[] mat;
    delete[] p;
    return 0;
}