#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

void RandomizeInPlace(vector<int> &A) {
  srand(time(NULL));
  for (size_t i = 0; i < A.size(); ++i) {
    swap(A[i], A[rand()%(A.size()-i) + i]);
  }
}

int main() {
  vector<int> A;
  for (size_t i = 0; i < 30; ++i) {
    A.push_back(i);
  }
  RandomizeInPlace(A);
  for (int num : A) {
    cout << num << " ";
  }
  cout << endl;
  return 0;
}
