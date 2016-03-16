#include <iostream>
#include <algorithm>
#include <vector>
#include <tuple>
#include <cstdlib>
#include <ctime>
using namespace std;

vector<int> GeneratorAarray(int n) {
  srand(time(NULL)); 
  vector<int> ret(n);
  generate(ret.begin(), ret.end(), [](){ return (rand()&0xff)-0x7f;} );
  return move(ret);
}

tuple<int, int, int> FindCrossingMax(const vector<int> &A, int left, int mid, int right) {
  int sum(0), left_sum(0x80000000), right_sum(0x80000000);
  int max_left(mid), max_right(mid);
  for (int i = mid; i >= left; --i) {
    sum += A[i];
    if (sum > left_sum) {
      left_sum = sum;
      max_left = i;
    }
  }
  sum = 0;
  for (int i = mid+1; i <= right; ++i) {
    sum += A[i];
    if (sum > right_sum) {
      right_sum = sum;
      max_right = i;
    }
  }
  return move(make_tuple(max_left, max_right, left_sum + right_sum));
}

tuple<int, int, int> FindMaxSubarrayRecur(const vector<int> &A, int left, int right) {
  if (left == right) return make_tuple(left, right, A[left]);
  int mid = (left + right) / 2;
  auto left_tuple = FindMaxSubarrayRecur(A, left, mid);
  auto right_tuple = FindMaxSubarrayRecur(A, mid+1, right);
  auto mid_tuple = FindCrossingMax(A, left, mid, right);
  if (get<2>(left_tuple) > get<2>(right_tuple) && get<2>(left_tuple) > get<2>(mid_tuple)) {
    return move(left_tuple);
  } else if (get<2>(right_tuple) > get<2>(left_tuple) && get<2>(right_tuple) > get<2>(mid_tuple)) {
    return move(right_tuple);
  } else {
    return move(mid_tuple);
  }
}

tuple<int, int, int> FindMaxSubarrayBrute(const vector<int> &A, int left_border, int right_border) {
  int left(0), right(0), sum(0), max_sum(0x80000000);
  for (int i = left_border; i <= right_border; ++i) {
    sum = 0;
    for (int j = i; j <= right_border; ++j) {
      sum += A[j];
      if (sum > max_sum) {
        max_sum = sum;
        left = i; right = j;
      }
    }
  }
  return move(make_tuple(left, right, max_sum));
}

tuple<int, int, int> FindMaxSubarrayMix(const vector<int> &A, int left, int right) {
  if (right - left < 32) {
    return FindMaxSubarrayBrute(A, left, right);
  }
  int mid = (left + right) / 2;
  auto left_tuple = FindMaxSubarrayMix(A, left, mid);
  auto right_tuple = FindMaxSubarrayMix(A, mid+1, right);
  auto mid_tuple = FindCrossingMax(A, left, mid, right);
  if (get<2>(left_tuple) > get<2>(right_tuple) && get<2>(left_tuple) > get<2>(mid_tuple)) {
    return move(left_tuple);
  } else if (get<2>(right_tuple) > get<2>(left_tuple) && get<2>(right_tuple) > get<2>(mid_tuple)) {
    return move(right_tuple);
  } else {
    return move(mid_tuple);
  }
}

tuple<int, int, int> FindMaxSubarrayLinear(const vector<int> &A) {
  int left(0), right(0), sum(0), max_sum(0x80000000);
  for (int i = 0; i < A.size(); ++i) {
    sum += A[i];
    if (sum < A[i]) sum = A[left = i];
    if (sum > max_sum) {
      max_sum = sum;
      right = i;
    }
  }
  return move(make_tuple(left, right, max_sum));
}

class Matrix {
  private:
    int _bi {0};
    int _bj {0};
    int _N {0};
    vector<vector<float>> *_pdata {nullptr};

  public:
    Matrix(vector<vector<float>> *pdata, int bi, int bj, int N) {
      set(pdata, bi, bj, N);
    }
    Matrix(Matrix &M, int pi, int pj, int N)
      : _bi(M._bi+pi), _bj(M._bj+pj), _N(N), _pdata(M._pdata) {}
    Matrix() = default;

    void set(vector<vector<float>> *pdata, int bi, int bj, int N) {
      _bi = bi; 
      _bj = bj;
      _N = N;
      _pdata = pdata;
      // expand
      if (N > pdata->size()) {
        _pdata->resize(N);
        for (auto &row : *_pdata) {
          row.resize(N);
        }
      }
    }

    float& operator() (int i, int j) {
      return (*_pdata)[_bi+i][_bj+j];
    }

    int N() {
      return _N;
    }

    const vector<vector<float>>& data() {
      return *_pdata;
    }
};

void MatrixPlus(Matrix &A, Matrix &B, Matrix &C) {
  int n = C.N();
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      C(i, j) = A(i, j) + B(i, j);
    }
  }
}

