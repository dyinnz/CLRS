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

#define left_child(x) ((x) << 1)
#define right_child(x) (((x) << 1) + 1)
#define parent(x) (x >> 1)

void MinHeapify(vector<Node*> &A, size_t pos) {
    size_t left = left_child(pos);
    size_t right = right_child(pos);

    size_t minpos {-1};
    if (left < A.size() && A[left] > A[pos]) {
        maxpos = left;
    } else {
        maxpos = pos;
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

    auto dag_graph = ReadGraph(dag_edges, 6); 
    auto topo_order = TopoSort(dag_graph, 0);
    DAGShortestPaths(dag_graph, 1);
    cout << endl;
    for (auto p : topo_order) {
        //cout << p->index << endl;
        cout << p->index << " " << p->distance << endl;
    }

    cout << CalcDAGPaths(dag_graph, 0) << endl;

    return 0;
}


