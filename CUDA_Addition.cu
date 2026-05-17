#include <iostream>
#include <cuda_runtime.h>

using namespace std;

#define N 1000

// CUDA Kernel
__global__ void vectorAdd(int *A, int *B, int *C) {

    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if(i < N) {
        C[i] = A[i] + B[i];
    }
}

int main() {

    int A[N], B[N], C[N];

    // Initialize vectors
    for(int i = 0; i < N; i++) {
        A[i] = i;
        B[i] = i * 2;
    }

    cout << "Vector A 1st 20 elements: ";

    for(int i = 0; i < 20; i++) {
        cout << A[i] << " ";
    }
    cout<<endl;

    cout << "Vector B 1st 20 elements: ";

    for(int i = 0; i < 20; i++) {
        cout << B[i] << " ";
    }
    cout<<endl;

    int *d_A, *d_B, *d_C;

    // Allocate GPU memory
    cudaMalloc((void**)&d_A, N * sizeof(int));
    cudaMalloc((void**)&d_B, N * sizeof(int));
    cudaMalloc((void**)&d_C, N * sizeof(int));

    // Copy data from CPU to GPU
    cudaMemcpy(d_A, A, N * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, N * sizeof(int), cudaMemcpyHostToDevice);

    // Define block size and grid size
    int threadsPerBlock = 256;
    int blocksPerGrid = (N + threadsPerBlock - 1) / threadsPerBlock;

    // Launch kernel
    vectorAdd<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C);

    // Copy result back to CPU
    cudaMemcpy(C, d_C, N * sizeof(int), cudaMemcpyDeviceToHost);

    // Display first 10 results
    cout << "First 20 elements of Result Vector:" << endl;

    for(int i = 0; i < 20; i++) {
        cout << A[i] << " + " << B[i] << " = " << C[i] << endl;
    }

    // Free GPU memory
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}