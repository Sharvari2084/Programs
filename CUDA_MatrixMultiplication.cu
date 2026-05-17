#include <iostream>
#include <cuda_runtime.h>

using namespace std;

#define N 4

// CUDA Kernel
__global__ void matrixMul(int *A, int *B, int *C) {

    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if(row < N && col < N) {

        int sum = 0;

        for(int k = 0; k < N; k++) {
            sum += A[row * N + k] * B[k * N + col];
        }

        C[row * N + col] = sum;
    }
}

int main() {

    int A[N][N], B[N][N], C[N][N];

    // Initialize matrices
    for(int i = 0; i < N; i++) {

        for(int j = 0; j < N; j++) {

            A[i][j] = i + j;
            B[i][j] = i + j*2;
        }
    }

    // Display Matrix A
    cout << "Matrix A:\n";

    for(int i = 0; i < N; i++) {

        for(int j = 0; j < N; j++) {
            cout << A[i][j] << " ";
        }

        cout << endl;
    }

    // Display Matrix B
    cout << "\nMatrix B:\n";

    for(int i = 0; i < N; i++) {

        for(int j = 0; j < N; j++) {
            cout << B[i][j] << " ";
        }

        cout << endl;
    }

    int *d_A, *d_B, *d_C;

    // Allocate GPU memory
    cudaMalloc((void**)&d_A, N * N * sizeof(int));
    cudaMalloc((void**)&d_B, N * N * sizeof(int));
    cudaMalloc((void**)&d_C, N * N * sizeof(int));

    // Copy CPU to GPU
    cudaMemcpy(d_A, A, N * N * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, N * N * sizeof(int), cudaMemcpyHostToDevice);

    // Thread block size
    dim3 threadsPerBlock(2, 2);

    // Grid size
    dim3 blocksPerGrid((N + 1) / 2, (N + 1) / 2);

    // Kernel launch
    matrixMul<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C);

    // Copy result back
    cudaMemcpy(C, d_C, N * N * sizeof(int), cudaMemcpyDeviceToHost);

    // Display Result Matrix
    cout << "\nResult Matrix:\n";

    for(int i = 0; i < N; i++) {

        for(int j = 0; j < N; j++) {
            cout << C[i][j] << " ";
        }

        cout << endl;
    }

    // Free memory
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}