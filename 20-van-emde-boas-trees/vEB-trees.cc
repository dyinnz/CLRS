#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <climits>

using namespace std;

class vEBTree {
public:
    struct vEBNode {
        int     max {INT_MIN},
                min {INT_MIN};
        int     u {0};
        int     sqrt_u {0};

        vEBNode *summary {0};
        vEBNode *cluster {0};

        int high(int key) {
            return key / floor(sqrt(u));
        }

        int low(int key) {
            return key % int(floor(sqrt(u)));
        }
        
        int index(int x, int y) {
            return x * floor(sqrt(u)) + y;
        }
    };

    vEBTree(int u) {
        root_ = new vEBNode;
        InitNode(root_, u);
    }

    ~vEBTree() {
        Release(root_);
        delete root_;
    }

    int Minimum() {
        return Minimum(root_);
    }

    int Maximum() {
        return Maximum(root_);
    }

    bool Member(int key) {
        return Member(root_, key);
    }

    int Successor(int key) {
        return Successor(root_, key);
    }

    int Predecessor(int key) {
        return Predecessor(root_, key);
    }

    void Insert(int key) {
        Insert(root_, key);
    }

    void Delete(int key) {
        Delete(root_, key);
    }
    
    void Print() {
        Print(root_, 0);
    }

private:
    int Minimum(vEBNode *v) {
        return v->min;
    }

    int Maximum(vEBNode *v) {
        return v->max;
    }

    bool Member(vEBNode *v, int key) {
        if (key == v->min || key == v->max) {
            return true;
        } else if (2 == v->u) {
            return false;
        } else {
            return Member(&v->cluster[v->high(key)], v->low(key));
        }
    }

    int Successor(vEBNode *v, int key) {
        if (2 == v->u) {
            if (0 == key && 1 == v->max) {
                return 1;
            } else {
                return INT_MIN;
            }

        } else if (INT_MIN != v->min && key < v->min) {
            return v->min;

        } else {
            int max_low = Maximum(&v->cluster[v->high(key)]);
            if (INT_MIN != max_low && v->low(key) < max_low) {
                int offset = Successor(&v->cluster[v->high(key)], v->low(key));
                return v->index(v->high(key), offset);

            } else {
                int succ_cluster = Successor(v->summary, v->high(key));
                if (INT_MIN == succ_cluster) {
                    return INT_MIN;
                } else {
                    int offset = Minimum(&v->cluster[succ_cluster]);
                    return v->index(succ_cluster, offset);
                }
            }
        }

    }

    int Predecessor(vEBNode *v, int key) {
        if (2 == v->u) {
            if (1 == key && 0 == v->min) {
                return 0;
            } else {
                return INT_MIN;
            }

        } else if (INT_MIN != v->max && key > v->max) {
            return v->max;

        } else {
            int min_low = Minimum(&v->cluster[v->high(key)]);

            if (INT_MIN != min_low && v->low(key) > min_low) {
                int offset = Predecessor(v->summary, v->high(key));
                return v->index(v->high(key), offset);

            } else {
                int pred_cluster = Predecessor(v->summary, v->high(key));
                if (INT_MIN == pred_cluster) {
                    if (INT_MIN != v->min && key > v->min) {
                        return v->min;
                    } else {
                        return INT_MIN;
                    }

                } else {
                    int offset = Maximum(&v->cluster[pred_cluster]);
                    return v->index(pred_cluster, offset);
                }
            }
        }
    }

    void InsertEmpty(vEBNode *v, int key) {
        v->min = v->max = key;
    }

    void Insert(vEBNode *v, int key) {
        if (INT_MIN == v->min) {
            InsertEmpty(v, key);

        } else {
            if (key < v->min) {
                swap(v->min, key);
            }

            if (v->u > 2) {
                if (INT_MIN == Minimum(&v->cluster[v->high(key)])) {
                    Insert(v->summary, v->high(key));
                    InsertEmpty(&v->cluster[v->high(key)], v->low(key));
                } else {
                    Insert(v->cluster+v->high(key), v->low(key));
                }
            }

            if (key > v->max) {
                v->max = key;
            }
        }
    }

    void Delete(vEBNode *v, int key) {
        if (v->min == v->max) {
            v->min = v->max = INT_MIN;
            
        } else if (2 == v->u) {
            if (0 == key) {
                v->min = 1;
            } else {
                v->min = 0;
            }
            v->max = v->min;

        } else {
            if (key == v->min) {
                int first_cluster = Minimum(v->summary);
                key = v->index(first_cluster, Minimum(v->cluster + first_cluster));
                v->min = key;
            }
            Delete(v->cluster + v->high(key), v->low(key));

            if (INT_MIN == Minimum(v->cluster + v->high(key))) {
                Delete(v->summary, v->high(key));
                if (key == v->max) {
                    int summary_max = Maximum(v->summary);
                    if (INT_MIN == summary_max) {
                        v->max = v->min;

                    } else {
                        v->max = v->index(summary_max, Maximum(v->cluster + summary_max));
                    }
                }

            } else if (key == v->max) {
                v->max = v->index( v->high(key), Maximum(v->cluster + v->high(key)) );
            }

        }
    }

    void InitNode(vEBNode *v, int u) {
        v->u = u;
        if (u > 2) {
            v->summary = new vEBNode;
            InitNode(v->summary, ceil(sqrt(v->u)) );

            v->cluster = new vEBNode[v->summary->u];

            int floor_sqrt = floor(sqrt(v->u));
            for (int i = 0; i < v->summary->u; ++i) {
                InitNode(v->cluster + i, floor_sqrt);
            }
        }
    }

    void Release(vEBNode *v) {
        if (v->summary) {
            for (int i = 0; i < v->summary->u; ++i) {
                Release(v->cluster + i);
            }
            delete[] v->cluster;
            delete v->summary;
        }
    }

    void PrintSpace(int deep) {
        deep <<= 2;
        while (deep--) putchar(' ');
    }

    void Print(vEBNode *v, int deep) {
        PrintSpace(deep); 
        cout << "u: " << v->u << " min: " << v->min << " max: " << v->max << endl;

        if (v->summary) {
            PrintSpace(deep);
            cout << "summary: " << "size: " << v->summary->u << endl;
            Print(v->summary, deep+1);

            PrintSpace(deep);
            cout << "cluster: " << endl;
            for (int i = 0; i < v->summary->u; ++i) {
                PrintSpace(deep);
                cout << i << ": " << endl;
                Print(v->cluster + i, deep+1);
            }
        }
    }

    vEBNode *root_ {0};
};

int main() {
     
    vEBTree tree(16);
    tree.Print();
    cout << "----------------------------------------------------------------------------" << endl;

    tree.Insert(2);
    tree.Insert(3);
    tree.Insert(4);
    tree.Insert(5);
    tree.Insert(7);
    tree.Insert(14);
    tree.Insert(15);
    tree.Print();

    cout << "----------------------------------------------------------------------------" << endl;
    tree.Delete(5);
    tree.Print();
    
    cout << "----------------------------------------------------------------------------" << endl;
    cout << tree.Predecessor(7) << endl;
    cout << tree.Successor(14) << endl;
    cout << endl;
    return 0;
}
