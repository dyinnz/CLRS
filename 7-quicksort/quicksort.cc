#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<int> GeneratorAarray(int n) {
    srand(time(NULL)); 
    vector<int> ret(n);
    generate(ret.begin(), ret.end(), [](){ return (rand()&0xff)-0x7f;} );
    return move(ret);
}

int Partition(vector<int> &A, int l, int r) {
    int x = A[r];
    int i = l - 1;
    for (int j = l; j < r; ++j) {
        if (A[j] <= x) {
            swap(A[++i], A[j]);
        }
    }
    swap(A[++i], A[r]);
    return i;
}

int RandomizedParition(vector<int> &A, int l, int r) {
    int p = rand()%(r-l) + l;
    swap(A[r], A[p]);
    return Partition(A, l, r);
}

void QuickSort(vector<int> &A, int l, int r) {
    if (l < r) {
        size_t q = Partition(A, l, r);
        QuickSort(A, l, q-1);
        QuickSort(A, q+1, r);
    }
}

void QuickSortHoare(vector<int> &A, int l, int r) {
    if (l >= r) return;
    int x = A[r];
    int i = l - 1;
    int j = r + 1;
    while (true) {
        while (A[++i] < x) {}
        while (A[--j] > x) {}

        if (i < j) {
            swap(A[i], A[j]);
        } else {
            break;
        }
    }
    QuickSortHoare(A, l, j-1);
    QuickSortHoare(A, j, r);

    /* Consider the i == j, and let p = i. If choose A[r] as pilvot,
     * the recursion call should use p-1 and p. And if choose A[l] as
     * pilvot, the recursion should use p and p+1.
     */
}

int main() {
    // auto A = GeneratorAarray(100);
    // QuickSort(A, 0, A.size()-1);

    vector<int> A {0, 1};
    // Here are some bugs
    QuickSortHoare(A, 0, A.size()-1);
    for (int num : A) {
        cout << num << endl;
    }
    return 0;
}
