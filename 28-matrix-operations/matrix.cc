/*******************************************************************************
 * Author: Dyinnz.HUST.UniqueStudio
 * Email:  ml_143@sina.com
 * Github: https://github.com/dyinnz
 * Date:   2015-12-02
 ******************************************************************************/

#include <cassert>
#include <array>
#include <vector>
#include <iostream>
#include <tuple>
#include <algorithm>

using namespace std;

template<int M, int N>
class Matrix {
  public:
    typedef array<float, N> Vec;
    typedef array<array<float, N>, M> Array2D;

    Matrix() {
      for (int i = 0; i < row_length(); ++i) {
        for (int j = 0; j < column_length(); ++j) {
          _matrix[i][j] = 0.0f;
        }
      }
    }

    Matrix(const Array2D &a) {
        _matrix = a;
    }

    Matrix(const vector<vector<float>> &v) {
      for (int i = 0; i < row_length(); ++i) {
        for (int j = 0; j < column_length(); ++j) {
          _matrix[i][j] = v[i][j];
        }
      }
    }

    Matrix(const Matrix &) = default;
    Matrix& operator=(const Matrix &) = default;

    Matrix(Matrix &&m) {
      _matrix = move(m._matrix);
    }

    Matrix& operator=(Matrix &&m) {
      if (this != &m) {
        _matrix = move(m._matrix);
      }
      return *this;
    }

    int row_length() const { return M; }
    int column_length() const { return N; }

    const Vec& vec(int m) const { return _matrix[m]; }
    Vec& vec(int m) { return _matrix[m]; }

    const Vec& operator[] (int m) const { return _matrix[m]; }
    Vec& operator[] (int m) { return _matrix[m]; }


    void set_vec(int m, const Vec& vec) {
      _matrix[m] = vec;
    }

    int at(int m, int n) const { return _matrix[m][n]; }
    int &at(int m, int n) { return _matrix[m][n]; }

  private:
    array<Vec, M> _matrix;
};

template <int M, int N> 
void DisplayMatrix(const Matrix<M, N> &m) {
  for (int i = 0; i < m.row_length(); ++i) {
    for (int j = 0; j < m.column_length(); ++j) {
      cout << m[i][j] << '\t';
    }
    cout << endl;
  }
}

template <class T>
void DisplayArray(const T &a) {
  for (auto f : a) {
    cout << f << '\t';
  }
  cout << endl;
}

template <int M>
array<float, M> LUPSolve(const Matrix<M, M> &L, 
                         const Matrix<M, M> &U, 
                         const array<int, M> &P,
                         const array<float, M> &b) {
  array<float, M> x;
  array<float, M> y;
  for (int i = 0; i < M; ++i) {
    y[i] = b[P[i]];
    for (int j = 0; j < i; ++j) {
      y[i] -= L[i][j] * y[j];
    }
  }

  for (int i = M-1; i >= 0; --i) {
    x[i] = 0;
    for (int j = i+1; j < M; ++j) {
      x[i] += U[i][j] * x[j];
    }
    x[i] = (y[i] - x[i]) / U[i][i];
  }

  return x;
}

template <int M>
tuple<Matrix<M, M>, Matrix<M, M>> LUDecomposition(Matrix<M, M> &A) {
  Matrix<M, M> L;
  Matrix<M, M> U;

  for (int i = 0; i < M; ++i) {
    U[i][i] = 1.0f;
  }

  for (int k = 0; k < M; ++k) {
    U[k][k] = A[k][k];
    for (int i = k+1; i < M; ++i) {
      L[i][k] = A[i][k] / U[k][k]; U[k][i] = A[k][i]; 
    } 
    for (int i = k+1; i < M; ++i) { 
      for (int j = k+1; j < M; ++j) { 
        A[i][j] -= L[i][k] * U[k][j];
      }
    } 
  }
  return make_tuple(L, U);
}

template <int M> array<int, M> LUPDecomposition(Matrix<M, M> &A) {
  array<int, M> P;
  for (int i = 0; i < M; ++i) {
    P[i] = i;
  }
  for (int k = 0; k < M; ++k) {
    float minf = 0.0f;
    int min_index = -1;
    for (int i = k; i < M; ++i) {
      if(fabs(A[i][k]) > minf) {
        minf = fabs(A[i][k]);
        min_index = i;
      }
    }
    assert(min_index != -1); 
    swap(P[k], P[min_index]);
    // optimize
    for (int i = 0; i < M; ++i) {
      swap(P[k], P[min_index]);
    }
    for (int i = k+1; i < M; ++i) {
      A[i][k] /= A[k][k];
      for (int j = k+1; i < M; ++i) {
        A[i][j] -= A[i][k] * A[k][j];
      }
    }
  }
  return P;
}

typedef vector<vector<float>> Array2D;

void TestLUPComposition() {

  cout << __func__ << endl;
  Array2D LUPAry { 
    {2, 0, 2, 0.6},
    {3, 3, 4, -2},
    {5, 5, 4, 2},
    {-1, -2, 3.4, -1},
  };

  Matrix<4, 4> A(LUPAry);
  DisplayMatrix(A);
  auto P = LUPDecomposition(A);
  cout << "P: " << endl;
  DisplayArray(P);
}

int main() {
  Matrix<10, 10> matrix;
  DisplayMatrix(matrix);

  // typedef array<array<float, 3>, 3> Array2D;

  Array2D ary_L {
    {1, 0, 0},
    {0.2, 1, 0},
    {0.6, 0.5, 1},
  };
 
  Matrix<3, 3> L(ary_L);

  Array2D ary_U { 
    {5, 6, 3},
    {0, 0.8, -0.6},
    {0, 0, 2.5},
  };

  Matrix<3, 3> U(ary_U);

  array<float, 3> b { {3, 7, 8} };
  array<int, 3> P { {2, 0, 1} };

  auto x = LUPSolve(L, U, P, b);
  DisplayArray(x);

  Array2D ary_A {
    {2, 3, 1, 5},
    {6, 13, 5, 19},
    {2, 19, 10, 23},
    {4, 10, 11, 31},
  };

  Matrix<4, 4> A(ary_A);
  Matrix<4, 4> get_L, get_U;

  DisplayMatrix(A);

  tie(get_L, get_U) = LUDecomposition(A);
  DisplayMatrix(get_L);
  DisplayMatrix(get_U);
  

  TestLUPComposition();

  return 0;
}

