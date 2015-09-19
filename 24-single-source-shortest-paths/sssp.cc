/*
 * Author: Dyinnz
 * Email: ml_143@sina.com
 * Description:
 *  Single-Source Shortest Paths
 */
#include <climits>
#include <cassert>

#include <vector>
#include <list>
#include <iostream>
#include <functional>

using namespace std;
struct Edge {
    int u, v, w;
};

struct Node {
    Node () : index(0), distance(INT_MAX), parent(nullptr) {}
    int index;
    int distance;
    Node *parent;
    list<pair<Node*, int>> edges;
};

vector<Node> ReadGraph(vector<Edge> &edges, size_t vn) {
    vector<Node> graph(vn);
    for (size_t i = 0; i < vn; ++i) {
        graph[i].index = i;
    }

    for (auto &e : edges) {
        // cout << __func__ << " " << e.u << " " << e.v << endl; 
        graph[e.u].edges.push_back( {&(graph[e.v]), e.w} );
    }
    return move(graph);
}

void Relax(Node *u, Node *v, int w) {
    if (INT_MAX == u->distance) {
        return;

    } else if (v->distance > u->distance + w) {
        v->distance = u->distance + w;
        v->parent = u;
    }
}

bool BellmanFord(vector<Node> &graph, vector<Edge> &edges, int s) {
    assert(!graph.empty());

    graph[s].distance = 0;

    for (size_t i = 0; i < graph.size(); ++i) {
        for (auto &e : edges) {
            Relax(&graph[e.u], &graph[e.v], e.w);
        }
    }

    for (auto &e : edges) {
        if (graph[e.v].distance > graph[e.u].distance + e.w) {
            return false;
        }
    }

    return true;
}

void TopoDFS(Node *pu, vector<Node*> &topo_order) {
    pu->distance = -1;

    for (auto &edge : pu->edges) {
        Node *pv = edge.first;
        if (-1 != pv->distance) {
            TopoDFS(pv, topo_order);
        }
    }

    topo_order.push_back(pu);
}

vector<Node*> TopoSort(vector<Node> &graph, int s) {
    vector<Node*> topo_order;

    for (auto &node : graph) {
        node.distance = 0;
    }

    TopoDFS(&graph[s], topo_order);
    
    return move(topo_order);
}

void DAGShortestPaths(vector<Node> &graph, int s) {
    assert(!graph.empty());

    auto topo_order = TopoSort(graph, s);

    // clear the distance
    for (auto &node : graph) {
        node.distance = INT_MAX;
    }
    graph[s].distance = 0;

    for (auto iter = topo_order.rbegin(); iter != topo_order.rend(); ++iter) {
        Node *pu = *iter;
        for (auto &edge : pu->edges) {
            Relax(pu, edge.first, edge.second);
        }
    }
}

int CalcDAGPaths(vector<Node> &graph, int s) {
    assert(!graph.empty());

    auto topo_order = TopoSort(graph, s);
    
    // clear the distance
    for (auto &node : graph) {
        node.distance = 0;
    }
    graph[s].distance = 1;

    for (auto iter = topo_order.rbegin(); iter != topo_order.rend(); ++iter) {
        Node *pu = *iter;
        for (auto &edge : pu->edges) {
            Node *pv = edge.first;
            pv->distance += pu->distance;
        }
    }
    
    int ans = 0;
    graph[s].distance -= 1;
    for (auto iter = topo_order.rbegin(); iter != topo_order.rend(); ++iter) {
        Node *pu = *iter;
        cout << pu->index << " " << pu->distance << endl;
        ans += pu->distance;
    }

    return ans;
}


/******************************************************************************/

/* default for max heap */
template<class T, class PositionSetter, class Compare = less<T>>
class Heap {
public:
    Heap(const PositionSetter &set_pos = PositionSetter(), const Compare &cmp = Compare())
        : _set_pos(set_pos), _cmp(cmp) {
        _data.push_back(T());
        //CorrectPosition(0);
    }

    bool Empty() const {
        return 1 == _data.size();
    }

    const T& Top() const {
        assert(!Empty());
        return _data[1];
    }

    T Pop() {
        assert(!Empty());

        swap(_data[1], _data.back());
        CorrectPosition(1);

        T temp = _data.back();
        _data.pop_back();

        MaxHeapify(1);

        return move(temp);
    }

    void Push(const T &key) {
        _data.push_back(key);
        Float(_data.size()-1);
    }

