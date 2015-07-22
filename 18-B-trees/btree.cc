#include <iostream>
#include <algorithm>
#include <forward_list>

using namespace std;

class BTree {
public:
    static const int t = 2;

    struct Node {
        forward_list<int> keys;
        forward_list<Node*> children;
        bool leaf;
        int n;
    };

    BTree() {
        root_ = new Node;
        root_->leaf = true;
        root_->n = 0;
    }

    ~BTree() {
        Release(root_);
    }

    template<class T>
    typename T::iterator flist_at(T &l, int index) {
        int i = -1;
        auto iter = l.before_begin();
        while (iter != l.end() && i < index) {
            ++i; ++iter;
        }
        return iter;
    }

    int *Search(Node *x, int key) {
        auto iter_key = x->keys.begin();
        auto iter_child = x->children.begin();

        while (iter_key != x->keys.end() && key > *iter_key) {
            ++iter_key;
            ++iter_child;
        }

        if (iter_key != x->keys.end() && key == *iter_key) {
            return &*iter_key;
        } else if (x->leaf) {
            return 0;
        } else {
            return Search(*iter_child, key);
        }
    }

    void Insert(int key) {
        Node *r = root_;
        if (2*t - 1 == r->n) {
            root_ = new Node;
            root_->leaf = false;
            root_->n = 0;
            root_->children.push_front(r);
            SplitChild(root_, 0);
            InsertNotFull(root_, key);
        } else {
            InsertNotFull(r, key);
        }
    }

    void Print() {
        Print(root_, 0);        
    }

private:
    void PrintSpace(int n) {
        while (n--) {
            cout << "  ";
        }
    }

    void Print(Node *x, int deep) {
        auto iter_key = x->keys.begin();
        auto iter_child = x->children.begin();

        while (iter_key != x->keys.end()) {
            if (iter_child != x->children.end()) {
                Print(*iter_child, deep+1);
                ++iter_child;
            }

            PrintSpace(deep);
            cout << *iter_key << endl;
            ++iter_key;
        }

        if (iter_child != x->children.end()) {
            Print(*iter_child, deep+1);
        }
    }

    void Release(Node *x) {
        for (auto child : x->children) {
            Release(child);
        }
        delete(x);
    }

    void SplitChild(Node *x, int index) {
        Node *z = new Node;
        
        auto iter_key = flist_at(x->keys, index-1);
        auto iter_child = flist_at(x->children, index);

        Node *y = *iter_child;

        z->leaf = y->leaf;
        z->n = t - 1;

        auto before_split = flist_at(y->keys, t-2);
        auto iter_c_key = before_split;
        ++iter_c_key;
        z->keys.splice_after(z->keys.before_begin(), y->keys, iter_c_key, y->keys.end());

        if (!y->leaf) {
            auto iter_c_child = flist_at(y->children, t-1);
            z->children.splice_after(z->children.before_begin(), y->children, iter_c_child, y->children.end());
        }
        y->n = t - 1; 

        x->keys.splice_after(iter_key, y->keys, before_split);
        x->children.insert_after(iter_child, z);

        x->n += 1;
   }

    void InsertNotFull(Node *x, int key) {
        if (x->leaf) {
            auto before = x->keys.before_begin();
            auto iter = x->keys.begin();
            while (iter != x->keys.end() && key > *iter) {
                ++before;
                ++iter;
            }
            x->keys.insert_after(before, key); 
            x->n += 1;

        } else {
            int index = 0;
            auto iter_key = x->keys.begin();
            auto iter_child = x->children.begin();
            while (index < x->n-1 && key > *iter_key) {
                ++index;
                ++iter_key;
                ++iter_child;
            }
            if (key > *iter_key) {
                ++index;
                ++iter_child;
            }

            if ( 2*t - 1 == (*iter_child)->n ) {
                SplitChild(x, index);

                if (key > *++iter_key) {
                    ++iter_child;
                }
            }

            InsertNotFull(*iter_child, key);
        }
    }
    

private:
    Node *root_;
};

int main() {

    BTree bt;
    for (int i = 1; i <= 300; ++i) {
        bt.Insert(i);
    }
    bt.Print();

    return 0;
}
