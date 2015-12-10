/*******************************************************************************
 * Author: Dyinnz.HUST.UniqueStudio
 * Email:  ml_143@sina.com
 * Github: https://github.com/dyinnz
 * Date:   2015-12-09
 ******************************************************************************/
#include <iostream>
#include <array>
#include <vector>

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

template<int Bsize, int Nsize> class SlackFormBuilder;

template<int Bsize, int Nsize> 
class SlackForm {
  private:
    array<float, Bsize> b;
    array<float, Nsize> c;
    Matrix<Bsize, Nsize> A;
    float v {0};

    friend class SlackFormBuilder<Bsize, Nsize>;

  public:
    float max_value() { return v; }

  public:
    void Pivot(int leave, int enter) {
      Matrix<Bsize, Nsize> newA;
      array<float, Bsize> newb;
      array<float, Nsize> newc;

      newb[leave] = b[leave] / A[leave][enter];
      for (int i = 0; i < Nsize; ++i) if (enter != i) {
        newA[leave][i] = A[leave][i] / A[leave][enter];
      }
      newA[leave][enter] = 1.0f / A[leave][enter];

      for (int i = 0; i < Bsize; ++i) if (leave != i) {
        newb[i] = b[i] - A[i][enter] * newb[leave];
        for (int j = 0; j < Nsize; ++j) if (enter != j) {
          newA[i][j] = A[i][j] - A[i][enter] * newA[leave][j];
        }
        newA[i][enter] = -A[i][enter]*newA[leave][enter];
      }

      v += c[enter] * newb[leave];

      for (int i = 0; i < Nsize; ++i) if (enter != i) {
        newc[i] = c[i] - c[enter] * newA[leave][i];
      }
      newc[enter] = -c[enter] * newA[leave][enter];

      A = newA;
      b = newb;
      c = newc;
    }

    bool Simplex() {
      for (;;) {
        int enter = -1;
        for (int i = 0; i < Nsize; ++i) if (c[i] > 0) {
          enter = i;
        }
        if (-1 == enter) break;

        float min_delta = 10e11;
        int min_index = -1;
        for (int i = 0; i < Bsize; ++i) if (A[i][enter] > 0) {
          float temp_delta = b[i] / A[i][enter];
          if (temp_delta < min_delta) {
            min_delta = temp_delta;
            min_index = i;
          }
        }
        if (-1 == min_index) {
          return false;
        }
        Pivot(min_index, enter);
      }
      return true;
    }

    void DebugDisplay() {
      printf("Z = %.2f\t", v);
      for (int i = 0; i < Nsize; ++i) {
        printf("%.2f\t", c[i]);
      }
      printf("\n");

      for (int i = 0; i < Bsize; ++i) {
        printf("X%d= %.2f\t", i, b[i]);
        for (int j = 0; j < Nsize; ++j) {
          printf("%.2f\t", A[i][j]);
        }
        printf("\n");
      }
      printf("\n");
    }
};

template<int Bsize, int Nsize> 
class SlackFormBuilder {
  private:
    SlackForm<Bsize, Nsize> *_slack;

  public:
    SlackFormBuilder() {
      _slack = new SlackForm<Bsize, Nsize>();
    }
    SlackFormBuilder& set_b(const array<float, Bsize> &b) {
      _slack->b = b;
      return *this;
    }
    SlackFormBuilder& set_c(const array<float, Nsize> &c) {
      _slack->c = c;
      return *this;
    }
    SlackFormBuilder& set_A(const Matrix<Bsize, Nsize> &A) {
      _slack->A = A;
      return *this;
    }
    SlackFormBuilder& set_v(float v) {
      _slack->v = v;
      return *this;
    }
    SlackForm<Bsize, Nsize>* slack() {
      return _slack;
    }
};

void TestPivot() {

  vector<vector<float>>  A_ary {
    {1, 1, 1},
    {2, 2, 5},
    {4, 1, 2},
  };

  auto slack = SlackFormBuilder<3, 3>()
                .set_b({{30, 24, 36}})
                .set_c({{3, 1, 2}})
                .set_v(0)
                .set_A(Matrix<3, 3>(A_ary))
                .slack();
  slack->DebugDisplay();
  slack->Pivot(2, 0);
  slack->DebugDisplay();
  slack->Simplex();
  cout << slack->max_value() << endl;
} 

int main() {
  TestPivot();
  return 0;
}
