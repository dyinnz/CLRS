/*******************************************************************************
 * Author: Dyinnz.HUST.UniqueStudio
 * Email:  ml_143@sina.com
 * Github: https://github.com/dyinnz
 * Date:   2015-12-11
 ******************************************************************************/

#include <iostream>
#include <vector>
#include <complex>
#include <cassert>

using namespace std;

int gcd(int m, int n) {
  return 0 == n ? m : gcd(n, m%n);
}

class Fraction {
  private:
    int _num {0};
    int _den {1};

  private:
    void reduction() {
      if (0 != _num) {
        int divisor = gcd(_num, _den);
        _num /= divisor;
        _den /= divisor;
      } else {
        _den = 1;
      }
    }

  public:
    void set(int num, int den) {
      _num = num;
      _den = den;
    }

    int num() const { return _num; }
    int den() const { return _den; }

    int &num() { return _num; }
    int &den() { return _den; }

    Fraction() = default;
    Fraction& operator=(const Fraction &) = default;

    Fraction(int num) : _num(num), _den(1) {}
    Fraction& operator=(int num) {
      _num = num;
      _den = 1;
      return *this;
    }

    Fraction(int num, int den) : _num(num), _den(den) {
      reduction();
    }

    operator float() {
      return float(_num) / float(_den);
    }

    Fraction& operator+=(const Fraction &rhs) {
      _num = _num*rhs._den + _den*rhs._num;
      _den *= rhs._den;
      reduction();
      return *this;
    }

    Fraction& operator-=(const Fraction &rhs) {
      _num = _num*rhs._den - _den*rhs._num;
      _den *= rhs._den;
      reduction();
      return *this;
    }

    Fraction& operator*=(const Fraction &rhs) {
      _num *= rhs._num;
      _den *= rhs._den;
      reduction();
      return *this;
    }

    Fraction& operator/=(const Fraction &rhs) {
      _num *= rhs._den;
      _den *= rhs._num;
      reduction();
      return *this;
    }
};

Fraction operator+(const Fraction &lhs, const Fraction &rhs) {
  Fraction temp(lhs);
  temp += rhs;
  return temp;
}

Fraction operator-(const Fraction &lhs, const Fraction &rhs) {
  Fraction temp(lhs);
  temp -= rhs;
  return temp;
}

Fraction operator*(const Fraction &lhs, const Fraction &rhs) {
  Fraction temp(lhs);
  temp *= rhs;
  return temp;
}

Fraction operator/(const Fraction &lhs, const Fraction &rhs) {
  Fraction temp(lhs);
  temp /= rhs;
  return temp;
}

ostream& operator<<(ostream &out, const Fraction &rhs) {
  out << rhs.num() << '/' << rhs.den();
  return out;
}

void DisplayPolynomials(const vector<complex<float>> &A) {
  for (auto &c : A) {
    cout << c << ' ';
  }
  cout << endl;
}

vector<complex<float>> RecursiveFFT(const vector<complex<float>> &A) {
  size_t n = A.size();
  if (1 == A.size()) {
    return A;
  }

  vector<complex<float>> A0(n/2), A1(n - n/2);
  for (size_t i = 0; i < n; ++i) {
    if (i & 1) {
      A1[i / 2] = A[i];
    } else {
      A0[i / 2] = A[i];
    }
  }

  auto Y0 = RecursiveFFT(A0);
  auto Y1 = RecursiveFFT(A1);
  vector<complex<float>> Y(n);

  complex<float> wn(0.0, M_PI * 2.0 / n);
  wn = exp(wn);
  complex<float> w(1.0, 0.0);
  for (int k = 0; k < n/2; ++k) {
    Y[k] = Y0[k] + w * Y1[k];
    Y[k + n/2] = Y0[k] - w * Y1[k];
    w *= wn;
  }
  return Y;
}

void BitReverseCopy(const vector<complex<float>> &former, 
                    vector<complex<float>> &latter,
                    const vector<int> &rev) {
  assert(former.size() == latter.size());
  for (int k = 0; k < former.size(); ++k) {
    latter[rev[k]] = former[k];
  }
}

vector<int> ComputeReverseBit(size_t s) {
  size_t n = 1 << s;
  auto compute = [=] (int k) { 
    int result = 0;
    for (size_t i = 0; i < s; ++i) {
      result |= ((k >> i) & 1) << (s - i - 1);
    }
    return result;
  };

  vector<int> rev(n);
  for (int i = 0; i < int(n); ++i) {
    rev[i] = compute(i); 
  }
  return rev;
}

