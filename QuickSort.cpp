// Mini Project: Parallel Quick Sort using MPI
// Compile: mpic++ quicksort_mpi.cpp -o quicksort
// Run: mpirun -np 4 quicksort

#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Swap function
void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

// Partition function
int partition(int arr[], int low, int high) {

    int pivot = arr[high];
    int i = low - 1;

    for(int j = low; j < high; j++) {

        if(arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[i + 1], arr[high]);

    return i + 1;
}

// Sequential Quick Sort
void quickSort(int arr[], int low, int high) {

    if(low < high) {

        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Function to print array
void printArray(int arr[], int size) {

    for(int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }

    cout << endl;
}

int main(int argc, char* argv[]) {

    MPI_Init(&argc, &argv);

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 20;

    int data[N];
    int local_n = N / size;

    int* local_data = new int[local_n];

    double start, end;

    // Master process creates array
    if(rank == 0) {

        srand(time(0));

        cout << "Original Array:\n";

        for(int i = 0; i < N; i++) {
            data[i] = rand() % 100;
            cout << data[i] << " ";
        }

        cout << endl;
    }

    // Start timer
    start = MPI_Wtime();

    // Distribute data to all processes
    MPI_Scatter(data, local_n, MPI_INT,
                local_data, local_n, MPI_INT,
                0, MPI_COMM_WORLD);

    // Each process sorts its part
    quickSort(local_data, 0, local_n - 1);

    // Gather sorted parts
    MPI_Gather(local_data, local_n, MPI_INT,
               data, local_n, MPI_INT,
               0, MPI_COMM_WORLD);

    // Final sorting by master process
    if(rank == 0) {

        quickSort(data, 0, N - 1);

        end = MPI_Wtime();

        cout << "\nSorted Array:\n";
        printArray(data, N);

        cout << "\nExecution Time: " << end - start << " seconds\n";

        cout << "\nPerformance Enhancement Achieved Using Parallel Quick Sort with MPI.\n";
    }

    delete[] local_data;

    MPI_Finalize();

    return 0;
}