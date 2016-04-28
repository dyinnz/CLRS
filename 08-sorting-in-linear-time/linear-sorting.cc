#include <iostream>
#include <vector>
#include <forward_list>
#include <algorithm>
#include <iterator>
#include <climits>
using namespace std;

vector<int> CountintSort(vector<int> &A, size_t k) {
  vector<int> B(A.size());
  vector<int> C(k, 0);

  for (int num : A) {
    ++C[num];
  }
  for (size_t i = 1; i < k; ++i) {
    C[i] += C[i-1];
  }
  for (auto it = A.rbegin(); it != A.rend(); ++it) {
    B[--C[*it]] = *it;
  }
  return move(B);
}

inline size_t count_bit(size_t num) {
  size_t c = 0;
  while (num) {
    ++c;
    num &= num - 1;
  }
  return c;
}

void RadixSort(vector<int> &A, int maxv, size_t r) {
  size_t mask = (1 << r) - 1;
  size_t digit = count_bit(maxv);

  vector<int> B(A.size());
  vector<int> C(1<<r, 0);

  for (size_t i = 0; i < digit; i += r) {
    B.assign(A.size(), 0);
    C.assign(1<<r, 0);

    for (int num : A) {
      ++C[ (num>>i) & mask ];
    }
    for (size_t j = 1; j < (1u<<r); ++j) {
      C[j] += C[j-1];
    }
    for (auto it = A.rbegin(); it != A.rend(); ++it) {
      B[ --C[(*it>>i) & mask] ] = *it;
    }
    swap(B, A);
  }
}

/* Supposed that the elements of A is in the interval [0,1)
*/
void BucketSort(vector<float> &A) {
  vector<forward_list<float>> buckets(A.size());
  for (float num : A) {
    buckets[int(num*A.size())].push_front(num);
  }
  A.clear();
  for (auto bucket : buckets) {
    bucket.sort();
    copy(bucket.begin(), bucket.end(), back_inserter(A));
  }
}

struct Node {
  Node() : number(INT_MIN), left(0), right(0) {}
  int number;
  union {
    struct { Node *left, *right; };
    struct { size_t lower, upper; };
  };
};

Node* GetInterval(Node *p, int number) {
  while (p && INT_MIN != p->number) {
    if (number < p->number) {
      p = p->left;
    } else {
      p = p->right;
    }
  }
  return p;
}

size_t Partition(vector<int> &red, size_t l, size_t r, int pivot) {
  size_t pivot_index = 0;
  size_t k = l;
  for (size_t i = l; i <= r; ++i) {
    if (red[i] < pivot) {
      swap(red[k++], red[i]);
    } else if (red[i] == pivot) {
      pivot_index = k;
      swap(red[k++], red[i]);
    }
  }
  swap(red[k-1], red[pivot_index]);
  return k-1;
}

void FreeTree(Node *root) {
  if (!root) {
    Node *temp = root;
    if (INT_MIN != temp->number) {
      FreeTree(root->left);
      FreeTree(root->right);
    }
    delete temp;
  }
}

void MatchJugs(vector<int> &red, vector<int> &blue) {
  Node *root = new Node;
  root->lower = 0;
  root->upper = red.size()-1;

  vector<pair<int, int>> answer;

  for (int water : blue) {
    Node *p = GetInterval(root, water);
    size_t index = Partition(red, p->lower, p->upper, water);

    p->number = water;

    Node *left = new Node;
    left->lower = p->lower;
    left->upper = index-1;
    p->left = left;

    Node *right = new Node;
    right->lower = index+1;
    right->upper = p->upper;
    p->right = right;

    answer.push_back({red[index], water});
    cout << red[index] << '\t' << water << endl;
  }

  FreeTree(root);
}

int main() {
  srand(time(NULL));
  vector<int> A(30);

  generate(A.begin(), A.end(), [](){ return (rand()%100); });
  for (int num : A) {
    cout << num << ' ';
  }
  cout << endl;
  //auto B = CountintSort(A, 0xffffffff);
  RadixSort(A, 0xffffffff, 8);
  for (int num : A) {
    cout << num << ' ';
  }
  cout << endl;

  vector<float> B(30);

  generate(B.begin(), B.end(), [](){ return rand() / float(RAND_MAX); } );
  for (float num : B) {
    cout << num << ' ';
  }
  cout << endl;
  BucketSort(B);
  for (float num : B) {
    cout << num << endl;
  }
  cout << endl;

  /*
     vector<int> C(30);
     generate(C.begin(), C.end(), [](){ return (rand()&0xffff); });
     vector<int> D(C);
     MatchJugs(C, D);

     sort(C.begin(), C.end());
     for (int num : C) {
     cout << num << " ";
     }
     cout << endl;
     */

  return 0;
}


