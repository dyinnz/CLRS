#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdio>
#include <queue>
#include <functional>

using namespace std;

vector<int> GreedyActivitySelection(const vector<int> &s, const vector<int> &f) {
  vector<int> A {0};
  int k = 0;
  for (size_t i = 1; i < s.size();  ++i) {
    if (s[i] >= f[k]) {
      A.push_back(i);
      k = i;
    }
  }
  return move(A);
}

vector<int> GreedyActivitySelectionLast(const vector<int> &s, const vector<int> &f) {
  int n = int(s.size());
  vector<int> index(n);
  for (int i = 0; i < n; ++i) {
    index[i] = i;
  }

  auto fun = [&](int x, int y) { return s[index[x]] < s[index[y]]; };
  sort(index.begin(), index.end(), fun);

  vector<int> A {index[n-1]};
  int k = n-1;
  for (int i = n-2; i >= 0; --i) {
    if (f[index[i]] <= s[index[k]]) {
      A.push_back(index[i]);
      k = i;
    }
  }

  return move(A);
}

int IntervalGraphColor(const vector<int> &s, const vector<int> &f) {
  enum Type { kFinish, kStart };
  int n = int(s.size());
  vector<pair<int, Type>> activities;

  activities.reserve(2*n);
  for (auto num : s) {
    activities.push_back( {num, kStart} );
  }
  for (auto num : f) {
    activities.push_back( {num, kFinish} );
  }

  sort(activities.begin(), activities.end());

  int max_hall = 0;
  int hall = 0;
  for (auto &p : activities) {
    auto type = p.second;
    if (kFinish == type) {
      --hall;
    } else {
      if (++hall > max_hall) {
        max_hall = hall;
      }
    }
  }
  return max_hall;
}

size_t Partition(vector<pair<int, int>> &items, size_t l, size_t r) {
  size_t p = rand()%(r-l) + l;
  swap(items[r], items[p]);

  auto x = r;
  size_t i = l;
  for (size_t j = l; j < r; ++j) {
    if (items[j].first * items[x].second >= items[x].first * items[j].second) {
      swap(items[i++], items[j]);
    }
  }
  swap(items[i], items[r]);
  return i;
}

float FractionalKnapsack(int total_w, vector<pair<int, int>> &items) {
  int sum_w = 0;
  float supply = 0.0;

  size_t l = 0, r = items.size()-1;
  size_t q = 0;

  while (true) {
    if (l != r) {
      q = Partition(items, l, r);
    } else {
      q = l;
    }

    sum_w = 0;
    for (size_t i = 0; i < q; ++i) {
      sum_w += items[i].second;
    }

    if (sum_w <= total_w) {
      if (sum_w + items[q].second > total_w) {
        supply = float(total_w - sum_w) / items[q].second * items[q].first;
        break;
      } else if (q == items.size() - 1) {
                supply = items[q].first;
                break;
            } else {
                l = q + 1;
            }
        } else {
            r = q + 1;
        }
    }

    float ans = 0.0;
    for (size_t i = 0; i < q; ++i) {
        ans += items[i].first;
    }
    ans += supply;
    return ans;
}

struct CharNode {
    CharNode() : left(0), right(0), freq(0), c(0) {}
    CharNode *left, *right;
    int freq;
    char c;
};

void ReleaseCharNode(CharNode *node) {
    if (node) {
        ReleaseCharNode(node->left);
        ReleaseCharNode(node->right);
        delete node;
    }
}

void PrintTree(CharNode *node, int deep=0) {
    if (node) {
        PrintTree(node->left, deep+1);
        if (node->c) {
            for (int i = 0; i < deep*2; ++i) {
                cout << ' ';
            }
            cout << node->c << endl;
        }
        PrintTree(node->right, deep+1);
    }
}

CharNode* Huffman(const vector<char> &chars, const vector<int> &frequences) {
    int n = int(chars.size());

    auto cmp = [](CharNode *x, CharNode *y) { return x->freq > y->freq; };
    priority_queue<CharNode*, vector<CharNode*>, function<bool(CharNode* x, CharNode*y)>> heap(cmp);

    for (int i = 0; i < n; ++i) {
        CharNode *p = new CharNode;
        p->c = chars[i];
        p->freq = frequences[i];
        heap.push(p);
    }
    
    for (int i = 1; i <= n-1; ++i) {
        CharNode *p = new CharNode;
        p->left = heap.top(); heap.pop();
        p->right = heap.top(); heap.pop();
        p->freq = p->left->freq + p->right->freq;
        heap.push(p);
    }

    return heap.top();
}

int main() {
    vector<int> s { 1, 3, 0, 5, 3, 5, 6, 8, 8, 2, 12 }; 
    vector<int> f { 4, 5, 6, 7, 9, 9, 10, 11, 12, 14, 16};
    auto result = GreedyActivitySelection(s, f);
    for (auto a : result) {
        cout << a << ' ';
    }
    cout << endl;

    auto result2 = GreedyActivitySelectionLast(s, f);
    for (auto a : result2) {
        cout << a << ' ';
    }
    cout << endl;

    cout << IntervalGraphColor(s, f) << endl;

    vector<pair<int, int>> items = {{60, 10}, {100, 20}, {120, 30}};
    cout << FractionalKnapsack(50, items) << endl;
    for (auto p : items) {
        cout << p.first << ' ' << p.second << endl;
    }
    cout << endl;

    CharNode *tree = Huffman({'a', 'b', 'c', 'd', 'e', 'f'}, {45, 13, 12, 16, 9, 5});
    PrintTree(tree);
    return 0;
}

