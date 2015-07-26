#include <iostream>
#include <vector>
#include <list>
#include <map>

using namespace std;

class DisjointSet {
public:
    DisjointSet(int size) : sets(size), flags(size) {
        for (int i = 0; i < size; ++i) {
            sets[i].parent = i;
            sets[i].r = 0;
        }
    }

    void Union(int x, int y) {
        Link(Find(x), Find(y));
    }
    
    int Find(int x) {
        if (sets[x].parent != x) {
            sets[x].parent = Find(sets[x].parent);
        }
        return sets[x].parent;
    }

    void SetFlag(int x, int flag) {
        flags[Find(x)] = flag;
    }

    int GetFlag(int x) {
        return flags[Find(x)];
    }

private:
    struct Element {
        int parent {-1};
        int r {0};
    };

    void Link(int x, int y) {
        if (sets[x].r > sets[y].r) {
            sets[y].parent = x;

        } else {
            sets[x].parent = y;

            if (sets[x].r == sets[y].r) {
                sets[y].r += 1;
            }
        }
    }

    vector<Element> sets;
    vector<int> flags;
};

int main() {
    return 0;
}

