#include <iostream>
#include <algorithm>
#include <list>
#include <cassert>
#include <iterator>

using namespace std;

class BTree {
public:
    static const int t = 2;

    struct Node {
        list<int> keys;
        list<Node*> children;
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
    typename T::iterator list_at(T &l, int index) {
        int i = 0;
        auto iter = l.begin();
        while (iter != l.end() && i < index) {
            ++i; ++iter;
        }
        return iter;
    }

    pair<Node*, int> Search(int key) {
        return Search(root_, key);
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

    void Delete(int key) {
        Delete(root_, key);
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
//        PrintSpace(deep);
//        cout << "size: " << x->n << endl;
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

    pair<Node*, int> Search(Node *x, int key) {
        auto iter_key = x->keys.begin();
        auto iter_child = x->children.begin();

        while (iter_key != x->keys.end() && key > *iter_key) {
            ++iter_key;
            ++iter_child;
        }

        if (iter_key != x->keys.end() && key == *iter_key) {
            return {x, key};
        } else if (x->leaf) {
            return {0, 0};
        } else {
            return Search(*iter_child, key);
        }
    }

    void SplitChild(Node *x, int index) {
        auto iter_child = list_at(x->children, index);
        Node *y = *iter_child;

        Node *z = new Node;
        z->leaf = y->leaf;
        z->n = t - 1;

        auto split_key = list_at(y->keys, t-1);
        z->keys.splice(z->keys.begin(), y->keys, next(split_key), y->keys.cend());

        if (!y->leaf) {
            auto right_child = list_at(y->children, t);
            z->children.splice(z->children.begin(), y->children, right_child, y->children.end());
        }
        y->n = t - 1; 

        x->keys.splice(list_at(x->keys, index), y->keys, split_key);
        x->children.insert(next(iter_child), z);
        x->n += 1;
    }
 
    void InsertNotFull(Node *x, int key) {
        if (x->leaf) {
            auto iter = x->keys.begin();
            while (iter != x->keys.end() && key > *iter) {
                ++iter;
            }
            x->keys.insert(iter, key); 
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

    void MergeKeys(list<Node*>::iterator c, list<int> &keys, list<int>::iterator it) {
        auto lc = *c;
        auto rc = *next(c);
        lc->keys.splice(lc->keys.end(), keys, it);
        lc->keys.splice(lc->keys.end(), rc->keys, rc->keys.begin(), rc->keys.end());
        lc->n = 2*t - 1;
    }

    void MergeChildren(list<Node*>::iterator c, list<Node*> &children) {
        auto lc = *c;
        auto rc = *next(c);
        lc->children.splice(lc->children.end(), rc->children, rc->children.begin(), rc->children.end());
        children.erase(next(c));
    }

    void Delete(Node *x, int key) {
        if (x->leaf) {
            x->keys.remove(key);
            x->n -= 1;

        } else {
            auto iter_key = x->keys.begin();
            auto iter_child = x->children.begin();
            while (iter_key != x->keys.end() && key > *iter_key) {
                ++iter_key;
                ++iter_child;
            }

            if (iter_key != x->keys.end() && key == *iter_key) {
                Node *lc = *iter_child;
                Node *rc = *next(iter_child);

                if (lc->n >= t) {
                    *iter_key = lc->keys.back();
                    Delete(lc, key);

                }  else if (rc->n >= t) {
                    *iter_key = rc->keys.front();
                    Delete(rc, key);

                } else {
                    x->n -= 1;
                    MergeKeys(iter_child, x->keys, iter_key);
                    MergeChildren(iter_child, x->children);
                    Delete(lc, key);
                }
                return;
            }

            if ((*iter_child)->n < t) {
                if (next(iter_child) != x->children.end() && (*next(iter_child))->n >= t) {
                    Node *lc = *iter_child;
                    Node *rc = *next(iter_child);

                    lc->keys.push_back(*iter_key);
                    *iter_key = rc->keys.front();
                    rc->keys.pop_front();
                    lc->n += 1;
                    rc->n -= 1;

                    lc->children.splice(lc->children.end(), rc->children, rc->children.begin());

                    Delete(lc, key);

                } else if (iter_child != x->children.begin() && (*prev(iter_child))->n >= t) {
                    Node *lc = *prev(iter_child);
                    Node *rc = *iter_child;

                    rc->keys.push_front(*iter_key);
                    *iter_key = lc->keys.back();
                    lc->keys.pop_back();
                    rc->n += 1;
                    lc->n -= 1;

                    rc->children.splice(rc->children.begin(), lc->children, lc->children.rbegin().base());

                    Delete(rc, key);

                } else {
                    auto merge_iter = iter_child == x->children.begin() ? iter_child : prev(iter_child);
                    auto merge_key = iter_child == x->children.begin() ? iter_key : prev(iter_key);
                    x->n -= 1;
                    MergeKeys(merge_iter, x->keys, merge_key);
                    MergeChildren(merge_iter, x->children);
                    Delete(*merge_iter, key);
                }
            } else {
                Delete(*iter_child, key);
            }

        }
    }

private:
    Node *root_;
};

int main() {

    BTree bt;
    for (int i = 1; i <= 15; ++i) {
        bt.Insert(i);
    }
    bt.Print();
    bt.Delete(13);
    bt.Print();

    return 0;
}

