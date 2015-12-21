/*******************************************************************************
 * Author: Dyinnz.HUST.UniqueStudio
 * Email:  ml_143@sina.com
 * Github: https://github.com/dyinnz
 * Date:   2015-12-11
 ******************************************************************************/

#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

int gcd(int a, int b) {
  return 0 == b ? a : gcd(b, a%b);
}

tuple<int, int, int> extented_gcd(int a, int b) {
  if (0 == b) {
    return make_tuple(a, 1, 0);
  } else {
    int d, x, y;
    tie(d, x, y) = extented_gcd(b, a % b);
    return make_tuple(d, y, x - a/b * y);
  }
}

bool ModularLinearEquationSolver(int a, int b, int n) {
  int d, x, y;
  tie(d, x, y) = extented_gcd(a, n);
  if (0 == b % d) {
    int x0 = x * b/d % n;
    for (int i = 0; i < d; ++i) {
      cout << (x0 + i * n/d) % n << ' ';
    }
    cout << endl;
    return true;
  } else {
    return false;
  }
}

int ModularExponentiation(int a, int b, int n) {
  int c = 0;
  int d = 1;
  int k = 0;
  for (int i = 31; i > 0; ++i) if (b >> i) {
    k = i;
    break;
  }
  for (int i = k; i >= 0; --i) {
    c *= 2;
    d = d*d % n;
    if ((b >> i) & 1) {
      c += 1;
      d = d*a % n;
    }
  }
  return d;
}

int main() {
  cout << gcd(15, 20) << endl;
  int d, x, y;
  tie(d, x, y) = extented_gcd(99, 78);
  cout << d << ' '
       << x << ' '
       << y << endl;
  cout << ModularLinearEquationSolver(14, 30, 100) << endl;
  return 0;
}

