/*
 * Author: Dyinnz
 * Email: ml_143@sina.com
 * Description: 
 *      Maximum Flow
 */

#include <cassert>
#include <climits>

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <unordered_map>
#include <functional>

using namespace std;

struct ReadEdge {
  int u, v;
  int capacity;
};

class FlowGraph {
  private:
    struct Edge {
      int u, v;
      int capacity;
      int flow;
    };

    struct Node {
      int index;
      list<Edge> adjacence;
    };
    vector<Node> _graph;

    bool is_real_edge(const Edge &e) const {
      return 0 != e.capacity;
    }

    int get_residue(const Edge &e) const {
      if (is_real_edge(e)) {
        return e.capacity - e.flow;
      } else {
        return e.flow;
      }
    }

    list<Edge> &get_adjacence(int v) {
      return _graph[v].adjacence;
    }

    int AugmentFordFulkerson(int s, int t, vector<Edge*> &path) {
      path.clear();

      cout << __func__ << endl;
      
      // using BFS
      vector<Edge*> last_edge(_graph.size());
      queue<int> q;
      q.push(s);
      while (!q.empty()) {
        int u = q.front(); q.pop();

        // record the path
        if (u == t) {
          int pv = t;
          int augmented = INT_MAX;
          while (nullptr != last_edge[pv]) {
            cout << "Path: " << pv << endl;

            path.push_back(last_edge[pv]);
            if (get_residue(*last_edge[pv]) < augmented) {
              augmented = get_residue(*last_edge[pv]);
            }

            pv = last_edge[pv]->u;
          }
          cout << "Path: " << pv << endl;
          return augmented;
        }

        // bread-first search
        for (auto &e : get_adjacence(u)) {
          if (nullptr == last_edge[e.v] && get_residue(e) > 0) {
            cout << u << "->" << e.v << " residue: " << get_residue(e) << endl;
            last_edge[e.v] = &e;
            q.push(e.v);
          }
        }
      }

      return 0;
    }

  public:
    enum class Algorithm {
      FordFulkerson, EdmondsKarp,
    };

    FlowGraph() {
    }

    void ReadGraph(int size, const vector<ReadEdge> &edges) {
      _graph.resize(size+1);
      for (auto &edge : edges) {
        _graph[edge.u].adjacence.push_back({ edge.u, edge.v, edge.capacity, 0 });
        _graph[edge.v].adjacence.push_back({ edge.v, edge.u, 0, 0 });
      }
    }

    int CalcMaximumFlow(int s, int t, enum Algorithm) {
      vector<Edge*> path;
      int augmented = 0;
      while (0 != (augmented = AugmentFordFulkerson(s, t, path))) {
        for (auto pedge : path) {
          if (is_real_edge(*pedge)) {
            pedge->flow += augmented;
          } else {
            pedge->flow -= augmented;
          }
        }
      }

      int ans = 0;
      for (auto e : get_adjacence(s)) {
        ans += e.flow;
      }
      return ans;
    }
};

class PushRelabel {
  private:
    struct Edge {
      Edge(int u, int v, int c, int f) : u(u), v(v), capacity(c), flow(f) {}
      int u, v;
      int capacity, flow;
    };
    
    struct Node {
      int index;
      int height;
      int excess;
      unordered_map<int, Edge> neighbors_map;
      list<int> neighbors_list;
      list<int>::iterator current;
    };

    vector<Node> _graph;
    int _s {0},
        _t {0};

  public:
    PushRelabel() { }

    void ReadGraph(int size, const vector<ReadEdge> &edges, int s, int t) {
      _s = s;
      _t = t;
      _graph.resize(size+1);
      for (size_t i = 1; i <= size; ++i) {
        _graph[i].index = i;
      }

      // init node, set the heght of source
      for (auto &node : _graph) {
        node.height = 0;
        node.excess = 0;
      }
      node_at(s).height = size;

      // read edge, and set the flow of edge
      for (auto &edge : edges) {
        auto u2v = Edge(edge.u, edge.v, edge.capacity, 0);
        set_neighbor(edge.u, edge.v, u2v);

        auto v2u = Edge(edge.v, edge.u, 0, 0);
        set_neighbor(edge.v, edge.u, v2u);
      }

      // preflow
      for (auto &p : get_neighbors_map(s)) {
        auto &edge = p.second;
        edge.flow =  edge.capacity;
        node_at(edge.v).excess = edge.flow;
        node_at(s).excess -= edge.flow;
      }
    }

