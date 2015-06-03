#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

size_t RandomizedParition(vector<int> &A, size_t l, size_t r) {
    size_t p = rand()%(r-l) + l;
    swap(A[r], A[p]);

    int x = A[r];
    size_t i = l;
    for (size_t j = l; j < r; ++j) {
        if (A[j] <= x) swap(A[i++], A[j]);
    }
    swap(A[i], A[r]);
    return i;
}

int RandomizedSelect(vector<int> &A, size_t l, size_t r, size_t index) {
    if (l == r) return A[l];
    size_t q = RandomizedParition(A, l, r);
    size_t k = q - l;
    if (index == k) {
        return A[q];
    } else if (index < k) {
        return RandomizedSelect(A, l, q-1, index);
    } else {
        return RandomizedSelect(A, q+1, r, index-k-1);
    }
}

int RandomizedSelectIterative(vector<int> &A, size_t l, size_t r, size_t index) {
    size_t q = 0, k = 0;
    while (l != r) {
        q = RandomizedParition(A, l, r);
        k = q - l;
        if (index == k) {
            return A[q];
        } else if (index < k) {
            r = q-1;
        } else {
            l = q+1;
            index = index - k - 1;
        }
    }
    return A[l];
}

template<class Iterator>
Iterator FindMedianInTwo(Iterator al, Iterator ar, Iterator bl, Iterator br, int index) {
    static int count = 0;
    cout << ++count << endl;

    if (ar == al) return bl + index;
    if (br == bl) return al + index;

    if (0 == index) return *al < *bl ? al : bl;

    Iterator ap = min(al + (index-1)/2, ar-1);
    Iterator bp = min(bl + index/2, br-1);

    if (*ap < *bp) {
        return FindMedianInTwo(ap+1, ar, bl, br, index - (ap-al+1));
    } else {
        return FindMedianInTwo(al, ar, bp+1, br, index - (bp-bl+1));
    }
}

int main() {
    srand(time(NULL));
    vector<int> A(10000);
    generate(A.begin(), A.end(), [](){ return rand()&0xfff; } );
    vector<int> B(10000);
    generate(B.begin(), B.end(), [](){ return rand()&0xfff; } );
    // cout << RandomizedSelectIterative(A, 0, A.size()-1, 50) << endl;
    // cout << RandomizedSelect(A, 0, A.size()-1, 50) << endl;

    sort(A.begin(), A.end());
    sort(B.begin(), B.end());

    auto it = FindMedianInTwo(A.begin(), A.end(), B.begin(), B.end(), 10000); 
    int num = *it;
    //cout << *it << endl;

    A.insert(A.end(), B.begin(), B.end());
    sort(A.begin(), A.end());

    for (size_t i = 0; i < A.size(); ++i) {
        //cout << i << ' ' << A[i] << endl;
    }
    cout << A[10000] << endl;
    cout << num << endl;
    
    return 0;
}


