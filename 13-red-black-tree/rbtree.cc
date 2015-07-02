#include <iostream>
#include <algorithm>
#include <cassert>
#include <ctime>

using namespace std;

class RBTree {
private:
    enum class Color { kRed, kBlack };

public:
    struct Node {
        int x;
        Node *left, *right, *parent;
        Color color;  
    };

    RBTree() : nil_(&sentinel_), root_(nil_), black_height_(0) {
        sentinel_.parent = sentinel_.left = sentinel_.right = nil_;
        sentinel_.x = 0;
        sentinel_.color = Color::kBlack;
    }

    ~RBTree() {
        Release(root_); 
    }

    Node* Search(int x) {
        Node *p = root_;
        while (p && x != p->x) {
            if (x < p->x) {
                p = p->left;
            } else {
                p = p->right;
            }
        }
        return p;
    }

    Node* Debug_Search(int x) {
        Node *p = root_;
        while (p && x != p->x) {
            cout << p->x;
            if (Color::kRed == p->color) {
                cout << " Red" << endl;
            } else {
                cout << " Black" << endl;
            }
            if (x < p->x) {
                p = p->left;
            } else {
                p = p->right;
            }
        }
        if (p) {
            cout << p->x;
            if (Color::kRed == p->color) {
                cout << " Red" << endl;
            } else {
                cout << " Black" << endl;
            }
        }
        return p;
    }

    void Insert(int x) {
        Node *last = nil_;
        Node *p = root_;
        while (nil_ != p) {
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

        if (nil_ == last) {
            root_ = inserted;
        } else if (inserted->x < last->x) {
            last->left = inserted;
        } else {
            last->right = inserted;
        }

        InsertFixUp(inserted);
    }

    void Delete(Node *p) {
        Node *x = nil_;
        Node *y = p;
        Color y_original_color = y->color;
        if (nil_ == p->left) {
            x = p->right;
            Transplant(p, p->right);
        } else if (nil_ == p->right) {
            x = p->left;
            Transplant(p, p->left);
        } else {
            y = Minimum(p->right);
            x = y->right;

            if (y->parent == p) {
                x->parent = y;
            } else {
                Transplant(y, x);
                y->right = p->right;
                y->right->parent = y;
            }
            Transplant(p, y);
            y->left = p->left;
            y->left->parent = y;
            y->color = p->color;
        }
        if (Color::kBlack == y_original_color) {
            DeleteFixUp(x);
        }
        delete p;
    }

    int black_height() {
        return black_height_;
    }

    /*
    Node *FindMaxSubTree(int black_height) {
        int current_bh = black_height_;
        Node *p = root_;
        while (current_bh > black_height) {
            p = p->right;
            if (Color::kBlack == p->color) {
                --current_bh;
            }
        }
        return p;
    }

    Node *FindMinSubTree(int black_height) {
        int current_bh = black_height_;
        Node *p = root_;
        while (current_bh > black_height) {
            p = p->left;
            if (Color::kBlack == p->color) {
                --current_bh;
            }
        }
        return p;
    }

    void Join(RBTree &tree, int x) {
        assert(black_height_ > tree.black_height_);
        Node *sub = FindMaxSubTree(tree.black_height_); 
        if (sub) {
            Node *joint = new Node;
            joint->x = x;
            joint->color = Color::kRed;

            sub->parent->right = joint;
            joint->parent = sub->parent;

            joint->left = sub;
            sub->parent = joint;

            joint->right = tree.root_;
            tree.root_->parent = joint;
            
            tree.root_ = tree.nil_;
            tree.black_height_ = 0;

            InsertFixUp(joint);
        }
    }
    */

    void Print() {
        Print(root_, 0);
    }

private:
    void Print(Node *p, int deep) {
        if (nil_ != p) {
            Print(p->left, deep + 1);
            cout << p->x << ' ' << deep;
            if (Color::kRed == p->color) {
                cout << " Red" << endl;
            } else {
                cout << " Black" << endl;
            }
            Print(p->right, deep + 1);
        }
    }

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
        if (Color::kRed == root_->color) {
            root_->color = Color::kBlack;
            ++black_height_;
        }
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
                    ++black_height_;
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
                    ++black_height_;
                }
            }
        }
        root_->color = Color::kBlack;
        --black_height_;
    }

    Node * const nil_;
    Node *root_;
    Node sentinel_;
    int black_height_;
};

class AVLTree {
public:
    struct Node {
        Node *left, *right, *parent;
        int x; 
        int height;
    };

    AVLTree() : root_(0) {}

    ~AVLTree() {
        Release(root_);
    }

    Node* Search(int x) {
        Node *p = root_;
        while (p && x != p->x) {
            if (x < p->x) {
                p = p->left;
            } else {
                p = p->right;
            }
        }
        return p;
    }

