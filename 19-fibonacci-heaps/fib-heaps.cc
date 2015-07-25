#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <climits>
#include <list>

using namespace std;

class FibonacciHeap {
public:
    struct Node {
        Node    *prev {0},
                *next {0},
                *parent {0},
                *child {0};
        int     key {0};
        int     degree {0};
        bool    mask {false};
    };

    FibonacciHeap() : min_(0), head_(0), size_(0) {}

    ~FibonacciHeap() {
        for (Node *p = head_; p; p = p->next) {
            Release(p);
            if (p->next == head_) break;
        }
    }

    int min() {
        return min_->key;
    }

    Node* Insert(int key) {
        Node *x = new Node;
        x->key = key;

        if (!min_) {
            x->next = x->prev = x;
            min_ = x;
            head_ = x;

        } else {
            ListInsert(head_, x);
            if (key < min_->key) {
                min_ = x;
            }
        }

        ++size_;
        return x;
    }

    void Union(FibonacciHeap &H) {
        Node *last = H.head_->prev;
        last->next = head_->next;
        H.head_->prev = head_;

        head_->next->prev = last;
        head_->next = H.head_;
        
        if (H.min_->key < min_->key) {
            min_ = H.min_;
        }
        size_ += H.size_;

        H.head_ = 0;
    }

    void Print() {
        PrintNode(head_, 0);
        cout << endl;
    }

    int ExtractMin() {
        assert(min_);

        int min_key = min_->key;

        if (min_->child) {
            Node *last = min_->child->prev;
            last->next = head_->next;
            min_->child->prev = head_;

            head_->next->prev = last;
            head_->next = min_->child;

            min_->child = 0;
        }

        if (min_ == min_->next) {
            delete min_;
            min_ = 0;
            head_ = 0;
        } else {
            if (head_ == min_) {
                head_ = min_->next;
            }
            ListDelete(min_);
            delete min_;

            min_ = head_;
            Consolidate();
        }
       
        --size_;
        return min_key;
    }

    void DecreaseKey(Node *x , int key) {
        assert(x->key > key);

        x->key = key;
        Node *p = x->parent;
        if (p && x->key < p->key) {
            // TODO
            Cut(x);
            CascadingCat(p);
        }
        if (x->key < min_->key) {
            min_ = x;
        }
    }

    void Delete(Node *x) {
        DecreaseKey(x, INT_MIN);
        ExtractMin();
    }

private:
    void PrintSpace(int deep) {
        while (deep--) putchar(' ');
    }
    void PrintNode(Node *first, int deep) {
        for (Node *p = first; p; p = p->next) {
            PrintSpace(deep);
            cout << p->key << " " << p->prev->key << " " << p->next->key << endl;
            PrintNode(p->child, deep+1);
            if (p->next == first) break;
        }
    }

    void Release(Node *r) {
        for (Node *p = r->child; p; p = p->next) {
            Release(p);
            if (p == r->child) break;
        }
        delete r;
    }

    static void ListInsert(Node *l, Node *x) {
        x->next = l->next;
        x->prev = l;
        x->parent = l->parent;

        l->next->prev = x;
        l->next = x;
    }

    static void ListDelete(Node *p) {
        p->prev->next = p->next;
        p->next->prev = p->prev;
        p->parent = nullptr;
    }

    static void Link(Node *p, Node *c) {
        ListDelete(c);
        if (p->child) {
            ListInsert(p->child, c);
        } else {
            p->child = c;
            c->parent = p;
            c->next = c->prev = c;
        }
        c->mask = false;
        ++(p->degree);
    }

    void Consolidate() {
        int D = log(size_) / log(1.618) + 1;
        Node* A[D] {0};
        memset(A, 0, sizeof(A));
    
        Node *p = head_;
        Node *end = head_->prev;
        while (p) {
            Node *x = p;

            int d = x->degree;
            while (A[d]) {
                Node *y = A[d];
                if (x->key > y->key) {
                    swap(x, y);
                }
                Link(x, y);
                A[d++] = nullptr;
            }
            A[d] = x;

            /*
            for (int i = 0; i < D; ++i) {
                if (A[i]) {
                    PrintNode(A[i], 0);
                } else {
                    cout << "index :" << i <<  A[i] << endl;
                }
            }
            cout << endl;

            //getchar();
            */

            if (p == end) break;
            p = x->next;
        }

        //cout << "before build root list" << endl;
        min_ = nullptr;
        head_ = nullptr;
        for (int i = 0; i < D; ++i) if (A[i]) {
            if (!min_) {
                min_ = A[i];
                head_ = A[i];
                //head_->next = head_->prev = head_;
            } else {
                //ListInsert(head_, A[i]);
                if (A[i]->key < min_->key) {
                    min_ = A[i];
                }
            }
            //Print();
            //getchar();
        }
    }

    void Cut(Node *x) {
        if (x->next == x) {
            x->parent->child = nullptr;
        } else {
            if (x->parent->child == x) {
                x->parent->child = x->next;
            }
            ListDelete(x);
        }
        ListInsert(head_, x);
        x->mask = false;
    }

    void CascadingCat(Node *x) {
        Node *p = x->parent;
        if (p) {
            if (!x->mask) {
                x->mask = true;
            } else {
                Cut(x);
                CascadingCat(p);
            }
        }
    }

