/*******************************************************************************
 * Author: Dyinnz.HUST.UniqueStudio
 * Email:  ml_143@sina.com
 * Github: https://github.com/dyinnz
 * Date:   2015-12-11
 ******************************************************************************/

#include <cstdlib>
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
  for (int i = 31; i > 0; --i) if (b >> i) {
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

bool Witness(int a, int n) {
  int u = n-1;
  int t = 0;
  while (0 == (u & 1)) {
    u /= 2;
    t += 1;
  }
  int last_x = ModularExponentiation(a, u, n);
  int x = 0;
  for (int i = 1; i <= t; ++i) {
    x = last_x*last_x % n;
    if (1 == x && 1 != last_x && n-1 != last_x) {
      return true;
    }
    last_x = x;
  }
  return 1 != last_x;
}

bool MillerRabin(int n, int s) {
  for (int j = 0; j < s; ++j) {
    int a = rand() % (n-1) + 1;
    if (Witness(a, n)) {
      return false;
    }
  }
  return true;
}

int main() {
  cout << gcd(15, 20) << endl;
  int d, x, y;
  tie(d, x, y) = extented_gcd(99, 78);
  cout << d << ' '
    << x << ' '
    << y << endl;
  cout << ModularLinearEquationSolver(14, 30, 100) << endl;

  for (int i = 3; i < 30; i +=2) {
    cout << i << "\n" << MillerRabin(i, 5) << endl;
  }
  cout << endl;
  return 0;
}

