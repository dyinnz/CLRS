#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
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

pair<int, int> PartitionEqual(vector<int> &A, int l, int r) {
    int x = A[r];
    int i = l - 1, j = r + 1;
    int k = l;
    while (k < j) {
        if (A[k] < x) {
            swap(A[++i], A[k]);
            ++k;
        } else if (A[k] > x) {
            swap(A[--j], A[k]);
        } else {
            ++k;
        }
    }
    return {i, j};
}

pair<int, int> RandomizedParitionEqual(vector<int> &A, int l, int r) {
    int p = rand()%(r-l) + l; 
    swap(A[r], A[p]);
    return PartitionEqual(A, l, r);
}

void QuickSort(vector<int> &A, int l, int r) {
    if (l < r) {
        // size_t q = Partition(A, l, r);
        int q = RandomizedParition(A, l, r);
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

void QuickSortEqual(vector<int> &A, int l, int r) {
    if (l < r) {
        auto p = PartitionEqual(A, l, r);
        QuickSortEqual(A, l, p.first);
        QuickSortEqual(A, p.second, r);
    }
}

void TailRecursiveQuickSort(vector<int> &A, int l, int r) {
    while (l < r) {
        int q = Partition(A, l, r);
        if (q-l <= r-q) {
            TailRecursiveQuickSort(A, l, q-1);
            l = q + 1;
        } else {
            TailRecursiveQuickSort(A, q+1, r);
            r = q - 1;
        }
    }
}

void FuzzySortRecursion(vector<pair<int, int>> &A, int l, int r, int limit) {
    if (l < r) {
        int i = l-1;
        int j = r;
        int k = l;
        int maxr = limit;
        auto x = A[r];
        while (k < j) {
            if (A[k].second <= limit) {
                swap(A[++i], A[k]);
                ++k;
            } else if (A[k].first <= x.first) {
                if (A[k].second > maxr) {
                    maxr = A[k].second;
                }
                ++k;
            } else {
                swap(A[--j], A[k]);
            }
        }
        swap(A[j], A[r]);

        // Recursion
        FuzzySortRecursion(A, i+1, j-1, limit);
        FuzzySortRecursion(A, j+1, r, maxr);
    }
}

void FuzzySortIntervals(vector<pair<int, int>> &A) {
    FuzzySortRecursion(A, 0, A.size()-1, INT_MIN);
}

int main() {
    auto A = GeneratorAarray(100);
    //vector<int> A;
    srand(time(NULL));
    //generate_n(back_inserter(A), 100, [](){ return (rand()&0xf);} );
    //QuickSort(A, 0, A.size()-1);
    //QuickSortEqual(A, 0, A.size()-1);
    TailRecursiveQuickSort(A, 0, A.size()-1);
    for (int num : A) {
        cout << num << endl;
    }

    auto g = []() {
        int l = rand() % 100;
        int r = rand() % 200 + l;
        return make_pair(l, r);
    };
    vector<pair<int, int>> I;
    generate_n(back_inserter(I), 40, g);
    FuzzySortIntervals(I);
    for (auto p : I) {
        cout << p.first << '\t' << p.second << endl;
    }

    return 0;
}