    void Insert(int x) {
        Node *last = 0;
        Node *p = root_;

        while (p) {
            last = p;
            if (x < p->x) {
                p = p->left;
            } else {
                p = p->right;
            }
        }

        Node *inserted = new Node;
        inserted->x = x;
        inserted->parent = last;
        inserted->left = 0;
        inserted->right = 0;
        inserted->height = 0;

        if (!last) {
            root_ = inserted;
        } else if (inserted->x < last->x) {
            last->left = inserted;
        } else {
            last->right = inserted;
        }
        Balance(inserted);
    }

    void Delete(Node *p) {
        Node *x = 0;
        if (!p->left) {
            x = p->right;
            Transplant(p, p->right);
        } else if (!p->right) {
            x = p->left;
            Transplant(p, p->left);
        } else {
            Node *next = Minimum(p->right);
            x = next;
            if (next->parent != p) {
                Transplant(next, next->right);
                next->right = p->right;
                next->right->parent = next;
            }
            Transplant(p, next);
            next->left = p->left;
            next->left->parent = next;
        }

        delete p;
        
        if (x) {
            Balance(x);
        }
    }

    void Print() {
        Print(root_);
    }

private:
    void Print(Node *p) {
        if (p) {
            Print(p->left);
            cout << p-> x << ' ' << p->height << endl;
            Print(p->right);
        }
    }

    Node *root_; 
    
    void Release(Node *p) {
        if (p) {
            Release(p->left);
            Release(p->right);
            delete p;
        }
    }

    Node *Minimum(Node *p) {
        while (p->left) {
            p = p->left;
        }
        return p;
    }

    int Height(Node *p) {
        return p ? p->height : -1;
    }

    void UpdateLeft(Node *p) {
        if (!p) return;
    }

    void LeftRotate(Node *p) {
        Node *t = p->right;

        // Turn t's left subtree into p's right subtree
        p->right = t->left;
        if (t->left) {
            t->left->parent = p;
        }

        // Link p's parent to t
        t->parent = p->parent; 
        if (!p->parent) {
            root_ = t;
        } else if (p == p->parent->left) {
            p->parent->left = t;
        } else {
            p->parent->right = t;
        }

        // put p on t's left
        t->left = p;
        p->parent = t;

        p->height = max(Height(p->left), Height(p->right)) + 1;
        t->height = max(p->height, Height(t->right)) + 1;
    }

    // Symmetric
    void RightRotate(Node *p) {
        Node *t = p->left;

        p->left = t->right;
        if (t->right) {
            t->right->parent = p;
        }

        t->parent = p->parent; 
        if (!p->parent) {
            root_ = t;
        } else if (p == p->parent->left) {
            p->parent->left = t;
        } else {
            p->parent->right = t;
        }

        t->right = p;
        p->parent = t;

        p->height = max(Height(p->left), Height(p->right)) + 1;
        t->height = max(Height(t->left), p->height) + 1;
    }

    void Balance(Node *p) {
        int x = p->x;
        p = p->parent;
        while (p) {
            p->height = max(Height(p->left), Height(p->right)) + 1;
            if (2 == Height(p->left) - Height(p->right)) {
                if (x < p->left->x) {
                    RightRotate(p);
                } else {
                    LeftRotate(p->left);
                    RightRotate(p);
                }
                p = p->parent->parent;
            } else if (-2 == Height(p->left) - Height(p->right)) {
                if (x > p->right->x) {
                    LeftRotate(p);
                } else {
                    RightRotate(p->right);
                    LeftRotate(p);
                }
                p = p->parent->parent;
            } else {
                p = p->parent;
            }
        }
    }

    void Transplant(Node *old, Node *fresh) {
        if (!old->parent) {
            root_ = fresh;
        } else if (old == old->parent->left) {
            old->parent->left = fresh;
        } else {
            old->parent->right = fresh;
        }
        if (fresh) {
            fresh->parent = old->parent;
        }
    }

};

class Treap {
public:
    struct Node {
        int x, priority;
        Node *left, *right, *parent;
    };

    Treap() : root_(0) {
        srand(time(0));
    }

    ~Treap() {
        Release(root_);
    }

    void Insert(int x) {
        Node *last = 0;
        Node *p = root_;

        while (p) {
            last = p;
            if (x < p->x) {
                p = p->left;
            } else {
                p = p->right;
            }
        }

        Node *inserted = new Node;
        inserted->x = x;
        inserted->parent = last;
        inserted->left = 0;
        inserted->right = 0;
        inserted->priority = rand();

        if (!last) {
            root_ = inserted;
        } else if (inserted->x < last->x) {
            last->left = inserted;
        } else {
            last->right = inserted;
        }

        Balance(inserted);
    }

//private:
    Node *root_;

