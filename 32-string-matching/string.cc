/*******************************************************************************
 * Author: Dyinnz.HUST.UniqueStudio
 * Email:  ml_143@sina.com
 * Github: https://github.com/dyinnz
 * Date:   2015-12-21
 ******************************************************************************/

#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

int power_mod(int b, int e, int m) {
  if (0 == e) {
    return 1;
  } else if (1 == e) {
    return b;
  } else {
    if (1 & e) {
      return power_mod(b*b, e/2, m) * b % m;
    } else {
      return power_mod(b*b, e/2, m) % m;
    }
  }
}

char* BrainKarpMatcher(char *T, const char *P, int d, int prime) {
  int n = strlen(T);
  int m = strlen(P);
  int h = power_mod(d, m - 1, prime);
  int t = 0, p = 0;
  for (int i = 0; i < m; ++i) {
    t = (d*t + T[i]) % prime;
    p = (d*p + P[i]) % prime;
  }
  for (int s = 0; s < n - m; ++s) {
    if (t == p) {
      int is_match = true;
      for (int i = 0; i < m; ++i) if (T[s + i] != P[i]) {
        is_match = false;
        break;
      }
      if (is_match) {
        return T + s;
      }
    }
    t = (d * (t - T[s]*h) + T[s + m]) % prime;
    if (t < 0) {
      t += prime;
    }
  }
  return nullptr;
}

vector<int> ComputePrefixFunction(const char *P) {
  int m = strlen(P);
  vector<int> trans(m);
  trans[0] = -1;
  int k = -1;
  for (int q = 1; q < m; ++q) {
    while (k >= 0 && P[k+1] != P[q]) {
      k = trans[k];
    }
    if (P[k+1] == P[q]) {
      k += 1;
    }
    trans[q] = k;
  }
  return trans;
}

char* KMPMatcher(char *T, const char *P) {
  int n = strlen(T);
  int m = strlen(P);
  auto trans = ComputePrefixFunction(P);
  int q = 0;
  for (int i = 0; i < n; ++i) {
    while (q > 0 && P[q] != T[i]) {
      q = trans[q];
    }
    if (P[q] == T[i]) {
      q += 1;
    }
    if (q == m) {
      return T + i - m + 1;
      // cout << "Pattern occurs with shift " << i - m + 1 << endl;
      // q = trans[q];
    }
  }
  // cout << "No such sub string" << endl;
  return nullptr;
}

int main() {
  char t[] = "hehehaha";
  char p[] = "he";
  cout << (BrainKarpMatcher(t, p, 127, 23)) << endl;
  cout << (KMPMatcher(t, p)) << endl;
  return 0;
}