void MatrixMinus(Matrix &A, Matrix &B, Matrix &C) {
  int n = C.N();
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      C(i, j) = A(i, j) - B(i, j);
    }
  }
}

void SquareMatrixMultiply(Matrix &A, Matrix &B, Matrix &C) {
  int n = C.N();
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      C(i, j) = 0.0;
      for (int k = 0; k < n; ++k) {
        C(i, j) += A(i, j) * B(i, j);
      }
    }
  }
}

void StrassenMethod(Matrix &A, Matrix &B, Matrix &C) {
  int n = C.N();
  int half = n / 2;
  if (n < 2) {
    SquareMatrixMultiply(A, B, C);

  } else {
    vector<vector<float>> VS[10];
    Matrix S[10];
    for (int i = 0; i < 10; ++i) {
      S[i].set(&VS[i], 0, 0, half);
    }
    Matrix A11(A, 0,    0,    half);
    Matrix A12(A, 0,    half, half);
    Matrix A21(A, half, 0,    half);
    Matrix A22(A, half, half, half);
    Matrix B11(B, 0,    0,    half);
    Matrix B12(B, 0,    half, half);
    Matrix B21(B, half, 0,    half);
    Matrix B22(B, half, half, half);

    MatrixMinus(B12, B22, S[0]);
    MatrixPlus (A11, A12, S[1]);
    MatrixPlus (A21, A22, S[2]);
    MatrixMinus(B21, B11, S[3]);
    MatrixPlus (A11, A22, S[4]);
    MatrixPlus (B11, B22, S[5]);
    MatrixMinus(A12, A22, S[6]);
    MatrixPlus (B21, B22, S[7]);
    MatrixMinus(A11, A21, S[8]);
    MatrixPlus (B11, B12, S[9]);

    vector<vector<float>> VP[7];
    Matrix P[10];
    for (int i = 0; i < 10; ++i) {
      P[i].set(&VP[i], 0, 0, half);
    }
    StrassenMethod(A11,  S[0], P[0]);
    StrassenMethod(S[1], B22,  P[1]);
    StrassenMethod(S[2], B11,  P[2]);
    StrassenMethod(A22,  S[3], P[3]);
    StrassenMethod(S[4], S[5], P[4]);
    StrassenMethod(S[6], S[7], P[5]);
    StrassenMethod(S[8], S[9], P[6]);

    Matrix C11(C, 0,    0,    half);
    Matrix C12(C, 0,    half, half);
    Matrix C21(C, half, 0,    half);
    Matrix C22(C, half, half, half);

    MatrixPlus (P[4], P[3], C11);
    MatrixMinus(C11,  P[1], C11);
    MatrixPlus (C11,  P[5], C11);

    MatrixPlus (P[0], P[1], C12);

    MatrixPlus (P[2], P[3], C21);

    MatrixPlus (P[4], P[0], C22);
    MatrixMinus(C22,  P[2], C22);
    MatrixMinus(C22,  P[6], C22);

    for (int i = 0; i < half; ++i) {
      for (int j = 0; j < half; ++j) {
        cout << S[0](i, j) << " ";
      }
      cout << endl;
    }
  }
}

int main() {
  /*
  unsigned int a = 0;
  while (cin >> a) {
    vector<int> nums = GeneratorAarray(a);
    int left(0), right(0), sum(0);

    clock_t curr_time = clock();
    tie(left, right, sum) = FindMaxSubarrayRecur(nums, 0, nums.size()-1); 
    printf("cost time: %f\n", float(clock() - curr_time) / CLOCKS_PER_SEC);
    //cout << float(clock() - curr_time) / CLOCKS_PER_SEC << endl;
    cout << left << " " << right << " " << sum << endl;

    curr_time = clock();
    tie(left, right, sum) = FindMaxSubarrayMix(nums, 0, nums.size()-1); 
    printf("cost time: %f\n", float(clock() - curr_time) / CLOCKS_PER_SEC);
    cout << left << " " << right << " " << sum << endl;

    curr_time = clock();
    tie(left, right, sum) = FindMaxSubarrayLinear(nums);
    printf("cost time: %f\n", float(clock() - curr_time) / CLOCKS_PER_SEC);
    cout << left << " " << right << " " << sum << endl;
  }
  */
  vector<vector<float>> VA { {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1} };
  vector<vector<float>> VB { {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1} };
  // vector<vector<float>> VB { {2, 2, 2, 2}, {2, 2, 2, 2}, {2, 2, 2, 2}, {2, 2, 2, 2} };
  vector<vector<float>> VC;
  Matrix A(&VA, 0, 0, 4);
  Matrix B(&VB, 0, 0, 4);
  Matrix C(&VC, 0, 0, 4);
  // MatrixPlus(A, B, C);
  // SquareMatrixMultiply(A, B, C);
  StrassenMethod(A, B, C);
  for (auto &row : C.data()) {
    for (auto &f : row) {
      cout << f << " ";
    }
    cout << endl;
  }
  return 0;
}