    void Release(Node *p) {
        if (p) {
            Release(p->left);
            Release(p->right);
            delete p;
        }
    }

    void LeftRotate(Node *p) {
        Node *t = p->right;

        p->right = t->left;
        if (t->left) {
            t->left->parent = p;
        }

        t->parent = p->parent; 
        if (!p->parent) {
            root_ = t;
        } else if (p == p->parent->left) {
            p->parent->left = t;
        } else {
            p->parent->right = t;
        }

        t->left = p;
        p->parent = t;
    }

    // Symmetric
    void RightRotate(Node *p) {
        Node *t = p->left;

        p->left = t->right;
        if (t->right) {
            t->right->parent = p;
        }

        t->parent = p->parent; 
        if (!p->parent) {
            root_ = t;
        } else if (p == p->parent->left) {
            p->parent->left = t;
        } else {
            p->parent->right = t;
        }

        t->right = p;
        p->parent = t;
    }

    void Balance(Node *p) {
        while (p->parent && p->priority < p->parent->priority) {
            if (p->x < p->parent->x) {
                RightRotate(p->parent);
            } else {
                LeftRotate(p->parent);
            }
        }
    }
};

/*
template<class T>
T* PersistentInsert(T *root, int x) {
    if (!root) {
        T *inserted = new T;
        inserted->x = x;
        inserted->left = 0;
        inserted->right = 0;
        return inserted;
    } else {
        T *new_root = new T;
        new_root->x = root->x;
        new_root->left = root->left;
        new_root->right = root->right;

        if (x < root->x) {
            new_root->left = PersistentInsert(root->left, x);
        } else {
            new_root->right = PersistentInsert(root->right, x);
        }
        return new_root;
    }
}
*/

template<class T>
void PrintTree(T *node, int deep) {
    if (node) {
        PrintTree(node->left, deep+1);
        cout << node->x << ' ' << deep << endl;
        PrintTree(node->right, deep+1);
    }
}

template<class T>
void DebugPrint(T *node) {
    if (0 != node->x) {
        DebugPrint(node->left);
        cout << node->x << endl;
        DebugPrint(node->right);
    }
}


int main() {
    
    RBTree tree;
    tree.Insert(1);
    tree.Insert(2);
    tree.Insert(3);
    tree.Insert(4);
    tree.Insert(5);
    tree.Insert(6);
    tree.Insert(7);
    tree.Insert(8);
    tree.Insert(9);
    tree.Insert(10);
    tree.Insert(11);
    tree.Insert(12);
 
    /*
    tree.Insert(10);
    tree.Insert(32);
    tree.Insert(1);
    tree.Insert(22);
    tree.Insert(15);
//    tree.Delete(tree.Search(1));
    tree.Insert(8);
    tree.Insert(11);
    tree.Insert(3);
    //tree.Delete(tree.Search(22));
    tree.Insert(14);
    tree.Insert(2);
    tree.Insert(45);
    tree.Insert(18);
    //tree.Delete(tree.Search(14));
    tree.Insert(4);
    tree.Insert(9);
    tree.Insert(5);
    tree.Insert(20);
    */

    RBTree to_join;
    to_join.Insert(111);
    to_join.Insert(100);
    to_join.Insert(120);

    tree.Print();
    cout << tree.black_height() << endl;
    to_join.Print();
    cout << to_join.black_height() << endl;

    //tree.Join(to_join, 99);
/*
    AVLTree avl;
    avl.Insert(10);
    avl.Insert(32);
    avl.Insert(1);
    avl.Insert(22);
    avl.Insert(15);
    avl.Delete(avl.Search(1));
    avl.Insert(8);
    avl.Insert(11);
    avl.Insert(3);
    avl.Delete(avl.Search(22));
    avl.Insert(14);
    avl.Insert(2);
    avl.Insert(45);
    avl.Insert(18);
    avl.Delete(avl.Search(14));

    avl.Print();
*/

    /*
    auto avl_root = avl.Search(11);
    auto new_root = PersistentInsert(avl_root, 49);
    cout << endl;
    PrintTree(avl_root);
    cout << endl;
    PrintTree(new_root);
    cout << endl;
    */

    Treap treap;
    treap.Insert(1);
    treap.Insert(2);
    treap.Insert(3);
    treap.Insert(4);
    treap.Insert(5);
    treap.Insert(6);
    treap.Insert(7);
    treap.Insert(8);
    treap.Insert(9);
    treap.Insert(10);
    treap.Insert(11);
    treap.Insert(12);
    PrintTree(treap.root_, 0);

    return 0;
}

