#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <ctime>
using namespace std;

vector<int> GenerateArray(int n) {
    srand(time(NULL)); 
    vector<int> ret(n);
    generate(ret.begin(), ret.end(), [](){ return rand() % 1000;} );
    return move(ret);
}

void InsertionSort(vector<int> &A, int left, int right) {
    for (int i = left+1; i < right; ++i) {
        int key = A[i];
        int j = i - 1;
        while (j > left-1 && A[j] > key) {
            A[j+1] = A[j];
            --j;
        }
        A[j+1] = key;
    }
}

void Merge(vector<int> &A, int left, int mid, int right) {
    //cout << left << " " << mid << " " << right << endl;
    int nl = mid - left, nr = right - mid;
    int L[nl+1], R[nr+1];
    for (int i = 0; i < nl; ++i) {
        L[i] = A[left + i];
    }
    for (int i = 0; i < nr; ++i) {
        R[i] = A[mid + i];
    }
    L[nl] = R[nr] = 0x7fffffff;
    for (int k = left, l = 0, r = 0; k < right; ++k) {
        if (L[l] < R[r]) {
            A[k] = L[l++];
        } else {
            A[k] = R[r++];
        }
    }
}

void MergeSort(vector<int> &A, int left, int right) {
    if (left < right-1) {
        int mid = (right + left) / 2;
        MergeSort(A, left, mid);
        MergeSort(A, mid, right);
        Merge(A, left, mid, right);
    }
}

void MergeSortMix(vector<int> &A, int left, int right) {
    if (right - left < 16) {
        InsertionSort(A, left, right);
    } else if (left < right-1) {
        int mid = (right + left) / 2;
        MergeSort(A, left, mid);
        MergeSort(A, mid, right);
        Merge(A, left, mid, right);
    }
}

int main() {
    int size(0);
    while (cin >> size) {
        auto nums = GenerateArray(size);

        auto copied = nums;

        for (auto num : copied) {
          printf("%d  ", num);
        }
        printf("\n");

        auto curr_time = clock();
        MergeSort(copied, 0, copied.size());
        printf("cost time: %f\n", float(clock() - curr_time) / CLOCKS_PER_SEC);

        for (auto num : copied) {
          printf("%d  ", num);
        }
        printf("\n");

        copied = nums; 
        curr_time = clock();
        InsertionSort(copied, 0, copied.size());
        printf("cost time: %f\n", float(clock() - curr_time) / CLOCKS_PER_SEC);

        for (auto num : copied) {
          printf("%d  ", num);
        }
        printf("\n");

        /*
        copied = nums;
        curr_time = clock();
        sort(copied.begin(), copied.end());
        printf("cost time: %f\n", float(clock() - curr_time) / CLOCKS_PER_SEC);

        copied = nums;
        curr_time = clock();
        stable_sort(copied.begin(), copied.end());
        printf("cost time: %f\n", float(clock() - curr_time) / CLOCKS_PER_SEC);
        */
    }
    return 0;
}

