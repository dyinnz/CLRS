#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <queue>
#include <functional>

using namespace std;

class DisjointSet {
public:
    DisjointSet(int size) : sets(size) {
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
};

struct ToPoint {
    ToPoint(int u = 0, int w = 0) : u(u), w(w) {}
    int u;
    int w;
};

struct Edge {
    int v, u, w;
};

vector<list<ToPoint>> ReadGraph(const vector<Edge> &edges, size_t vn) {

    vector<list<ToPoint>> graph(vn+1);
    
    for(size_t i = 0; i < edges.size(); ++i) {
        int v = edges[i].v;
        int u = edges[i].u;
        int w = edges[i].w;
        graph[v].push_back(ToPoint(u, w));
        graph[u].push_back(ToPoint(v, w));
    }

    return move(graph);
}

vector<pair<int, int>> Prim(const vector<list<ToPoint>> &graph) {
    size_t vn = graph.size()-1;
    vector<int> parent(vn+1);
    vector<bool> visit(vn+1);
    vector<pair<int, int>> mst;
    mst.reserve(vn-1);

    struct Vertex {
        int u, key, parent;
    };
    auto f = [](Vertex &x, Vertex &y) { return x.key > y.key; };

    //priority_queue<Vertex, vector<Vertex>, function<bool(Vertex&, Vertex&)>> q(f);
    priority_queue<Vertex, vector<Vertex>, decltype(f)> q(f);

    q.push( {1, 0, 0} );
    
    int total = 0;
    while (!q.empty() && mst.size() < vn-1) {
        int v = q.top().u;
        int w = q.top().key;
        int p = q.top().parent;
        q.pop();

        if (visit[v]) {
            continue;
        }
        visit[v] = true;

        cout << v << " " << w <<  " " << p << endl;

        total += w;

        if (0 != p) {
            mst.push_back( {p, v} );
        }
        
        for (auto &tp : graph[v]) if (!visit[tp.u]) {
            q.push( {tp.u, tp.w, v} );
        }
    }

    cout << "total weights: " << total << endl;

    return move(mst);
}

vector<pair<int, int>> Kruskal(vector<Edge> &edges, size_t vn) {

    vector<pair<int, int>> mst;
    mst.reserve(vn-1);
    DisjointSet ds(vn+1);

    sort(edges.begin(), edges.end(), [](Edge &x, Edge &y) { return x.w < y.w; });
    
    size_t select = 0;
    size_t total = 0;
    for (size_t i = 0; i < edges.size() && select < vn-1; ++i) {
        int v = edges[i].v;
        int u = edges[i].u;
        if (ds.Find(v) != ds.Find(u)) {
            ds.Union(v, u);
            mst.push_back({v, u});
            total += edges[i].w;
            ++select;
        }
    }

    cout << "total weights: " << total << endl;

    return move(mst);
}

int main() {
    vector<Edge> edges {
        {1, 2, 4},
        {2, 3, 8},
        {3, 4, 7},
        {4, 5, 9},
        {5, 6, 10},
        {6, 7, 2},
        {7, 8, 1},
        {8, 9, 7},
        {8, 1, 8},
        {2, 8, 11},
        {3, 9, 2},
        {3, 6, 4},
        {4, 6, 14},
        {7, 9, 6},
    };

    for (auto &p : Kruskal(edges, 9)) {
        cout << p.first << ' ' << p.second << endl;
    }
    cout << endl;

    auto graph = ReadGraph(edges, 9);
    for (auto &p : Prim(graph)) {
        cout << p.first << ' ' << p.second << endl;
    }   

    return 0;
}

