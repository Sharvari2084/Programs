#include <iostream>
#include <omp.h>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;
using namespace std::chrono;

// Print first 15 elements
void printArray(int arr[], int n) {

    for(int i = 0; i < 15; i++) {
        cout << arr[i] << " ";
    }

    cout << endl;
}

// Merge Function
void merge(int arr[], int low, int mid, int high) {

    int temp[20000];

    int i = low;
    int j = mid + 1;
    int k = low;

    while(i <= mid && j <= high) {

        if(arr[i] < arr[j])
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }

    while(i <= mid)
        temp[k++] = arr[i++];

    while(j <= high)
        temp[k++] = arr[j++];

    for(i = low; i <= high; i++)
        arr[i] = temp[i];
}

// Sequential Merge Sort
void sequentialMergeSort(int arr[], int low, int high) {

    if(low < high) {

        int mid = (low + high) / 2;

        sequentialMergeSort(arr, low, mid);
        sequentialMergeSort(arr, mid + 1, high);

        merge(arr, low, mid, high);
    }
}

// Parallel Merge Sort
void parallelMergeSort(int arr[], int low, int high) {

    if(low < high) {

        int mid = (low + high) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr, low, mid);

            #pragma omp section
            parallelMergeSort(arr, mid + 1, high);
        }

        merge(arr, low, mid, high);
    }
}

int main() {

    int n = 20000;

    int arr1[20000], arr2[20000];

    // Generate Unique Numbers
    for(int i = 0; i < n; i++) {
        arr1[i] = i;
    }

    // Shuffle the array randomly
    srand(time(0));
    random_shuffle(arr1, arr1 + n);

    // Copy Array
    for(int i = 0; i < n; i++) {
        arr2[i] = arr1[i];
    }

    cout << "Before Sorting:\n";
    printArray(arr1, n);

    // Sequential Time
    auto start1 = high_resolution_clock::now();

    sequentialMergeSort(arr1, 0, n - 1);

    auto end1 = high_resolution_clock::now();

    // Parallel Time
    auto start2 = high_resolution_clock::now();

    parallelMergeSort(arr2, 0, n - 1);

    auto end2 = high_resolution_clock::now();

    auto seq_time =
    duration_cast<milliseconds>(end1 - start1);

    auto par_time =
    duration_cast<milliseconds>(end2 - start2);

    cout << "\nAfter Sequential Merge Sort:\n";
    printArray(arr1, n);

    cout << "Sequential Time : "
         << seq_time.count() << " ms" << endl;

    cout << "\nAfter Parallel Merge Sort:\n";
    printArray(arr2, n);

    cout << "Parallel Time   : "
         << par_time.count() << " ms" << endl;

    return 0;
}
