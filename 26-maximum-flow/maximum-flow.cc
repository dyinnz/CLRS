/*
 * Author: Dyinnz
 * Email: ml_143@sina.com
 * Description: 
 *      Maximum Flow
 */

#include <cassert>

#include <iostream>
#include <vector>
#include <list>
#include <queue>

using namespace std;

struct ReadEdge {
  int u, v;
  int capacity;
};

class FlowGraph {
  private:
    struct Edge {
      int v;
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

    const list<Edge> &get_adjacence(int v) {
      return _graph[v].adjacence;
    }

    int FindAugmentPath(int s, int t, vector<Edge*> &path) {
      path.clear();
      
      // using BFS
      vector<int> father(_graph.size());
      queue<int> q;
      q.push(s);
      while (!q.empty()) {
        int u = q.front(); q.pop();

        if (u == t) {
          // get the path
          return 1;
        }
        
        if (0 == father[u]) {
          continue;
        }

        for (auto &e : get_adjacence(u)) if (get_residue(e) > 0) {
          father[e.v] = u;
          q.push(e.v);
        }
      }

      return 0;
    }

  public:
    FlowGraph() {
    }

    void ReadGraph(int size, const vector<ReadEdge> &edges) {
      _graph.resize(size+1);
      for (auto &edge : edges) {
        _graph[edge.u].adjacence.push_back({ edge.v, edge.capacity, 0 });
        _graph[edge.u].adjacence.push_back({ edge.u, 0, 0 });
      }
    }

    void FordFulkerson(int s, int t) {
      vector<Edge*> path;
      int augmented = 0;
      while (0 != (augmented = FindAugmentPath(s, t, path))) {
        for (auto pedge : path) {
          if (is_real_edge(*pedge)) {
            pedge->flow += augmented;
          } else {
            pedge->flow -= augmented;
          }
        }
      }
    }
};

int main() {
  vector<ReadEdge> edges = {
    {1, 2, 2},
    {2, 3, 3},
  };

  FlowGraph graph;
  graph.ReadGraph(3, edges);
  graph.FordFulkerson(1, 3);


  return 0;
}


