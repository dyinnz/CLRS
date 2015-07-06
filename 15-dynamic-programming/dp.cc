#include <iostream>
#include <algorithm>
#include <cstring>
#include <climits>
#include <vector>

using namespace std;

void BottomUpCutRod(int price[], int n) {
    int r[n+1] {0}, s[n+1] {0};
    r[0] = 0;
    for (int j = 1; j <= n; ++j) {
        r[j] = -1;
        for (int i = 1; i <= j; ++i) {
            if (r[j] < price[i] + r[j-i]) {
                r[j] = price[i] + r[j-i];
                s[j] = i;
            } 
        }
    }

    cout << r[n] << endl;

    int l = n;
    while (l > 0) {
        cout << s[l] << endl;
        l -= s[l];
    }
}

int MemoizedCutRodAux(int price[], int n, int r[], int s[]) {
    if (0 == n) {
        return 0;
    } else if (r[n] > 0) {
        return r[n];
    } else {
        for (int i = 1; i <= n; ++i) {
            int newr = price[i] + MemoizedCutRodAux(price, n-i, r, s);
            if (r[n] < newr) {
                r[n] = newr;
                s[n] = i;
            }
        }
        return r[n];
    }
}

void MemoizedCutRod(int price[], int n) {
    int r[n+1] {0}, s[n+1] {0};
    for (int i = 0; i <= n; ++i) {
        r[i] = -1;
    }
    int ans = MemoizedCutRodAux(price, n, r, s);
    cout << ans << endl;
    int l = n;
    while (l > 0) {
        cout << s[l] << endl;
        l -= s[l];
    }
}

void PrintOptimalParens(int *s, int n, int i, int j) {
    if (i == j) {
        cout << 'A' << i;
    } else {
        cout << '(';
        PrintOptimalParens(s, n, i, *(s+i*n+j));
        PrintOptimalParens(s, n, *(s+i*n+j)+1, j);
        cout << ')';
    }
}

void MatrixChainOrder(int p[], int n) {
    int m[n][n], s[n][n];
    memset(m, 0, sizeof(m));
    memset(s, 0, sizeof(s));
    for (int l = 2; l <= n; ++l) {
        for (int i = 0; i <= n - l; ++i) {
            // closed interval
            int j = i + l - 1;
            m[i][j] = INT_MAX;
            for (int k = i; k <= j; ++k) {
                int cost = m[i][k] + m[k+1][j] + p[i]*p[k+1]*p[j+1];
                if (cost < m[i][j]) {
                    m[i][j] = cost;
                    s[i][j] = k;
                }
            }
        }
    }
    PrintOptimalParens(&s[0][0], n, 0, n-1);
    cout << endl;
}

void PrintLCS(char *b, int bn, const vector<char> &X, int i, int j) {
    if (0 == i || 0 == j) {
        return;
    } else if ('\\' == *(b+i*bn+j)){
        PrintLCS(b, bn, X, i-1, j-1);
        cout << X[i];
    } else if ('|' == *(b+i*bn+j)) {
        PrintLCS(b, bn, X, i-1, j);
    } else {
        PrintLCS(b, bn, X, i, j-1);
    }
}

void LCSLength(const vector<char> &X, const vector<char> &Y) {
    char b[X.size()+1][Y.size()+1];
    int c[X.size()+1][Y.size()+1];
    for (size_t i = 1; i <= X.size(); ++i) {
        c[i][0] = 0;
    }
    for (size_t i = 0; i <= Y.size(); ++i) {
        c[0][i] = 0;
    }
    for (size_t i = 1; i <= X.size(); ++i) {
        for (size_t j = 1; j <= Y.size(); ++j) {
            if (X[i] == Y[i]) {
                c[i][j] = c[i-1][j-1] + 1;
                b[i][j] = '\\';
            } else if (c[i-1][j] > c[i][j-1]) {
                c[i][j] = c[i-1][j];
                b[i][j] = '|';
            } else {
                c[i][j] = c[i][j-1];
                b[i][j] = '-';
            }
        }
    }
    PrintLCS(&b[0][0], X.size()+1, X, X.size(), Y.size());
    cout << endl;
}

int main() {

    int price[] = {0, 1, 5, 8, 9, 10, 17, 17, 20, 24, 30};
    //BottomUpCutRod(price, 10);
    BottomUpCutRod(price, 7);
    MemoizedCutRod(price, 7);

    int p[] = {30, 35, 15, 5, 10, 20, 25};
    MatrixChainOrder(p, 6);

    vector<char> X {'A', 'B', 'C', 'D', 'E', 'F' };
    vector<char> Y {'C', 'B', 'C', 'A', 'E', 'F' };
    LCSLength(X, Y);

    return 0;
}

