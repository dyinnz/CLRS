#include <iostream>
#include <algorithm>
#include <cstring>
#include <climits>
#include <vector>
#include <list>

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
        cout << X[i-1];
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
        b[i][0] = '*';
    }
    for (size_t i = 0; i <= Y.size(); ++i) {
        c[0][i] = 0;
        b[0][i] = '*';
    } 
    for (size_t i = 1; i <= X.size(); ++i) {
        for (size_t j = 1; j <= Y.size(); ++j) {
            if (X[i-1] == Y[j-1]) {
                c[i][j] = c[i-1][j-1] + 1;
                b[i][j] = '\\';
            } else if (c[i-1][j] >= c[i][j-1]) {
                c[i][j] = c[i-1][j];
                b[i][j] = '|';
            } else {
                c[i][j] = c[i][j-1];
                b[i][j] = '-';
            }
        }
    }
    for (size_t i = 0; i <= X.size(); ++i) {
        for (size_t j = 0; j <= Y.size(); ++j) {
            cout << b[i][j] << ' ';
        }
        cout << endl;
    }

    cout << "LCS: " << c[X.size()][Y.size()] << endl;
    PrintLCS(&b[0][0], Y.size()+1, X, X.size(), Y.size());
    cout << endl;
}

void LCSLengthCompress(const vector<char> &X, const vector<char> &Y) {
    if (X.size() < Y.size()) {
        LCSLengthCompress(Y, X);
    }
    int c[Y.size()+1];
    memset(c, 0, sizeof(c));

    for (size_t i = 1; i <= X.size(); ++i) {
        //for (size_t j = Y.size(); j >= 1; --j) {
        for (size_t j = 1; j <= Y.size(); ++j) {
            if(X[i-1] == Y[j-1]) {
                c[j] = c[j-1] + 1;
            } else if (c[j-1] > c[j]) {
                c[j] = c[j-1];
            }
        }
    }
    cout << "LCS: " << c[Y.size()] << endl;
}

void PrintLIS(int s[], const vector<int> &A, int pos) {
    if (0 != pos) {
        PrintLIS(s, A, s[pos]);
        cout << A[pos-1] << ' ';
    }
}

void LongestIncreasingSubsequnce(const vector<int> &A) {
    size_t n = A.size();
    int c[n+1];
    int s[n+1];
    memset(c, 0, sizeof(c));
    memset(s, 0, sizeof(s));

    for (size_t i = 1; i <= n; ++i) {
        c[i] = 1;
        //s[i] = i;
    }

    for (size_t i = 1; i <= n; ++i) {
        for (size_t j = 1; j < i; ++j) {
            if (A[i-1] > A[j-1] && c[j] + 1 > c[i]) {
                c[i] = c[j] + 1;
                s[i] = j;
            }
        }
    }

    int ans = 0;
    int ans_pos = 0;
    for (size_t i = 0; i <= n; ++i) {
        //cout << c[i] << endl;
        if (c[i] > ans) {
            ans = c[i];
            ans_pos = i;
        }
    }
    cout << "longest: " << ans << endl;

    PrintLIS(s, A, ans_pos);
    cout << endl;
}

void PrintLISQuick(int s[], const vector<int> &A, int pos) {
    if (-1 != pos) {
        PrintLISQuick(s, A, s[pos]);
        cout << A[pos] << ' ';
    }
}

void LIS(const vector<int> &A) {
    vector<int> L;
    int s[A.size()];
    int prev[A.size()];
    memset(s, -1, sizeof(s));
    memset(prev, -1, sizeof(prev));
    
    for (int i = 0; i < int(A.size()); ++i) {
        //auto iter = upper_bound(L.begin(), L.end(), A[i]);
        auto iter = upper_bound(L.begin(), L.end(), i, [&](int x, int y) { return A[x] < A[y]; });
        if (iter == L.end()) {
            //L.push_back(A[i]);
            if (!L.empty()) {
                s[i] = L.back();
            }
            L.push_back(i);
        } else {
            //*iter = A[i];
            //cout << *iter << ' ' << i << endl;
            //s[*iter] = i;
            auto last  = iter;
            if (last != L.begin()) {
                s[i] = *--last;
            }
            *iter = i;
        }
    }

    cout << "longest: " << L.size() << endl;
    PrintLISQuick(s, A, L.back());

    cout << endl;
    cout << "S: ";
    for (int num : s) {
        cout << num << ' ';
    }
    cout << endl;
}

