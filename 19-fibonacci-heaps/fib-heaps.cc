#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>

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

    int min() {
        return min_->key;
    }

    void Insert(int key) {
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
    }

    void Print() {
        PrintNode(head_, 0);
        cout << endl;
    }

    int ExtractMin() {
        assert(min_);

        int min_key = min_->key;
        for (Node *p = min_->child; p; p = p->next) {
            ListInsert(head_, p);
            if (min_->child == p->next) break;
        }
        
        if (min_ == min_->next) {
            delete min_;
            min_ = 0;
            head_ = 0;
        } else {
            ListDelete(min_);
            if (head_ == min_) {
                head_ = min_->next;
            }
            delete min_;
            min_ = min_->next;
            Consolidate();
        }
        
        --size_;
        return min_key;
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
    }

    static void Link(Node *p, Node *c) {
        ListDelete(c);
        if (p->child) {
            ListInsert(p->child, c);
            c->parent = p;
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
        bool jump = false;
        while (p) {
            Node *x = p;

            if (x == end) {
                jump = true;
            }

            int d = x->degree;
            while (A[d]) {
                Node *y = A[d];
                if (x->key > y->key) {
                    swap(x, y);
                }
                Link(x, y);
                A[d] = nullptr;
                ++d;
            }
            A[d] = x;

            for (int i = 0; i < D; ++i) {
                if (A[i]) {
                    PrintNode(A[i], 0);
                } else {
                    cout << "index :" << i <<  A[i] << endl;
                }
            }
            cout << endl;

            getchar();

            if (jump) break;
            p = x->next;
        }

        cout << "before build root list" << endl;
        min_ = nullptr;
        head_ = nullptr;
        for (int i = 0; i < D; ++i) if (A[i]) {
            if (!min_) {
                min_ = A[i];
                head_ = A[i];
                head_->next = head_->prev = head_;
            } else {
                ListInsert(head_, A[i]);
                if (A[i]->key < min_->key) {
                    min_ = A[i];
                }
            }
            Print();
            getchar();
        }
    }

    Node *min_; 
    Node *head_;
    int size_;
};

int main() {
    FibonacciHeap h;
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

    return 0;
}
