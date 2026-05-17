#include <iostream>
#include <omp.h>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace std::chrono;

// Function to print first 10 elements
void printArray(int arr[], int n) {

    for(int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }

    cout << endl;
}

// Sequential Bubble Sort
void sequentialBubbleSort(int arr[], int n) {

    for(int i = 0; i < n - 1; i++) {

        for(int j = 0; j < n - i - 1; j++) {

            if(arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Parallel Bubble Sort using Odd-Even Method
void parallelBubbleSort(int arr[], int n) {

    for(int i = 0; i < n; i++) {

        // Even Phase
        #pragma omp parallel for
        for(int j = 0; j < n - 1; j += 2) {

            if(arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }

        // Odd Phase
        #pragma omp parallel for
        for(int j = 1; j < n - 1; j += 2) {

            if(arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

int main() {

    int n = 300;

    int arr1[n], arr2[n];

    // Generate Unique Numbers
    for(int i = 0; i < n; i++) {
        arr1[i] = i + 1;
    }

    // Shuffle Array
    random_shuffle(arr1, arr1 + n);

    // Copy Array
    for(int i = 0; i < n; i++) {
        arr2[i] = arr1[i];
    }

    cout << "First 10 elements before Sorting:\n";
    printArray(arr1, n);

    // Sequential Timing
    auto start1 = high_resolution_clock::now();

    sequentialBubbleSort(arr1, n);

    auto end1 = high_resolution_clock::now();

    // Parallel Timing
    auto start2 = high_resolution_clock::now();

    parallelBubbleSort(arr2, n);

    auto end2 = high_resolution_clock::now();

    // Calculate Time
    auto seq_time =
    duration_cast<milliseconds>(end1 - start1);

    auto par_time =
    duration_cast<milliseconds>(end2 - start2);

    cout << "\nAfter Sequential Bubble Sort:\n";
    printArray(arr1, n);

    cout << "Sequential Time : "
         << seq_time.count() << " milliseconds" << endl;

    cout << "\nAfter Parallel Bubble Sort:\n";
    printArray(arr2, n);

    cout << "Parallel Time   : "
         << par_time.count() << " milliseconds" << endl;

    return 0;
}