void OptimalBST(const vector<float> &p, const vector<float> &q, int n) {
    float e[n+2][n+1];
    float w[n+2][n+1];
    int root[n+1][n+1];

    for (int i = 1; i <= n+1; ++i) {
        e[i][i-1] = q[i-1];
        w[i][i-1] = q[i-1];
    }

    for (int l = 1; l <= n; ++l) {
        for (int i = 1; i <= n-l+1; ++i) {
            int j = i + l - 1;
            e[i][j] = 10e100;
            w[i][j] = w[i][j-1] + p[j] + q[j];
            for (int r = i; r <= j; ++r) {
                float t = e[i][r-1] + e[r+1][j] + w[i][j];
                if (t < e[i][j]) {
                    e[i][j] = t;
                    root[i][j] = r;
                }
            }
        }
    }
    cout << "Minimum E: " << e[1][n] << endl;
}

void OptimalBSTQuick(const vector<float> &p, const vector<float> &q, int n) {
    float e[n+2][n+1];
    float w[n+2][n+1];
    int root[n+1][n+1];

    for (int i = 1; i <= n+1; ++i) {
        e[i][i-1] = q[i-1];
        w[i][i-1] = q[i-1];
        root[i][i-1] = i;
    }

    for (int l = 1; l <= n; ++l) {
        for (int i = 1; i <= n-l+1; ++i) {
            int j = i + l - 1;
            e[i][j] = 10e100;
            w[i][j] = w[i][j-1] + p[j] + q[j];
            for (int r = max(i, root[i][j-1]); r <= min(j, root[i+1][j]); ++r) {
                float t = e[i][r-1] + e[r+1][j] + w[i][j];
                if (t < e[i][j]) {
                    e[i][j] = t;
                    root[i][j] = r;
                }
            }
        }
    }
    cout << "Minimum E: " << e[1][n] << endl;
}

struct Point {
    float x, y;
};

inline float Distance(const Point &v, const Point &u) {
    return sqrt((v.x - u.x)*(v.x - u.x) + (v.y - u.y)*(v.y - u.y));
}

void BitonicTours(vector<Point> &points) {
    sort(points.begin(), points.end(),
        [](Point &v, Point &u) {
            return v.x < v.x;
        });

    int n = points.size();
    float up[n][n];
    float down[n][n];

    up[1][0] = down[1][0] = Distance(points[0], points[1]);

    for (int i = 2; i <= n-2; ++i) {
        for (int j = 0; j <= i-2; ++j) {
            up[i][j] = up[i-1][j] + Distance(points[i], points[i-1]);
        }
        up[i][i-1] = 1e10;
        for (int j = 0; j <= i-2; ++j) {
            float dist = down[i-1][j] + Distance(points[i], points[j]);
            if (dist < up[i][i-1]) {
                up[i][i-1] = dist;
            }
        }
        for (int j = 0; j <= i-2; ++j) {
            down[i][j] = down[i-1][j] + Distance(points[i], points[i-1]);
        }
        down[i][i-1] = 1e10;
        for (int j = 0; j <= i-2; ++j) {
            float dist = up[i-1][j] + Distance(points[i], points[j]);
            if (dist < down[i][i-1]) {
                down[i][i-1] = dist;
            }
        }
    }
    
    float ans = 1e10;
    for (int i = 0; i <= n-3; ++i) {
        float dist = up[n-2][i] + Distance(points[n-2], points[n-1]) + Distance(points[n-1], points[i]);
        if (dist < ans) {
            ans = dist;
        }
    }
    cout << ans << endl;
}



int main() {

    int price[] = {0, 1, 5, 8, 9, 10, 17, 17, 20, 24, 30};
    //BottomUpCutRod(price, 10);
    BottomUpCutRod(price, 7);
    MemoizedCutRod(price, 7);

    int p[] = {30, 35, 15, 5, 10, 20, 25};
    MatrixChainOrder(p, 6);

//    vector<char> X {'A', 'B', 'C', 'D', 'E', 'F' };
//    vector<char> Y {'C', 'B', 'C', 'A', 'E', 'F' };
    vector<char> X {'A', 'B', 'C', 'B', 'D', 'A', 'B'};
    vector<char> Y {'B', 'D', 'C', 'A', 'B', 'A'};
    LCSLength(X, Y);
    LCSLengthCompress(X, Y);

    //vector<int> A {30, 35, 15, 5, 10, 20, 15};
    vector<int> A {0, 1, 5, 8, 9, 10, 17, 17, 20, 24, 30};
    LongestIncreasingSubsequnce(A);
    LIS(A);

    {
        vector<float> p {0, 0.15, 0.10, 0.05, 0.10, 0.20};
        vector<float> q {0.05, 0.10, 0.05, 0.05, 0.05, 0.10};
        OptimalBST(p, q, 5);
        OptimalBSTQuick(p, q, 5);
    }

    {
        vector<Point> points { {0, 6}, {1, 0}, {2, 3}, {5, 4}, {6, 1}, {7, 5}, {8, 2} };
        BitonicTours(points);
    }

    return 0;
}

