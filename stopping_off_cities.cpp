#include<bits/stdc++.h>
#define NIL -1
using namespace std;
int N;

struct pair_hash {
    template<class T1, class T2>
    std::size_t operator()(std::pair<T1, T2> const &pair) const {
        std::size_t h1 = std::hash<T1>()(pair.first);
        std::size_t h2 = std::hash<T2>()(pair.second);

        return min(h1, h2) ^ max(h1, h2);
    }
};

unordered_set<pair<int, int>, pair_hash> B;
int current_comp = 0;
vector<vector<int>> components;
unordered_set<int> allNodes;
unordered_set<pair<int, int>, pair_hash> visitedBridges;

class Graph {
    int V;
    list<int> *adj;

    void printAllPathsUtil(int, int, bool [], int [], int &);

public:
    Graph(int V);

    void addEdge(int u, int v);

    void printAllPaths(int s, int d);
};

Graph::Graph(int V) {
    this->V = V;
    adj = new list<int>[V];
}

void Graph::addEdge(int u, int v) {
    adj[u].push_back(v);
}

void Graph::printAllPaths(int s, int d) {
    bool *visited = new bool[V];

    int *path = new int[V];
    int path_index = 0;
    for (int i = 0; i < V; i++)
        visited[i] = false;

    printAllPathsUtil(s, d, visited, path, path_index);
}

void Graph::printAllPathsUtil(int u, int d, bool visited[],
                              int path[], int &path_index) {
    visited[u] = true;
    path[path_index] = u;
    path_index++;

    if (u == d) {
        for (int i = 0; i < path_index; i++) {
            if (i != 0) {
                pair<int, int> a;
                a.first = path[i - 1];
                a.second = path[i];
                visitedBridges.insert(a);
            }
            if (allNodes.find(path[i]) == allNodes.end()) {
                allNodes.insert(path[i]);
                if (allNodes.size() == N) return;
            }

        }
    } else {
        list<int>::iterator i;
        for (i = adj[u].begin(); i != adj[u].end(); ++i)
            if (!visited[*i])
                printAllPathsUtil(*i, d, visited, path, path_index);
    }
    path_index--;
    visited[u] = false;
}

class GBridges {
    int V;
    list<int> *adj;

    void bridgeUtil(int v, bool visited[], int disc[], int low[],
                    int parent[]);

public:
    GBridges(int V);

    void addEdge(int v, int w);

    void bridge();
};

GBridges::GBridges(int V) {
    this->V = V;
    adj = new list<int>[V];
}

void GBridges::addEdge(int v, int w) {
    adj[v].push_back(w);
    adj[w].push_back(v);
}

void GBridges::bridgeUtil(int u, bool visited[], int disc[],
                          int low[], int parent[]) {
    static int time = 0;
    visited[u] = true;
    disc[u] = low[u] = ++time;

    list<int>::iterator i;
    for (i = adj[u].begin(); i != adj[u].end(); ++i) {
        int v = *i;

        if (!visited[v]) {
            parent[v] = u;
            bridgeUtil(v, visited, disc, low, parent);

            low[u] = min(low[u], low[v]);
            if (low[v] > disc[u]) {
                pair<int, int> b;
                b.first = u;
                b.second = v;
                B.insert(b);
            }
        } else if (v != parent[u])
            low[u] = min(low[u], disc[v]);
    }
}

void GBridges::bridge() {
    bool *visited = new bool[V];
    int *disc = new int[V];
    int *low = new int[V];
    int *parent = new int[V];

    for (int i = 0; i < V; i++) {
        parent[i] = NIL;
        visited[i] = false;
    }

    for (int i = 0; i < V; i++)
        if (visited[i] == false)
            bridgeUtil(i, visited, disc, low, parent);
}


class GComponent {
    int V;
    list<int> *adj;

    void DFSUtil(int v, bool visited[]);

public:
    GComponent(int V);

    void addEdge(int v, int w);

    void connectedComponents();
};

void GComponent::connectedComponents() {
    bool *visited = new bool[V];
    for (int v = 0; v < V; v++)
        visited[v] = false;

    for (int v = 0; v < V; v++) {
        if (visited[v] == false) {
            vector<int> vec;
            components.push_back(vec);
            DFSUtil(v, visited);
            current_comp++;
        }
    }
}

