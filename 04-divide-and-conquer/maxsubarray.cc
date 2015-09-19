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


int main() {
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
    return 0;
}


