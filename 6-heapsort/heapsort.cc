#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

/* A hacking way to store heap-size is using the A[0] to store it,
 * and then get the children and parent position is easier to implement*/

#define left_child(x) (((x)<<1) + 1)
#define right_child(x) (((x)<<1) + 2)
#define parent(x) ((x-1)>>1)

vector<int> GeneratorAarray(int n) {
    srand(time(NULL)); 
    vector<int> ret(n);
    generate(ret.begin(), ret.end(), [](){ return (rand()&0xff)-0x7f;} );
    return move(ret);
}

bool IsMaxHeap(vector<int> &A, size_t heap_size) {
    for (size_t i = 0; i < heap_size; ++i) {
        if (left_child(i) < heap_size && A[left_child(i)] > A[i]) {
            return false;
        }
        if (right_child(i) < heap_size && A[right_child(i)] > A[i]) {
            return false;
        }
    }
    return true;
}

void MaxHeapify(vector<int> &A, size_t heap_size, size_t pos) {
    size_t left = left_child(pos);
    size_t right = right_child(pos);

    size_t maxpos {0};
    if (left < heap_size && A[left] > A[pos]) {
        maxpos = left;
    } else {
        maxpos = pos;
    }
    if (right < heap_size && A[right] > A[maxpos]) {
        maxpos = right;
    }
    if (maxpos != pos) {
        swap(A[pos], A[maxpos]);
        MaxHeapify(A, heap_size, maxpos);
    }
}

void BuildMapHeap(vector<int> &A) {
    for (int i = (A.size()-1)/2; i >= 0; --i) {
        MaxHeapify(A, A.size(), i);
    }
}

void HeapSort(vector<int> &A) {
    BuildMapHeap(A);
    for (size_t i = A.size()-1; i >= 1; --i) {
        swap(A[i], A[0]);
        MaxHeapify(A, i, 0);
    }
}

/******************************************************************************/

int HeapMaximum(vector<int> &A) {
    return A[0];
}

int HeapExtractMax(vector<int> &A) {
    assert(!A.empty());

    int maxv = A.front();
    A.front() = A.back();
    A.pop_back();
    MaxHeapify(A, A.size(), 0);
    return maxv;
}

void HeapIncreaseKey(vector<int> &A, size_t pos, int key) {
    while (pos > 0 && A[parent(pos)] < key) {
        A[pos] = A[parent(pos)];
        pos = parent(pos);
    }
    A[pos] = key;

}

void MaxHeapInsert(vector<int> &A, int key) {
    A.push_back(0x80000000);
    HeapIncreaseKey(A, A.size()-1, key);
}

void MaxHeapDelete(vector<int> &A, size_t i) {
    A[i] = A.back();
    A.pop_back();
    MaxHeapify(A, A.size(), i); 
}

/******************************************************************************/

typedef vector<vector<int>> Matrix;

bool IsYoungTableaus(Matrix &M) {
    for (size_t i = 0; i < M.size(); ++i) {
        for (size_t j = 1; j < M.front().size(); ++j) {
            if (M[i][j-1] > M[i][j-1]) return false;
        }
    }
    for (size_t j = 0; j < M.front().size(); ++j) {
        for (size_t i = 1; i < M.size(); ++i) {
            if (M[i-1][j] > M[i][j]) return false;
        }
    }
    return true;
}

void TableDeleteMin(Matrix &M, size_t x, size_t y) {
    size_t nx = x, ny = y;
    M[nx][ny] = 0x77777777;
    if ( x+1 < M.size() && M[x+1][y] < M[nx][ny]) {
        nx = x + 1;
    }
    if ( y+1 < M.front().size() && M[x][y+1] < M[nx][ny]) {
        nx = x;
        ny = y + 1;
    }

    if (nx != x || ny != y) {
        M[x][y] = M[nx][ny];
        TableDeleteMin(M, nx, ny);
    }
}

void TableInsert(Matrix &M, size_t x, size_t y, int key) {
}

/******************************************************************************/

int main() {
    /*
    auto A = GeneratorAarray(100);
    BuildMapHeap(A);
    cout << IsMaxHeap(A, A.size()) << endl;
    // HeapSort(A);
    
    while (!A.empty()) {
        cout << HeapExtractMax(A) << endl;
    }
    */
    vector<int> A;
    A.clear();
    cout << A.size() << endl;
    srand(time(NULL));
    auto f = [](){ return (rand()&0xff)-0x7f;} ;
    for (size_t i = 0; i < 100; ++i) {
        MaxHeapInsert(A, f());
    }
    for (int num : A) {
        cout << num << endl;
    }
    for (size_t i = 0; i < 50; i += 4) {
        MaxHeapDelete(A, i);
        cout << IsMaxHeap(A, A.size()) << endl;
    }
    cout << IsMaxHeap(A, A.size()) << endl;

    cout << endl;

    Matrix M {  { 2, 3, 4, 8, 12},
                { 4, 5, 7, 9, 14},
                { 8, 9, 10, 11, 15},
                { 10, 11, 14, 16, 17},
                { 12, 13, 15, 17, 18} };

    for (int i = 0; i < 25; ++i) {
        TableDeleteMin(M, 0, 0);
        cout << IsYoungTableaus(M) << endl; 
        for (auto vec : M) {
            for (auto num : vec) {
                cout << num << ' ';
            }
            cout << endl;
        }
    }
}



