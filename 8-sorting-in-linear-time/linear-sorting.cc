#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<int> CountintSort(vector<int> &A, size_t k) {
    vector<int> B(A.size());
    vector<int> C(k, 0);

    for (int num : A) {
        ++C[num];
    }
    for (size_t i = 1; i < k; ++i) {
        C[i] += C[i-1];
    }
    for (auto it = A.rbegin(); it != A.rend(); ++it) {
        B[C[*it]--] = *it;
    }
    return move(B);
}

int main() {
    srand(time(NULL));
    vector<int> A(100);
    generate(A.begin(), A.end(), [](){ return (rand()&0xff); });
    auto B = CountintSort(A, 0xff);
    for (int num : B) {
        cout << num << endl;
    }
    return 0;
}