vector<complex<float>> IterativeFFT(vector<complex<float>> &former) {
  int s = 0;
  for (int i = 31; i >= 0; --i) if ((former.size() >> i) & 1) {
    s = i;
    break;
  }
  vector<int> rev = ComputeReverseBit(s);
  vector<complex<float>> latter(former.size());
  BitReverseCopy(former, latter, rev);

  for (int i = 1; i <= s; ++i) {
    int m = 1 << i;
    complex<float> wn(0.0, M_PI * 2.0 / m);
    wn = exp(wn);
    for (int k = 0; k < int(latter.size()); k += m) {
      complex<float> w(1.0, 0.0);
      for (int j = 0; j < m/2; ++j) {
        auto t = w * latter[k + j + m/2];
        auto u = latter[k + j];
        latter[k + j] = u + t;
        latter[k + j + m/2] =u - t;
        w *= wn;
      }
    }
  }
  return latter;
}

vector<complex<float>> DoInverseDFT(const vector<complex<float>> &A) {
  size_t n = A.size();
  if (1 == A.size()) {
    return A;
  }

  vector<complex<float>> A0(n/2), A1(n - n/2);
  for (size_t i = 0; i < n; ++i) {
    if (i & 1) {
      A1[i / 2] = A[i];
    } else {
      A0[i / 2] = A[i];
    }
  }

  auto Y0 = DoInverseDFT(A0);
  auto Y1 = DoInverseDFT(A1);
  vector<complex<float>> Y(n);

  complex<float> wn(0.0, -M_PI * 2.0 / n);
  wn = exp(wn);
  complex<float> w(1.0, 0.0);
  for (int k = 0; k <= n/2 - 1; ++k) {
    Y[k] = (Y0[k] + w * Y1[k]);
    Y[k + n/2] = (Y0[k] - w * Y1[k]);
    w *= wn;
  }
  return Y;
}

vector<complex<float>> InverseDFT(const vector<complex<float>> &A) {
  auto Y = DoInverseDFT(A);
  for (auto &c : Y) {
    c /= float(Y.size());
    if (abs(c.imag()) < 1e-5) {
      c = complex<float>(c.real(), 0);
    }
  }
  return Y;
}

vector<complex<float>> MulFunction(const vector<complex<float>> &lhs, 
                                   const vector<complex<float>> &rhs) {
  vector<complex<float>> result(lhs.size() + rhs.size() - 1);
  for (size_t i = 0; i < lhs.size(); ++i) {
    for (size_t j = 0; j < rhs.size(); ++j) {
      result[i+j] += lhs[i] * rhs[j];
    }
  }
  return result;
}

vector<complex<float>> MulCorrespondPoints(const vector<complex<float>> &lhs, 
                                 const vector<complex<float>> &rhs) {
  if (lhs.size() < rhs.size()) {
    return MulCorrespondPoints(rhs, lhs);
  }
  vector<complex<float>> result(lhs);
  for (size_t i = 0; i < rhs.size(); ++i) {
    result[i] *= rhs[i];
  }
  return result;
}

vector<complex<float>> MulPolyPoints(const vector<complex<float>> &lhs,
                                     const vector<complex<float>> &rhs) {
  if (lhs.size() < rhs.size()) {
    return MulPolyPoints(rhs, lhs);
  }

  size_t size = lhs.size() * 2;
  vector<complex<float>> ex_lhs(size), ex_rhs(size);
  copy(lhs.begin(), lhs.end(), ex_lhs.begin());
  copy(rhs.begin(), rhs.end(), ex_rhs.begin());
  auto fft_lhs = RecursiveFFT(ex_lhs);
  auto fft_rhs = RecursiveFFT(ex_rhs);
  auto result = InverseDFT(MulCorrespondPoints(fft_lhs, fft_rhs));
  result.pop_back();
  return result;
}

int main() {
  vector<complex<float>> A1 { {1, 0}, {2, 0}, {3, 0}, {4, 0} };
  vector<complex<float>> A2 { {1, 0}, {2, 0}, {3, 0}, {4, 0} };
  DisplayPolynomials(MulFunction(A1, A2));
  DisplayPolynomials(MulCorrespondPoints(A1, A2));
  DisplayPolynomials(MulPolyPoints(A1, A2));

  auto rev = ComputeReverseBit(2);
  for (auto r : rev) {
    cout << r << ' ';
  }
  cout << endl;

  DisplayPolynomials(RecursiveFFT(A1));
  DisplayPolynomials(IterativeFFT(A1));

  return 0;
}


