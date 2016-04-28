/*
 * Author: Dyinnz
 * Email: ml_143@sina.com
 * Description: 
 *  All-Pairs Shortest Paths
 */

#include <iostream>
#include <vector>

#include <climits>
#include <cassert>
#include <cstring>

using namespace std;

struct Edge {
  int u, v, w;
};

class MatrixGraph {
  public:
    MatrixGraph(const size_t vn) : _vn(vn) {
      _weights = new int[_vn * _vn];
      _distances = new int[_vn * _vn];
      _parents = new size_t[_vn * _vn];

      memset(_weights, 0, sizeof(int) * _vn * _vn);
      memset(_parents, -1, sizeof(size_t) * _vn * _vn);

      for (size_t i = 0; i < _vn; ++i) {
        for (size_t j = 0; j < _vn; ++j) {
          SetDistance(i, j, INT_MAX);
        }
      }
    }

    void ReadGraph(const vector<Edge> &edges) {
      for (auto &edge : edges) {
        SetWeight(edge.u, edge.v, edge.w);
        SetDistance(edge.u, edge.v, edge.w);
        SetParent(edge.u, edge.v, edge.u);
      }

      for (size_t i = 0; i < _vn; ++i) {
        SetWeight(i, i, 0);
        SetDistance(i, i, 0);
      }
    }

    int GetWeight(size_t x, size_t y) const { 
      return *(_weights + x*_vn + y);
    }

    int GetDistance(size_t x, size_t y) const {
      return *(_distances + x*_vn + y);
    }

    size_t GetParent(size_t x, size_t y) const {
      return *(_parents + x*_vn + y);
    }

    size_t GetSize() const {
      return _vn;
    }

    void FloydWarshall() {
      for (size_t k = 0; k < _vn; ++k) {
        for (size_t i = 0; i < _vn; ++i) {
          for (size_t j = 0; j < _vn; ++j) {

            if (INT_MAX == GetDistance(i, k) || INT_MAX == GetDistance(k, j)) {
              continue;
            }

            if (GetDistance(i, j) > GetDistance(i, k) + GetDistance(k, j)) {

              int dist = GetDistance(i, k) + GetDistance(k, j);
              SetDistance(i, j, dist);
              SetParent(i, j, GetParent(k, j));
            }

          } } }

      DebugDistances();
      DebugParents();
    }

    void DebugDistances() {
      cout << "Distance Matrix:" << endl;
      for (size_t i = 0; i < _vn; ++i) {
        for (size_t j = 0; j < _vn; ++j) {
          if (GetDistance(i, j) == INT_MAX) {
            cout << "oo\t";
          } else {
            cout << GetDistance(i, j) << '\t';
          }

        }
        cout << endl;
      }
      cout << endl;
    }

    void DebugParents() {
      cout << "Parents Matrix:" << endl;
      for (size_t i = 0; i < _vn; ++i) {
        for (size_t j = 0; j < _vn; ++j) {
          if (UINT_MAX == GetParent(i, j)) {
            cout << "X\t";
          } else {
            cout << GetParent(i, j) << '\t';
                }
            }
            cout << endl;
        }
        cout << endl;
    }
    
private:
    size_t _vn;
    int *_weights;
    int *_distances;
    size_t *_parents;

    void SetWeight(size_t x, size_t y, int w) {
        *(_weights + x*_vn + y) = w;
    }

    void SetDistance(size_t x, size_t y, int d) {
        *(_distances + x*_vn + y) = d;
    }

    void SetParent(size_t x, size_t y, size_t p) {
        *(_parents + x*_vn + y) = p;
    }
};

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

    MatrixGraph matrix(5);
    matrix.ReadGraph(edges);
    matrix.FloydWarshall();

    for (size_t i = 0; i < matrix.GetSize(); ++i) {
        cout << i << " " << matrix.GetDistance(0, i) << endl;
    }
}




