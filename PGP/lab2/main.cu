#include <iostream>
#include <fstream>


#define HANDLE_ERROR(err)                             \
    do { if (err != cudaSuccess) { printf("ERROR: %s\n", cudaGetErrorString(err)); exit(0);} } while (0)


texture<uchar4, 2, cudaReadModeElementType> tex;


__global__ void kernel(uchar4 *dst, int w, int h, int wRatio, int hRatio)
{
    int idx = blockDim.x * blockIdx.x + threadIdx.x;
    int idy = blockDim.y * blockIdx.y + threadIdx.y;
    int offsetx = blockDim.x * gridDim.x;
    int offsety = blockDim.y * gridDim.y;
    int cntSamples = wRatio * hRatio;

    for (int x = idx; x < w; x += offsetx) {
        for (int y = idy; y < h; y += offsety) {
            int3 samples;
            samples.x = 0;
            samples.y = 0;
            samples.z = 0;
            for (int i = 0; i < wRatio; ++i) {
                for (int j = 0; j < hRatio; ++j) {
                    uchar4 p = tex2D(tex, x * wRatio + i, y * hRatio + j);
                    samples.x += p.x;
                    samples.y += p.y;
                    samples.z += p.z;
                }
            }
            samples.x /= cntSamples;
            samples.y /= cntSamples;
            samples.z /= cntSamples;
            dst[x + y * w] = make_uchar4(samples.x, samples.y, samples.z, 0);
        }
    }
}


int main()
{
    std::string input, output;
    int w, h, wNew, hNew;
    uchar4 *data;
    std::cin >> input >> output >> wNew >> hNew;

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

    int wRatio = w / wNew;
    int hRatio = h / hNew;

    cudaArray *arr;
    cudaChannelFormatDesc ch = cudaCreateChannelDesc<uchar4>();
    HANDLE_ERROR(cudaMallocArray(&arr, &ch, w, h));
    HANDLE_ERROR(cudaMemcpyToArray(arr, 0, 0, data, sizeof(uchar4) * h * w, cudaMemcpyHostToDevice));

    tex.addressMode[0] = cudaAddressModeClamp;
    tex.addressMode[1] = cudaAddressModeClamp;
    tex.channelDesc = ch;
    tex.filterMode = cudaFilterModePoint;
    tex.normalized = false;

    HANDLE_ERROR(cudaBindTextureToArray(tex, arr, ch));
    uchar4 *dev_data;
    HANDLE_ERROR(cudaMalloc(&dev_data, sizeof(uchar4) * hNew * wNew));
    kernel<<<dim3(16, 16), dim3(16, 16)>>>(dev_data, wNew, hNew, wRatio, hRatio);
    HANDLE_ERROR(cudaMemcpy(data, dev_data, sizeof(uchar4) * hNew * wNew, cudaMemcpyDeviceToHost));

    std::ofstream fsOut(output, std::ios::out | std::ios::binary);
    if (fsOut.is_open()) {
        fsOut.write((char *)&wNew, sizeof(wNew));
        fsOut.write((char *)&hNew, sizeof(hNew));
        fsOut.write((char *)data, wNew * hNew * sizeof(data[0]));
        fsOut.close();
    } else {
        return 1;
    }

    HANDLE_ERROR(cudaUnbindTexture(tex));
    HANDLE_ERROR(cudaFreeArray(arr));
    HANDLE_ERROR(cudaFree(dev_data));

    delete[] data;
    return 0;
}