#include <iostream>
#include <iomanip>


#define HANDLE_ERROR(err)                             \
    do { if (err != cudaSuccess) { printf("ERROR: %s\n", cudaGetErrorString(err)); exit(0);} } while (0)


__global__ void diffVector(double *v1, double *v2, double *res, int size)
{
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    int offset = gridDim.x * blockDim.x;
    while (idx < size) {
        res[idx] = v1[idx] - v2[idx];
        idx += offset;
    }
}


int main()
{
    std::ios_base::sync_with_stdio(false);

    int size = 0;
    std::cin >> size;

    double *vec1 = new double[size];
    double *vec2 = new double[size];
    double *res = new double[size];

    for (int i = 0; i < size; ++i) {
        std::cin >> vec1[i];
    }
    for (int i = 0; i < size; ++i) {
        std::cin >> vec2[i];
    }

    double *dev1, *dev2, *devRes;

    HANDLE_ERROR(cudaMalloc((void **) &dev1, sizeof(double) * size));
    HANDLE_ERROR(cudaMalloc((void **) &dev2, sizeof(double) * size));
    HANDLE_ERROR(cudaMalloc((void **) &devRes, sizeof(double) * size));

    HANDLE_ERROR(cudaMemcpy(dev1, vec1, sizeof(double) * size, cudaMemcpyHostToDevice));
    HANDLE_ERROR(cudaMemcpy(dev2, vec2, sizeof(double) * size, cudaMemcpyHostToDevice));

    diffVector<<<256, 256>>>(dev1, dev2, devRes, size);
    HANDLE_ERROR(cudaGetLastError());

    HANDLE_ERROR(cudaMemcpy(res, devRes, sizeof(double) * size, cudaMemcpyDeviceToHost));

    std::cout.precision(10);
    std::cout.setf(std::ios::scientific);
    for (int i = 0; i < size; ++i) {
        std::cout << res[i] << ' ';
    }
    std::cout << '\n';

    HANDLE_ERROR(cudaFree(dev1));
    HANDLE_ERROR(cudaFree(dev2));
    HANDLE_ERROR(cudaFree(devRes));

    delete[] vec1;
    delete[] vec2;
    delete[] res;
}
