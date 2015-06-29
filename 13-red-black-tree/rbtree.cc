#include <iostream>
#include <algorithm>

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

    RBTree() : nil_(&sentinel_), root_(nil_) {
        sentinel_.parent = sentinel_.left = sentinel_.right = nil_;
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

    // TODO
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

    void Print() {
        Print(root_);
    }

private:
    void Print(Node *p) {
        if (nil_ != p) {
            Print(p->left);
            cout << p->x << endl;
            Print(p->right);
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
                if (Color::kBlack == y->color) {
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
};

int main() {
    RBTree tree;
    tree.Insert(10);
    tree.Insert(32);
    tree.Insert(1);
    tree.Insert(22);
    tree.Insert(15);
    tree.Delete(tree.Search(1));
    tree.Insert(8);
    tree.Insert(11);
    tree.Insert(3);
    tree.Delete(tree.Search(22));
    tree.Insert(14);
    tree.Insert(2);
    tree.Insert(45);
    tree.Insert(18);
    tree.Delete(tree.Search(14));
 
    tree.Print();
    return 0;
}

