#include <iostream>
#include <algorithm>
#include <stack>

using namespace std;

/******************************************************************************/

class BinaryTree {
  public:
    struct Node {
      Node *parent, *left, *right;
      int x;
    };

    BinaryTree() : root_(0) {}

    ~BinaryTree() {
      Release(root_);
    }

    void Insert(int x) {
      Node *last(nullptr);
      Node *p(root_);

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
      inserted->left = nullptr;
      inserted->right = nullptr;

      if (!last) {
        root_ = inserted;
      } else if (inserted->x < last->x) {
        last->left = inserted;
      } else {
        last->right = inserted;
      }
    }

    void InsertX(int x) {
      Insert(root_, x);
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

    Node *Minimum() {
      return Minimum(root_);
    }

    Node *Maximum() {
      return Maximum(root_);
    }

    static Node *Predecessor(Node *p) {
      if (p->left) {
        return Maximum(p->left);
      }
      Node *prior = p->parent;
      while (prior && p == prior->left) {
        p = prior;
        prior = prior->parent;
      }
      return prior;
    }

    static Node *Successor(Node *p) {
      if (p->right) {
        return Minimum(p->right);
      }
      Node *prior = p->parent;
      while (prior && p == prior->right) {
        p = prior;
        prior = prior->parent;
      }
      return prior;
    }

    void Delete(Node *p) {
      if (!p->left) {
        Transplant(p, p->right);
      } else if (!p->right) {
        Transplant(p, p->left);
      } else {
        Node *next = Minimum(p->right);
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
    }

    void InorderWalk() {
        stack<pair<Node*, int>> s;
        enum { kLeft, kMiddle };

        s.push( {root_, kLeft} );

        while (!s.empty()) {
            auto state = s.top();
            s.pop();
            if (!state.first) continue;

            if (kLeft == state.second) {
                s.push( {state.first, kMiddle} );
                s.push( {state.first->left, kLeft} );

            } else if (kMiddle == state.second) {
                cout << state.first->x << endl;

                s.push( {state.first->right, kLeft} );
            }
        }
    }

    void PreorderWalk() {
        stack<Node*> s;
        s.push( root_ );

        while (!s.empty()) {
            Node *p = s.top();
            s.pop();

            if (p) {
                cout << p->x << endl;
                s.push( p->right );
                s.push( p->left);
            }
        }
    }

    void PostorderWalk() {
        stack<pair<Node*, int>> s;
        enum { kPush, kPrint };

        s.push( {root_, kPush} );

        while (!s.empty()) {
            auto state = s.top();
            s.pop();
            if (!state.first) continue;

            if (kPush == state.second) {
                s.push( {state.first, kPrint} );
                s.push( {state.first->right, kPush} );
                s.push( {state.first->left, kPush} );

            } else if (kPrint == state.second) {
                cout << state.first->x << endl;
            }
        }
    }

private:
    void Release(Node *p) {
         if (p) {
            Release(p->left);
            Release(p->right);
            delete p;
        }
    }
    
    static Node* Minimum(Node *p) {
        while (p->left) {
            p = p->left;
        }
        return p;
    }

    static Node *Maximum(Node *p) {
        while (p->right) {
            p = p->right;
        }
        return p;
    }

    void Transplant(Node *old_node, Node *new_node) {
        if (!old_node->parent) {
            root_ = new_node;
        } else if (old_node == old_node->parent->left) {
            old_node->parent->left = new_node;
        } else {
            old_node->parent->right = new_node;
        }
        if (new_node) {
            new_node->parent = old_node->parent;
        }
    }

    void Insert(Node *node, int x) {
        Node *next = nullptr;
        if (!node) {
            next = nullptr;
        } else if (x < node->x) {
            next = node->left;
        } else {
            next = node->right;
        }
        if (next) {
            Insert(next, x);
        } else {
            Node *inserted = new Node;
            inserted->x = x;
            inserted->left = nullptr;
            inserted->right = nullptr;
            inserted->parent = node;

            if (node) {
                if (inserted->x < node->x) {
                    node->left = inserted;
                } else {
                    node->right = inserted;
                }
            }
            if (!root_) {
                root_ = inserted;
            }
        }
    }

    Node *root_;
};

class BinaryTreeX {
public:
    struct Node {
        Node *succ, *left, *right;
        int x;
    };

    BinaryTreeX() : root_(0) {}

    ~BinaryTreeX() {
        Release(root_);
    }

    Node *GetParent(Node *p) {
        Node *last = nullptr;
        Node *found = root_;
        while (found && found->x != p->x) {
            last = found;
            if (p->x < found->x) {
                found = found->left;
            } else if (p->x > found->x) {
                found = found->right;
            }
        }
        return last;
    }

    void Insert(int x) {
        Node *last = nullptr;
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
        inserted->left = nullptr;
        inserted->right = nullptr;
        inserted->x = x;

        if (!last) {
            inserted->succ = nullptr;
            root_ = inserted;

        } else if (x < last->x) {
            inserted->succ = last;
            Node *parent = GetParent(last); 
            if (parent && parent->right == last) {
                parent->succ = inserted;
            }

            last->left = inserted;

       } else {
            inserted->succ = last->succ;
            last->succ = inserted;

            last->right = inserted;
        }
    }


    void Delete(Node *p) {
        if (!p->left) {
            Transplant(p, p->right);
            Node *predec = GetParent(p);
            if (predec && predec->right == p) {
                predec->succ = p->succ;
            }

        } else if (!p->right) {
            Transplant(p, p->left);
            Node *predec = Maximum(p->left);
            predec->succ = p->succ;

        } else {
            Node *next = p->succ;
            Node *next_parent = GetParent(next);
            if (next_parent != p) {
                Transplant(next, next->right);
                next->right = p->right;
            }
            Transplant(p, next);
            next->left = p->left;

            Node *predec = Maximum(p->left);
            predec->succ = p->succ;
        }
        
        delete p;
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

    void Print() {
        Print(root_);
    }

    Node *Minimum() {
        return Minimum(root_);
    }

    Node *Maximum() {
        return Maximum(root_);
    }

private:
    void Print(Node *p) {
        if (p) {
            Print(p->left);
            cout << p->x << endl;
            Print(p->right);
        }
    }

    void Release(Node *p) {
         if (p) {
            Release(p->left);
            Release(p->right);
            delete p;
        }
    }

    void Transplant(Node *old, Node *fresh) {
        Node *old_parent = GetParent(old);
        if (!old_parent) {
            root_ = fresh;
        } else if (old == old_parent->left) {
            old_parent->left = fresh;
        } else {
            old_parent->right = fresh;
        }
    }

    static Node *Maximum(Node *p) {
        while (p->right) {
            p = p->right;
        }
        return p;
    }

    static Node* Minimum(Node *p) {
        while (p->left) {
            p = p->left;
        }
        return p;
    }

    Node *root_;
};

/******************************************************************************/

int main() {

    BinaryTree tree;
/*
    tree.Insert(5);
    tree.Insert(2);
    tree.Insert(1);
    tree.Insert(8);
    tree.Insert(9);
    tree.Insert(3);
    tree.Insert(6);
    tree.Insert(7);
*/
    tree.InsertX(5);
    tree.InsertX(2);
    tree.InsertX(1);
    tree.InsertX(8);
    tree.InsertX(9);
    tree.InsertX(3);
    tree.InsertX(6);
    tree.InsertX(7);


    cout << endl;
    tree.InorderWalk();
    cout << endl;
    tree.PreorderWalk();
    cout << endl;
    tree.PostorderWalk();

    cout << endl;
    for (BinaryTree::Node *it = tree.Minimum(); it; it = BinaryTree::Successor(it)) {
        cout << it->x << endl;
    }

    tree.Delete(tree.Search(7));

    for (auto it = tree.Maximum(); it; it = BinaryTree::Predecessor(it)) {
        cout << it->x << endl;
    }

    BinaryTreeX tx;
    tx.Insert(5);
    tx.Insert(2);
    tx.Insert(1);
    tx.Insert(8);
    tx.Insert(9);
    tx.Insert(3);
    tx.Insert(6);
    tx.Insert(7);
    
    cout << endl;
    cout << "Ex tree" << endl;
    tx.Print();

    tx.Delete(tx.Search(3));
    tx.Delete(tx.Search(5));
    tx.Delete(tx.Search(8));
    
    cout << endl;
    tx.Print();

    for (auto it = tx.Minimum(); it; it = it->succ) {
        cout << it->x << endl;
    }

    return 0;
}


