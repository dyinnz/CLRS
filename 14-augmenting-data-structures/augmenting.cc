#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class OSTree {
  private:
    enum Color { kRed, kBlack };

  public:
    struct Node {
      int x, size;
      Node *left, *right, *parent;
      Color color;
    };

    OSTree() : nil_(&sentinel_), root_(nil_) {
      sentinel_.parent = sentinel_.left = sentinel_.right = nil_;
      sentinel_.color = Color::kBlack;
      sentinel_.x = 0;
      sentinel_.size = 0;
    }

    ~OSTree() {
      Release(root_);
    }

    Node* Select(int i) {
      ++i;

      //return SelectRecur(root_, i);
      Node *p = root_;
      while (nil_ != p) {
        int r = p->left->size + 1;
        if (i == r) {
          return p;
        } else if (i < r) {
          p = p->left;
        } else {
          p = p->right;
          i -= r;
        }
      }
      return 0;
    }

    int Rank(const Node *p) {
      int r = p->left->size + 1;
      while (root_ != p) {
        if (p == p->parent->right) {
          r += p->parent->left->size + 1;
        }
        p = p->parent;
      }
      return r-1;
    }

    int KeyRank(int x) {
      Node *p = root_;
      int r = 0;
      while (x != p->x) {
        if (nil_ == p) {
          return -1;
        }
        if (x < p->x) {
          p = p->left;
        } else {
          r += p->left->size + 1;
          p = p->right;
        }
      }
      return r;
    }

    Node* Search(int x) {
      Node *p = root_;
      while (nil_ != p && x != p->x) {
        if (x < p->x) {
          p = p->left;
        } else {
          p = p->right;
        }
      }
      return p;
    }

    Node* Insert(int x) {
      Node *last = nil_;
      Node *p = root_;
      while (nil_ != p) {
        // Add
        ++p->size;

        last = p;
        if (x < p->x) {
          p = p->left;
        } else {
          p = p->right;
        }
      }

      Node *inserted = new Node;
      inserted->x = x;
      inserted->left = nil_;
      inserted->right = nil_;
      inserted->color = Color::kRed;
      inserted->parent = last;
      inserted->size = 1;

      if (nil_ == last) {
        root_ = inserted;
        } else if (inserted->x < last->x) {
            last->left = inserted;
        } else {
            last->right = inserted;
        }

        InsertFixUp(inserted);

        return inserted;
    }

    void Delete(Node *p) {
        Node *x = nil_;
        Node *y = p;
        Color y_original_color = y->color;
        if (nil_ == p->left) {
            x = p->right;
            Transplant(p, p->right);
            --p->right->size;
        } else if (nil_ == p->right) {
            x = p->left;
            Transplant(p, p->left);
            --p->left->size;
        } else {
            y = Minimum(p->right);
            x = y->right;

            if (y->parent == p) {
                x->parent = y;
            } else {
                Transplant(y, x);
                --x->size;
                y->right = p->right;
                y->right->parent = y;
            }
            Transplant(p, y);
            y->left = p->left;
            y->left->parent = y;
            y->color = p->color;
        }

        // Add
        Node *t = x->parent;
        while (nil_ != t) {
            --t->size;
            t = t->parent;
        }

        if (Color::kBlack == y_original_color) {
            DeleteFixUp(x);
        }
        delete p;
    }

//private:
    Node* SelectRecur(Node *p, int i) {
        int r = p->size + 1;
        if (i == r) {
            return p;
        } else if (i < r) {
            return SelectRecur(p->left, i);
        } else {
            return SelectRecur(p->right, i - r);
        }
    }