    int CalcPushRelabel() {
      list<int> L;
      for (auto &node : _graph) {
        cout << "start: current u: " << node.index
          << " height: " << node.height
          << " excess: " << node.excess
          << endl;

        if (0 != node.index && _s != node.index && _t != node.index) {
          L.push_back(node.index);
          node.current = node.neighbors_list.begin();
        }
      }
      auto iter = L.begin();
      while (L.end() != iter) {
        auto u = *iter;
        auto oldh = node_at(u).height;
        Discharge(u);
        if (node_at(u).height > oldh && L.size() > 1) {
          L.splice(L.end(), L, L.begin());
        }
        ++iter;

        cout << "current u: " << u << " height: " << node_at(u).height << 
          " excess: " << node_at(u).excess << endl;
      }

      return node_at(_t).excess;
    }

  private:
    Node& node_at(int v) {
      return _graph[v];
    }

    Edge& get_edge(int u, int v) {
      return get_neighbors_map(u).at(v);
    }

    bool is_real_edge(const Edge &e) {
      return 0 != e.capacity;
    }

    int get_residue(const Edge &e) {
      return is_real_edge(e) ? (e.capacity - e.flow) : e.flow;
    }

    int get_residue(int u, int v) {
      return get_residue(get_edge(u, v));
    }

    list<int>& get_neighbors_list(int u) {
      return node_at(u).neighbors_list;
    }

    unordered_map<int, Edge>& get_neighbors_map(int u) {
      return node_at(u).neighbors_map;
    }

    void set_neighbor(int u, int v, Edge &e) {
      get_neighbors_map(u).insert({v, e});
      get_neighbors_list(u).push_back(v);
    }

    void set_neighbor_map(int u, int v, Edge &&e) {
      get_neighbors_map(u).insert({v, e});
      get_neighbors_list(u).push_back(v);
    }


    /**
     * Push delta_flow units of flow from u to v.
     * @notice: Applies when u is overflowing, cf(u, v) > 0, and u.h == v.h + 1
     */
    void Push(int u, int v) {
      auto &u2v = get_edge(u, v);
      int delta_flow = min(node_at(u).excess, get_residue(u2v));
      if (is_real_edge(u2v)) {
        u2v.flow += delta_flow; 
      } else {
        u2v.flow -= delta_flow;
      }
      node_at(u).excess -= delta_flow;
      node_at(v).excess += delta_flow;
    }

    /**
     * Increase the height of u.
     * @notice: Applies when u is overflowing and for all v of V such that
     * (u, v) of Ef, we have u.h <= v.h
     */
    void Relabel(int u) {
      int minh = INT_MAX;
      for (auto v : get_neighbors_list(u)) {
        if (node_at(v).height < minh) {
          minh = node_at(v).height;
        }
      }
      node_at(u).height = minh + 1;
    }

    void Discharge(int u) {
      while (node_at(u).excess > 0) {
        if (get_neighbors_list(u).end() == node_at(u).current) {
          Relabel(u);
          node_at(u).current = get_neighbors_list(u).begin();

        } else {
          auto v = *node_at(u).current;

          if (get_residue(u, v) > 0 
              && node_at(v).height+1 == node_at(u).height) {
            Push(u, v);
          } else {
            ++node_at(u).current;
          }
        }

      } // end of while
    } // end of Discharge

};

int main() {
  vector<ReadEdge> edges = {
    {1, 2, 2},
    {2, 3, 3},
  };

  FlowGraph graph;
  graph.ReadGraph(3, edges);
  int max_flow = graph.CalcMaximumFlow(1, 3, FlowGraph::Algorithm::FordFulkerson);
  cout << "max flow: " << max_flow << endl;

  PushRelabel push_relabel;
  push_relabel.ReadGraph(3, edges, 1, 3);
  max_flow = push_relabel.CalcPushRelabel();
  cout << "push relabel max flow: " << max_flow << endl;


  return 0;
}

