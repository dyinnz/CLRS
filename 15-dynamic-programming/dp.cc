#include <iostream>
#include <algorithm>

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

int main() {

    int price[] = {0, 1, 5, 8, 9, 10, 17, 17, 20, 24, 30};
    //BottomUpCutRod(price, 10);
    BottomUpCutRod(price, 7);
    MemoizedCutRod(price, 7);

    return 0;
}
