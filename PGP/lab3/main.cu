#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>


#define HANDLE_ERROR(err)                             \
    do { if (err != cudaSuccess) { printf("ERROR: %s\n", cudaGetErrorString(err)); exit(0);} } while (0)

__constant__ double AVG[32][3];
__constant__ double COV[32][3][3];
__constant__ double COV_INV[32][3][3];
__constant__ double DETS[32];


__device__ double func(uchar4 p, int i)
{
    double res = 0.0, p_avg[3], tmp[3];
    for (int j = 0; j < 3; ++j) {
        p_avg[j] = 0.0;
        tmp[j] = 0.0;
    }
    p_avg[0] = p.x - AVG[i][0];
    p_avg[1] = p.y - AVG[i][1];
    p_avg[2] = p.z - AVG[i][2];

    for (int j = 0; j < 3; ++j) {
        for (int k = 0; k < 3; ++k) {
            tmp[j] += -p_avg[k] * COV_INV[i][k][j];
        }
        res += tmp[j] * p_avg[j];
    }

    res -= std::log(std::abs(DETS[i]));
    return res;
}

__device__ int getClass(uchar4 pixel, int nc)
{
    double res[32];
    for (int i = 0; i < nc; ++i) {
        res[i] = func(pixel, i);
    }
    double maxEl = res[0];
    int idx = 0;
    for (int i = 0; i < nc; ++i) {
        if (res[i] > maxEl) {
            maxEl = res[i];
            idx = i;
        }
    }
    return idx;
}


__global__ void kernel(uchar4 *dst, int w, int h, int nc)
{
    int idx = blockDim.x * blockIdx.x + threadIdx.x;
    int idy = blockDim.y * blockIdx.y + threadIdx.y;
    int offsetx = blockDim.x * gridDim.x;
    int offsety = blockDim.y * gridDim.y;

    for (int x = idx; x < w; x += offsetx) {
        for (int y = idy; y < h; y += offsety) {
            dst[x + y * w].w = getClass(dst[x + y * w], nc);
        }
    }
}


