#include <climits>
#include <iostream>
#include <algorithm>
#include <vector>
#include <forward_list>
#include <list>

using namespace std;

/******************************************************************************/

class DirectAddress {
  public:
    DirectAddress(size_t size) : size_(size) {
      index_array_ = new size_t[size];
    }

    ~DirectAddress() {
      delete [] index_array_;
    }

    void Insert(int x) {
      index_array_[x] = T_.size();
      T_.push_back(x);
    }

    const int* Search(int x) {
      size_t index = index_array_[x];
      if (index < T_.size() && T_[index] == x) {
        return &T_[index];
      } else {
        return 0;
      }
    }

    void Delete(int x) {
      size_t index = index_array_[x];
      if (index < T_.size() && T_[index] == x) {
        swap(T_.back(), T_[index]);
        T_.pop_back();

        index_array_[T_[index]] = index;
        index_array_[x] = UINT_MAX;
      }
    }

  private:
    size_t *index_array_;
    size_t size_;
    vector<int> T_; 
};

/******************************************************************************/

class ChainningHash {
  public:
    ChainningHash() {
      T_.resize(kSize);
    }

    void Insert(int x) {
      auto &L = T_[Hash(x)];
      if (L.end() == find(L.begin(), L.end(), x)) {
        L.push_front(x);
      }
    }

    const int* Search(int x) {
      auto &L = T_[Hash(x)];
      auto it = find(L.begin(), L.end(), x);
      if (it != L.end()) {
        return &*it;
      } else {
        return 0;
      }
    }

    void Delete(int x) {
      auto &L = T_[Hash(x)];
      for (auto it = L.begin(), last = L.before_begin(); 
          it != L.end(); ++it, ++last) {
        if (*it == x) {
          L.erase_after(last);
          return;
        }
      }
    }

  private:
    int Hash(int x) {
      return x % kSize;
    }

    static const int kSize = 10007;

    vector<forward_list<int>> T_;
};

/******************************************************************************/

class OpenAddressHash {
  public:
    OpenAddressHash() {
      T_.resize(kSize, INT_MIN);
    }

    void Insert(int x) {
      int h = Hash(x);
      for (int i = 0; i < kSize; ++i) {
        if (kNil != T_[h] || kDelete != T_[h]) {
          T_[h] = x;
          return;
            }
            h = (h + i) % kSize;
        }
        cout << "hash table overflow" << endl;
    }

    const int* Search(int x) {
        int h = Hash(x);
        for (int i = 0; INT_MIN != T_[h] && i < kSize; ++i) {
            if (x == T_[h]) {
                return &T_[h];
            }
        }
        return 0;
    }

    void Delete(int x) {
        int *p = const_cast<int*>(Search(x));
        if (p) {
            *p = kDelete;
        }
    }

private:
    int Hash(int x) {
        return ((x * s) >> 2 ) & 16383;
    }

    static const int kSize = 16384; // 2^14
    static const int s = 2654435769; // (sqrt(5)-1)/2 * 2^32
    static const int kNil = INT_MIN;
    static const int kDelete = INT_MAX;

    vector<int> T_;
};


int main() {
    DirectAddress da(100);
    for (int i = 0; i < 20; i += 3) {
        da.Insert(i);
    }

    for (int i = 0; i < 20; i += 3) {
        cout << *da.Search(i) << endl;
    }
    da.Delete(6);
    da.Delete(12);

    for (int i = 0; i < 20; i += 3) {
        auto p = da.Search(i);
        if (!p) {
            cout << "null" << endl;
        } else {
            cout << *p << endl;
        }
    }

    cout << endl;
    cout << sizeof(forward_list<int>) << endl;
    cout << sizeof(list<int>) << endl;
    cout << sizeof(vector<int>) << endl;
    cout << endl;

    ChainningHash ch;
    for (int i = 0; i < 20; i += 3) {
        ch.Insert(i);
    }

    for (int i = 0; i < 20; i += 3) {
        cout << *ch.Search(i) << endl;
    }
    ch.Insert(10007);
    ch.Delete(6);
    ch.Delete(12);

    for (int i = 0; i < 20; i += 3) {
        auto p = ch.Search(i);
        if (!p) {
            cout << "null" << endl;
        } else {
            cout << *p << endl;
        }
    }

    cout << endl;

    OpenAddressHash oah;
    for (int i = 0; i < 20; i += 3) {
        oah.Insert(i);
    }

    for (int i = 0; i < 20; i += 3) {
        cout << *oah.Search(i) << endl;
    }
    oah.Insert(10007);
    oah.Delete(6);
    oah.Delete(12);

    for (int i = 0; i < 20; i += 3) {
        auto p = oah.Search(i);
        if (!p) {
            cout << "null" << endl;
        } else {
            cout << *p << endl;
        }
    }

    return 0;
}
