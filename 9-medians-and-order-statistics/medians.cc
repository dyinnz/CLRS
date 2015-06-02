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

int main() {
    vector<int> A(100);
    generate(A.begin(), A.end(), [](){ return rand()&0xfff; } );
    cout << RandomizedSelect(A, 0, A.size()-1, 50) << endl;

    sort(A.begin(), A.end());
    cout << A[50] << endl;
    
    for (size_t i = 0; i < A.size(); ++i) {
        cout << i << ' ' << A[i] << endl;
    }
    
    return 0;
}