//private:
    void Release(Node *p) {
        if (nil_ != p) {
            Release(p->left);
            Release(p->right);
            delete p;
        }
    }

    Node *Minimum(Node *p) {
        while (nil_ != p->left) {
            p = p->left;
        }
        return p;
    }

    void LeftRotate(Node *p) {
        Node *t = p->right;

        // Turn t's left subtree into p's right subtree
        p->right = t->left;
        if (nil_ != t->left) {
            t->left->parent = p;
        }

        // Link p's parent to t
        t->parent = p->parent; 
        if (nil_ == p->parent) {
            root_ = t;
        } else if (p == p->parent->left) {
            p->parent->left = t;
        } else {
            p->parent->right = t;
        }

        // put p on t's left
        t->left = p;
        p->parent = t;

        // Add
        t->size = p->size;
        p->size = p->left->size + p->right->size + 1;
    }

    // Symmetric
    void RightRotate(Node *p) {
        Node *t = p->left;

        p->left = t->right;
        if (nil_ != t->right) {
            t->right->parent = p;
        }

        t->parent = p->parent; 
        if (nil_ == p->parent) {
            root_ = t;
        } else if (p == p->parent->left) {
            p->parent->left = t;
        } else {
            p->parent->right = t;
        }

        t->right = p;
        p->parent = t;

        // Add
        t->size = p->size;
        p->size = p->left->size + p->right->size + 1;
    }

    void Transplant(Node *old, Node *fresh) {
        if (nil_ == old->parent) {
            root_ = fresh;
        } else if (old == old->parent->left) {
            old->parent->left = fresh;
        } else {
            old->parent->right = fresh;
        }
        fresh->parent = old->parent;
        fresh->size = old->size;
    }

    void InsertFixUp(Node *p) {
        while (Color::kRed == p->parent->color) {
            if (p->parent == p->parent->parent->left) {
                Node *y = p->parent->parent->right;
                if (Color::kRed == y->color) { 
                    p->parent->color = Color::kBlack;
                    y->color = Color::kBlack;
                    p->parent->parent->color = Color::kRed;
                    p = p->parent->parent;
                } else {
                    if (p == p->parent->right) {
                        p = p->parent;
                        LeftRotate(p);
                    }
                    p->parent->color = Color::kBlack;
                    p->parent->parent->color = Color::kRed;
                    RightRotate(p->parent->parent);
                }
            } else {
                Node *y = p->parent->parent->left;
                if (Color::kRed == y->color) {
                    p->parent->color = Color::kBlack;
                    y->color = Color::kBlack;
                    p->parent->parent->color = Color::kRed;
                    p = p->parent->parent;
                } else {
                    if (p == p->parent->left) {
                        p = p->parent;
                        RightRotate(p);
                    }
                    p->parent->color = Color::kBlack;
                    p->parent->parent->color = Color::kRed;
                    LeftRotate(p->parent->parent);
                }
            }
        }
        root_->color = Color::kBlack;
    }

    void DeleteFixUp(Node *p) {
        while (p != root_ && Color::kBlack == p->color) {
            if (p == p->parent->left) {
                Node* w = p->parent->right;
                if (Color::kRed == w->color) {
                    w->color = Color::kBlack;
                    p->parent->color = Color::kRed;
                    LeftRotate(p->parent);
                    w = p->parent->right;
                }
                if (Color::kBlack == w->left->color && Color::kBlack == w->right->color) {
                    w->color = Color::kRed;
                    p = p->parent;
                } else {
                    if (Color::kBlack == w->right->color) {
                        w->left->color = Color::kBlack;
                        w->color = Color::kRed;
                        RightRotate(w);
                        w = p->parent->right;
                    }
                    w->color = p->parent->color;
                    p->parent->color = Color::kBlack;
                    w->right->color = Color::kBlack;
                    LeftRotate(p->parent);
                    p = root_;
                }
            } else {
                // Symmetric
                Node* w = p->parent->left;
                if (Color::kRed == w->color) {
                    w->color = Color::kBlack;
                    p->parent->color = Color::kRed;
                    RightRotate(p->parent);
                    w = p->parent->left;
                }
                if (Color::kBlack == w->right->color && Color::kBlack == w->left->color) {
                    w->color = Color::kRed;
                    p = p->parent;
                } else {
                    if (Color::kBlack == w->left->color) {
                        w->right->color = Color::kBlack;
                        w->color = Color::kRed;
                        LeftRotate(w);
                        w = p->parent->left;
                    }
                    w->color = p->parent->color;
                    p->parent->color = Color::kBlack;
                    w->left->color = Color::kBlack;
                    RightRotate(p->parent);
                    p = root_;
                }
            }
        }
        root_->color = Color::kBlack;
    }

    Node * const nil_;
    Node *root_;
    Node sentinel_;

    void DebugPrint() {
        DebugPrint(root_, 0);
    }
    
    void DebugPrint(Node *p, int deep) {
        if (nil_ != p) {
            DebugPrint(p->left, deep + 1);
            cout << "val: " << p->x << " deep: " << deep << " size: " << p->size << endl;
            DebugPrint(p->right, deep + 1);
        }
    }
};

int CalcInversion(const vector<int> &A) {
    OSTree os;
    int inv = 0;
    for (size_t i = 0; i != A.size(); ++i) {
        auto *p = os.Insert(A[i]);
        int r = os.Rank(p);
        inv += i - r;
    }
    return inv;
}

int CalcIntersection(vector<pair<int, int>> &A) {
    vector<pair<int, int>> orders;

    for (size_t i = 0; i < A.size(); ++i) {
        if (A[i].first > A[i].second) {
            swap(A[i].first, A[i].second);
        }
        orders.push_back({A[i].first, i});
        orders.push_back({A[i].second, i});
    }

    sort(orders.begin(), orders.end());

    OSTree os;
    int ans = 0;
    for (auto &point : orders) {
        auto chords = A[point.second];
        if (point.first == chords.first) {
            os.Insert(chords.first);
        } else {
            auto *beg = os.Search(chords.first);
            int inc = os.root_->size - os.Rank(beg) - 1;
            os.Delete(beg);

            cout << inc << endl;
            ans += inc;
        }
    }
    return ans;
}

int main() {
    OSTree os;
    os.Insert(10);
    os.Insert(32);
    os.Insert(1);
    os.Insert(22);
    os.Insert(15);
    os.Delete(os.Search(1));
    os.Insert(11);
    os.Insert(3);
    os.Delete(os.Search(22));
    os.Insert(14);
    os.Insert(2);
    os.Insert(45);
    os.Insert(18);
    os.Delete(os.Search(14));
    os.Insert(4);
    os.Insert(9);
    os.Insert(5);
    os.Insert(20);

    os.DebugPrint();
    cout << os.Select(5)->x << endl;
    cout << os.Rank(os.Search(20)) << endl;
    cout << os.KeyRank(15) << endl;

    cout << endl;
    cout << CalcInversion({10, 32, 1, 22, 15, 11}) << endl;

    vector<pair<int, int>> chords {
        {30, 120},
        {60, 210},
        {90, 180},
        {150, 270},
        {240, 330},
        {285, 75},
        {300, 45},
    };
    cout << "Intersection: " << CalcIntersection(chords) << endl;
    return 0; 
}