int main()
{
    std::string input, output;
    int w, h, nc, np;
    uchar4 *data;
    std::cin >> input >> output >> nc;
    std::vector<std::vector<int2>> classes(nc);
    for (int i = 0; i < nc; ++i) {
        std::cin >> np;
        classes[i].resize(np);
        for (int j = 0; j < np; ++j) {
            std::cin >> classes[i][j].x >> classes[i][j].y;
        }
    }

    std::ifstream fsIn(input, std::ios::in | std::ios::binary);
    if (fsIn.is_open()) {
        fsIn.read((char *)&w, sizeof(w));
        fsIn.read((char *)&h, sizeof(h));
        data = new uchar4[w * h];
        fsIn.read((char *)data, w * h * sizeof(data[0]));
        fsIn.close();
    } else {
        return 1;
    }

    // дальше начинаются интересные вещи
    double avg[32][3];
    for (int i = 0; i < 32; ++i) {
        for (int j = 0; j < 3; ++j) {
            avg[i][j] = 0.0;
        }
    }

    for (int i = 0; i < nc; ++i) {
        int np = classes[i].size();
        for (int j = 0; j < np; ++j) {
            int x = classes[i][j].x;
            int y = classes[i][j].y;
            uchar4 curPixel = data[x + y * w];
            avg[i][0] += curPixel.x;
            avg[i][1] += curPixel.y;
            avg[i][2] += curPixel.z;
        }
        for (int k = 0; k < 3; ++k) {
            avg[i][k] /= np;
        }
    }

    double cov[32][3][3];
    for (int i = 0; i < 32; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                cov[i][j][k] = 0.0;
            }
        }
    }

    for (int i = 0; i < nc; ++i) {
        np =  classes[i].size();
        for (int j = 0; j < np; ++j) {
            double tmp[3];
            int x = classes[i][j].x;
            int y = classes[i][j].y;
            uchar4 curPixel = data[x + y * w];
            tmp[0] = curPixel.x - avg[i][0];
            tmp[1] = curPixel.y - avg[i][1];
            tmp[2] = curPixel.z - avg[i][2];

            for (int k = 0; k < 3; ++k) {
                for (int l = 0; l < 3; ++l) {
                    cov[i][k][l] += tmp[k] * tmp[l];
                }
            }
        }
        for (int k = 0; k < 3; ++k) {
            for (int l = 0; l < 3; ++l) {
                cov[i][k][l] /= np - 1;
            }
        }
    }

    double cov_inv[32][3][3];
    for (int i = 0; i < 32; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                cov_inv[i][j][k] = 0.0;
            }
        }
    }

    double dets[32];
    for (int i = 0; i < nc; ++i) {
        double det = 0;
        for (int j = 0; j < 3; ++j) {
            det += cov[i][0][j] * (cov[i][1][(j + 1) % 3] * cov[i][2][(j + 2) % 3] - cov[i][1][(j + 2) % 3] * cov[i][2][(j + 1) % 3]);
        }
        dets[i] = det;
    }

    // извините
    for (int i = 0; i < nc; ++i) {
        cov_inv[i][0][0] = (cov[i][1][1] * cov[i][2][2] - cov[i][2][1] * cov[i][1][2]) / dets[i];
        cov_inv[i][0][1] = (cov[i][0][2] * cov[i][2][1] - cov[i][0][1] * cov[i][2][2]) / dets[i];
        cov_inv[i][0][2] = (cov[i][0][1] * cov[i][1][2] - cov[i][0][2] * cov[i][1][1]) / dets[i];
        cov_inv[i][1][0] = (cov[i][1][2] * cov[i][2][0] - cov[i][1][0] * cov[i][2][2]) / dets[i];
        cov_inv[i][1][1] = (cov[i][0][0] * cov[i][2][2] - cov[i][0][2] * cov[i][2][0]) / dets[i];
        cov_inv[i][1][2] = (cov[i][1][0] * cov[i][0][2] - cov[i][0][0] * cov[i][1][2]) / dets[i];
        cov_inv[i][2][0] = (cov[i][1][0] * cov[i][2][1] - cov[i][2][0] * cov[i][1][1]) / dets[i];
        cov_inv[i][2][1] = (cov[i][2][0] * cov[i][0][1] - cov[i][0][0] * cov[i][2][1]) / dets[i];
        cov_inv[i][2][2] = (cov[i][0][0] * cov[i][1][1] - cov[i][1][0] * cov[i][0][1]) / dets[i];
    }

    HANDLE_ERROR(cudaMemcpyToSymbol(AVG, avg, sizeof(double) * 32 * 3));
    HANDLE_ERROR(cudaMemcpyToSymbol(COV, cov, sizeof(double) * 32 * 3 * 3));
    HANDLE_ERROR(cudaMemcpyToSymbol(COV_INV, cov_inv, sizeof(double) * 32 * 3 * 3));
    HANDLE_ERROR(cudaMemcpyToSymbol(DETS, dets, sizeof(double) * 32));

    uchar4 *dev_data;
    HANDLE_ERROR(cudaMalloc(&dev_data, sizeof(uchar4) * h * w));
    HANDLE_ERROR(cudaMemcpy(dev_data, data, sizeof(uchar4) * h * w, cudaMemcpyHostToDevice));
    kernel<<<dim3(16, 16), dim3(16, 16)>>>(dev_data, w, h, nc);
    HANDLE_ERROR(cudaMemcpy(data, dev_data, sizeof(uchar4) * h * w, cudaMemcpyDeviceToHost));

    std::ofstream fsOut(output, std::ios::out | std::ios::binary);
    if (fsOut.is_open()) {
        fsOut.write((char *)&w, sizeof(w));
        fsOut.write((char *)&h, sizeof(h));
        fsOut.write((char *)data, w * h * sizeof(data[0]));
        fsOut.close();
    } else {
        return 1;
    }

    HANDLE_ERROR(cudaFree(dev_data));

    delete[] data;
    return 0;
}