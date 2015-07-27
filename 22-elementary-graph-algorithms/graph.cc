#include <iostream>
#include <list>
#include <vector>
#include <algorithm>

using namespace std;

class Graph {
public:
    void Read(const vector<pair<int, int>> &edges, int n) {
        vertexs_.resize(n+1);
        for (auto &p : edges) {
            vertexs_[p.first].push_back(p.second);
        }
    }

    void DFS() {
        infos_.resize(vertexs_.size());
        for (auto &info : infos_) {
            info.parent = -1;
            info.color = kWhite;
            info.discovery = -1;
            info.finish = -1;
        }
        time_ = 0;

        for (int i = 1; i <= int(infos_.size()); ++i) if (kWhite == infos_[i].color) {
            DFSVisit(i);
        }

        cout << "topo sort: " << endl;
        for (auto &num : topo_) {
            cout << num << ' ';
        }
        cout << endl;
    }

    void DFSVisit(int v) {
        time_ += 1;
        infos_[v].discovery = time_;
        infos_[v].color = kGray;
        for (auto u : vertexs_[v]) {
            if (kWhite == infos_[u].color) {
                infos_[u].parent = v;
                DFSVisit(u);

                cout << "Tree edge: " << v << ' ' << u << endl;

            } else if (-1 == infos_[u].finish) {
                cout << "Back edge: " << v << ' ' << u << endl;

            } else if (infos_[u].finish > infos_[v].discovery) {
                cout << "Forward edge: " << v << ' ' << u << endl;
            } else {
                cout << "Cross edge: " << v << ' ' << u << endl;
            }
        }
        infos_[v].color = kBlack;
        time_ += 1;
        infos_[v].finish = time_;

        cout << v << " " << infos_[v].discovery << " " << infos_[v].finish << endl;

        topo_.push_front(v);
    }

private:
    enum Color { kWhite, kBlack, kGray };
    struct Info {
        int parent;
        Color color;
        int discovery;
        int finish;
    };
    vector<Info> infos_;
    int time_;

    list<int> topo_;

private:
    vector<list<int>> vertexs_;
};

class CalcSCC {
public:
    void Read(const vector<pair<int, int>> &edges, int n) {
        normal_.resize(n+1);
        reverse_.resize(n+1);
        for (auto &p : edges) {
            normal_[p.first].push_back(p.second);
            reverse_[p.second].push_back(p.first);
        }
    }

    void SCC() {
        visit_.resize(normal_.size());
        type_.resize(normal_.size());
        time_ = 0;

        for (size_t i = 1; i < normal_.size(); ++i) if (!visit_[i]) {
            DFSNormalVisit(i);
        }

        for (size_t i = 0;  i < visit_.size(); ++i) {
            visit_[i] = false;
        }

        int type = 0;
        for (int v : order_) if (!visit_[v]) {
            DFSReverseVisit(v, ++type);
        }
    }

    void DFSNormalVisit(int v) {
        visit_[v] = true;
        for (int u : normal_[v]) if (!visit_[u]) {
            DFSNormalVisit(u);
        }
        order_.push_front(v);
    }

    void DFSReverseVisit(int v, int type) {
        cout << v << ' ' << type << endl;
        visit_[v] = true;
        type_[v] = type;
        for (int u : reverse_[v]) if (!visit_[u]) {
            DFSReverseVisit(u, type);
        }
    }

private:
    vector<bool> visit_;
    list<int> order_;
    vector<int> type_;
    int time_;

private:
    vector<list<int>> normal_;
    vector<list<int>> reverse_;
};

class CalcBCC {
public:
    void Read(const vector<pair<int, int>> &edges, int n) {
        vertexs_.resize(n+1);
        for (auto &p : edges) {
            vertexs_[p.first].push_back(p.second);
            vertexs_[p.second].push_back(p.first);
        }
    }

    void DFS() {
        size_t n = vertexs_.size();
        visit.resize(n);
        d.resize(n);
        f.resize(n);
        parent.resize(n);
        low.resize(n);

        for (size_t i = 1; i < n; ++i) {
            d[i] = -1;
            f[i] = -1;
            low[i] = -1;
        }
        time = 0;
        
        for (size_t i = 1; i < n; ++i) if (!visit[i]) {
            DFSVisit(i);
        }

        for (size_t i = 0;  i < visit.size(); ++i) {
            visit[i] = false;
        }

        for (size_t i = 1; i < n; ++i) if (!visit[i]) {
            DFSBCC(i, 0);
        }

        cout << "record: " << endl;
        for (size_t i = 1; i < n; ++i) {
            cout << d[i] << ' ' << low[i] << endl;
        }
    }

    void DFSVisit(int v) {
        visit[v] = true;

        time += 1;
        d[v] = time;
        low[v] = time;

        for (auto u : vertexs_[v]) {
            if (!visit[u]) {
                parent[u] = v;
                DFSVisit(u);
            } else if (-1 == f[u] && u != parent[v]) {
                low[v] = min(low[v], d[u]);
            }
        }

        int flag = true;
        for (auto u : vertexs_[v]) if (u != parent[v]) {
            if (low[u] < d[v]) {
                low[v] = min(low[v], low[u]);
                flag = false;
            } else if (low[u] > d[v]) {
                cout << "bridge: " << v << ' ' << u << endl;
            }
        }

        if (flag) {
            cout << "articulation: " << v << endl;
        }

        time += 1;
        f[v] = time;
    }

    void DFSBCC(int v, int type) {
        visit[v] = true;
        for (auto u : vertexs_[v]) if (u != parent[v]) {
            if (!visit[u]) {
                if (low[u] > d[v]) {
                    DFSBCC(u, type+1);
                } else {
                    cout << "edge type: " << v << '-' << u << ' ' << type << endl;
                    DFSBCC(u, type);
                }
            } else {
                cout << "edge type: " << v << '-' << u << ' ' << type << endl;
            }
        }
    }

private:
    enum Color { kWhite, kBlack, kGray };

    vector<bool> visit;
    vector<int> d;
    vector<int> f;
    vector<int> parent;
    vector<int> low;
    int time;

private:
    vector<list<int>> vertexs_;
};

int main() {
/*    vector<pair<int, int>> edges {
        {1, 2},
        {1, 4},
        {4, 2},
        {2, 5},
        {5, 4},
        {3, 5},
        {3, 6},
        {6, 6},
    };
*/
    vector<pair<int, int>> edges {
        {1, 2},
        {1, 5},
        {2, 3},
        {2, 5},
        {3, 8},
        {4, 5},
        {6, 3},
        {6, 7},
        {7, 8},
    };

    Graph graph;
    graph.Read(edges, 8);
    graph.DFS();

    vector<pair<int, int>> scc_edges {
        {1, 2},
        {2, 3},
        {3, 1},
        {2, 4},
        {3, 4},
        {4, 5},
        {5, 4},
        {2, 6},
        {6, 5},
        {6, 7},
        {7, 6},
        {6, 8},
        {5, 8},
    };

    CalcSCC scc;
    scc.Read(scc_edges, 8);
    scc.SCC();

    vector<pair<int, int>> bcc_edges {
        {1, 2},
//        {1, 3},
        {1, 4},
        {2, 3},
//        {2, 4},
        {3, 4},
        {4, 5},
        {5, 6},
        {5, 7},
        {6, 7},
        {5, 10},
        {10, 11},
        {11, 8},
        {8, 9},
        {9, 10},
    };

    CalcBCC bcc;
    bcc.Read(bcc_edges, 11);
    bcc.DFS();

    return 0;
}