void GComponent::DFSUtil(int v, bool visited[]) {
    visited[v] = true;
    components[current_comp].push_back(v);
    list<int>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i)
        if (!visited[*i])
            DFSUtil(*i, visited);
}

GComponent::GComponent(int V) {
    this->V = V;
    adj = new list<int>[V];
}

void GComponent::addEdge(int v, int w) {
    adj[v].push_back(w);
    adj[w].push_back(v);
}

int main() {
    int n;
    cin >> n;
    N = n;
    vector<pair<int, int>> edges;
    GBridges g1(n);
    int l, r;
    while (true) {
        cin >> l >> r;
        if (l == -1 && r == -1) break;
        pair<int, int> e;
        e.first = l;
        e.second = r;
        edges.push_back(e);
        g1.addEdge(l, r);
    }
    int s, d;
    cin >> s >> d;
    g1.bridge();

    GComponent wo_bridges(n);

    for (auto u : edges) {
        if (B.find(u) == B.end()) {
            wo_bridges.addEdge(u.first, u.second);
        }
    }
    wo_bridges.connectedComponents();
    unordered_map<int, int> nodeToComponent;
    int k = 0;
    for (vector<int> comp : components) {
        for (int i : comp) {
            nodeToComponent[i] = k;
        }
        k++;
    }

    if (nodeToComponent[s] == nodeToComponent[d]) {
        //cout << "true";
        int their_comp = nodeToComponent[s];
        Graph g1(components[their_comp].size());
        unordered_map<int, int> indexTOnode;
        unordered_map<int, int> nodeToIndex;
        unordered_set<int> completeComponent;
        int i = 0;
        for (int vert : components[their_comp]) {
            completeComponent.insert(vert);
            nodeToIndex[vert] = i;
            indexTOnode[i] = vert;
            i++;
        }

        for (auto u : edges) {
            if(completeComponent.find(u.first) != completeComponent.end() && completeComponent.find(u.second) != completeComponent.end()){
                //cout << "adding" << endl;
                g1.addEdge(nodeToIndex[u.first], nodeToIndex[u.second]);
                g1.addEdge(nodeToIndex[u.second], nodeToIndex[u.first]);
            }
        }

        g1.printAllPaths(nodeToIndex[s], nodeToIndex[d]);
        vector<int> output;
        for (auto u : allNodes) {
            output.push_back( indexTOnode[u]);
        }
        sort(output.begin(), output.end());
        for(int i = 0; i < output.size(); i++){
            cout << output[i]<< endl;
        }
        return 0;
    }

    Graph g(components.size());

    unordered_map<pair<int, int>, pair<int, int>, pair_hash> map_bridges;

    for (pair<int, int> b : B) {
        pair<int, int> i, j;
        i.first = j.second = nodeToComponent[b.first];
        j.first = i.second = nodeToComponent[b.second];
        map_bridges[i] = b;
        map_bridges[j] = b;
        g.addEdge(nodeToComponent[b.first], nodeToComponent[b.second]);
        g.addEdge(nodeToComponent[b.second], nodeToComponent[b.first]);

    }

    g.printAllPaths(nodeToComponent[s], nodeToComponent[d]);

    unordered_set<int> solution;
    for (auto u : visitedBridges) {
        pair<int, int> actual = map_bridges[u];
        if (actual.first != s && actual.first != d) {
            for (auto i : components[nodeToComponent[actual.first]])
                solution.insert(i);
        } else {
            solution.insert(actual.first);
        }
        if (actual.second != s && actual.second != d) {
            for (auto i : components[nodeToComponent[actual.second]])
                solution.insert(i);
        } else {
            solution.insert(actual.second);
        }

    }

    vector<int> output;

    for (auto i : solution) {
        output.push_back(i);
    }

    sort(output.begin(), output.end());
    for(int i = 0; i < output.size(); i++){
        cout << output[i] << endl;
    }


    return 0;
}

/*
25
0 1
1 2
2 3
3 4
4 5
5 6
6 7
1 8
8 11
8 9
9 10
11 12
9 12
12 13
12 15
12 14
14 19
14 18
14 17
17 18
18 19
13 15
15 16
16 20
16 21
20 21
21 24
22 24
22 23
10 22
10 23
-1 -1
10 16


 */