    Node *min_; 
    Node *head_;
    int size_;
};

class BinomialHeap {
public:
    struct Node {
        Node    *left {0},
                *right {0},
                *parent {0};
        int     key {0};
        int     degree {0};
    };

    BinomialHeap() : roots_(0) {}

    ~BinomialHeap() {
        for (auto p : roots_) {
            Release(p);
        }
    }

    int Min() {
        return min_->key;
    }

    Node* Insert(int key) {
        Node *p = new Node;
        p->key = key;

        Node *ret = p;

        if (!min_ || min_->key < key) {
            min_ = p;
        }

        if (roots_.empty() || roots_.front()->degree > 0) {
            roots_.push_front(p);
        } else {
            for (auto &root : roots_) {
                if (root->degree == p->degree) {
                    Link(root, p);
                    p = root;
                } else {
                    break;
                }
            }

            while (roots_.front()->degree < p->degree) {
                roots_.pop_front();
            }
        }

        return ret;
    }

    void Union(BinomialHeap &H) {
        auto l_iter = roots_.begin();
        auto r_iter = H.roots_.begin();

        while (r_iter != H.roots_.end()) {
            auto &l_root = *l_iter;
            auto &r_root = *r_iter;
            if (l_root->degree < r_root->degree) {
                ++l_iter;
            } else if (l_root->degree > r_root->degree) {
                auto temp = r_iter;
                ++r_iter;
                roots_.splice(l_iter, H.roots_, temp);
            } else {
                Link(l_root, r_root); 
                ++r_iter;
            }

            if (next(l_iter) != roots_.end() && (*next(l_iter))->degree == l_root->degree) {
                ++l_iter;
                Link(*l_iter, *prev(l_iter));
                roots_.erase(prev(l_iter));
            }
        }

        for (auto p : roots_) if (p->key < min_->key) {
            min_ = p;
        }
        H.roots_.clear();
    }

    int ExtractMin() {
        auto iter = roots_.begin();
        while (iter != roots_.end() && *iter != min_) {
            ++iter;
        }

        auto min_iter = iter;
        Node *split = *min_iter;

        --iter;
        while (split->left) {
            Node *remain = split->left;
            split->left = split->left->right;

            if (iter != roots_.end() && (*iter)->degree == remain->degree) {
                Link(*iter, remain);

                --iter;
            } else {
                remain->right = nullptr;
                remain->parent = nullptr;
                roots_.insert(next(iter), remain);
            }
        }

        roots_.erase(min_iter);
                
        if (roots_.empty()) {
            min_ = nullptr;
        } else {
            min_ = roots_.front();
            for (auto p : roots_) if (p->key < min_->key) {
                min_ = p;
            }
        }

        int ret = split->key;
        delete split;
        return ret;
    }

    void DecreaseKey(Node *x, int key) {
        assert(x->key > key);

        x->key = key;
        while (x->parent && x->key < x->parent->key) {
            swap(x->key, x->parent->key);
            x = x->parent;
        }

        if (x->key < min_->key) {
            min_ = x;
        }
    }

    void Delete(Node *x) {
        DecreaseKey(x, INT_MIN);
        ExtractMin();
    }

    void Print() {
        for (auto p : roots_) {
            //cout << "r : ";
            PrintNode(p, 0);
        }
        cout << endl;
    }

private:    
    void PrintNode(Node *p, int deep) {
        if (p->right) PrintNode(p->right, deep);
        for (int i = 0; i < deep; ++i) putchar(' ');
        if (p->parent) {
            cout << p->key << ' ' << p->parent->key << endl;
        } else {
            cout << p->key << endl;
        }

        if (p->left) {
            PrintNode(p->left, deep+1);
        }
    }

    void Release(Node *p) {
        if (p) {
            Release(p->left);
            Release(p->right);
            delete p;
        }
    }

    void Link(Node *&p, Node *&c) {
        if (p->key > c->key) {
            swap(p, c);
        }
        c->right = p->left;
        p->left = c;
        c->parent = p;

        p->right = nullptr;
        p->degree += 1;
    }

    list<Node*> roots_;
    Node *min_ {0};
};

int main() {
/*    
    FibonacciHeap h;
    auto p = h.Insert(20);
    for (int i = 1; i <= 40; i += 2) {
        h.Insert(i); 
    }
    h.Print();
    h.ExtractMin();
    h.Print();

    for (int i = 2; i <= 12; i += 2) {
        h.Insert(i);
    }
    h.ExtractMin();
    h.Print();

    for (int i = 0; i < 10; ++i) {
        h.ExtractMin();
    }
    h.Print();
    h.Delete(p);
    h.Print();
*/    

    BinomialHeap h;
    for (int i = 0; i < 20; i += 2) {
        h.Insert(i);
    }
    BinomialHeap h2;
    for (int i = 1; i < 11; i += 2) {
        h2.Insert(i); 
    }
    auto x = h.Insert(15);
    h.Print();
    h2.Print();
    h.Union(h2);
    h.Print();
    for (int i = 0; i < 10; ++i) {
        cout << "ExtractMin: " << h.ExtractMin() << endl;
        h.Print();
    }
    h.Delete(x);
    h.Print();

    return 0;
}