    void Push(T &&key) {
        _data.push_back(key);
        Float(_data.size()-1);
    }

    void IncreaseKey(size_t pos, const T &key) {
        if (_cmp(_data[pos], key)) {
            _data[pos] = key;
            Float(pos);
        }
    }

    void IncreaseKey(size_t pos, function<void(T &key)> change) {
        change(_data[pos]);
        Float(pos);
    }

    void Float(size_t pos) {
        while (parent(pos) > 0 && _cmp(_data[parent(pos)], _data[pos])) {
            swap(_data[parent(pos)], _data[pos]);
            CorrectPosition(pos);
            pos = parent(pos);
        }
        CorrectPosition(pos);
    }

    vector<T>& DebugGetData() {
        return _data;
    }

private:
    vector<T> _data;
    const PositionSetter _set_pos;
    const Compare _cmp;

    void MaxHeapify(const size_t pos) {
        size_t left = left_child(pos);
        size_t right = right_child(pos);

        size_t maxpos {pos};
        if (0 < left && left < _data.size() && _cmp(_data[maxpos], _data[left])) {
            maxpos = left;
        }
        if (0 < right && right < _data.size() && _cmp(_data[maxpos], _data[right])) {
            maxpos = right;
        }
        if (maxpos != pos) {
            swap(_data[pos], _data[maxpos]);
            MaxHeapify(maxpos);
        }
    }

    void CorrectPosition(const size_t pos) {
        _set_pos(_data[pos], pos);
    }

    static size_t left_child(size_t x) { return x << 1; }
    static size_t right_child(size_t x) { return (x << 1) + 1; }
    static size_t parent(size_t x) { return x >> 1; }
};

void Dijkstra(vector<Node> &graph, int s) {
    vector<int> heap_pos(graph.size());
    auto set_pos = [&](Node *x, size_t pos) { heap_pos[x->index] = pos; };

    auto cmp = [](Node *x, Node *y) { return x->distance > y->distance; };

    Heap<Node*, function<void(Node*, size_t)>, function<bool(Node *x, Node *y)>>
        heap(set_pos, cmp);

    for (auto &node : graph) {
        node.distance = INT_MAX;
    }
    graph[s].distance = 0;

    for (auto &node : graph) {
        heap.Push(&node);
    }

    while (!heap.Empty()) {
        Node *pu = heap.Pop();
        for (auto &edge : pu->edges) {
            Relax(pu, edge.first, edge.second);
            heap.Float(heap_pos[edge.first->index]);
        }
    }

    for (auto &node : graph) {
        cout << node.index << " " << node.distance << endl;
    }
}

int main() {
    vector<Edge> edges {
        {0, 1, 6},
        {0, 3, 7},
        {1, 2, 5},
        {1, 3, 8},
        {1, 4, -4},
        {2, 1, -2},
        {3, 2, -3},
        {3, 4, 9},
        {4, 2, 7},
        {4, 0, 2},
    };

    auto graph = ReadGraph(edges, 5);
    if (!BellmanFord(graph, edges, 0)) {
        cout << "exist negative-weight cycle\n" << endl;
    } else {
        for (auto node : graph) {
            cout << node.index << " " << node.distance << endl;
        }
    }
    cout << endl;

    vector<Edge> dag_edges {
        {0, 1, 5},
        {0, 2, 3},
        {1, 2, 2},
        {1, 3, 6},
        {2, 3, 7},
        {2, 4, 4},
        {2, 5, 2},
        {3, 4, -1},
        {3, 5, 1},
        {4, 5, -2},
    };

    /******************************************************************************/

    auto dag_graph = ReadGraph(dag_edges, 6); 
    auto topo_order = TopoSort(dag_graph, 0);
    DAGShortestPaths(dag_graph, 1);
    for (auto p : topo_order) {
        //cout << p->index << endl;
        cout << p->index << " " << p->distance << endl;
    }

    cout << CalcDAGPaths(dag_graph, 0) << endl;

    cout << endl;

    /******************************************************************************/
    vector<Edge> dij_edges {
        {0, 1, 10},
        {0, 3, 5},
        {1, 2, 1},
        {1, 3, 2},
        {2, 4, 4},
        {3, 1, 3},
        {3, 2, 9},
        {3, 4, 2},
        {4, 2, 6},
        {4, 0, 3},
    };
    auto dij_graph = ReadGraph(dij_edges, 5);
    Dijkstra(dij_graph, 0);


    return 0;
}


