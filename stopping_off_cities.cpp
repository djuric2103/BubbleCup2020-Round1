#include <bits/stdc++.h> 
using namespace std; 
vector<bool> canReach(10000, false);
 
inline void addVertices(vector<int> v){ 
    for (int i = 0; i < v.size(); i++) canReach[v[i]] = true;
} 
  
bool notOnPath(int node, vector<int> v) { 
    for (int i = 0; i < v.size(); i++)  
        if (v[i] == node)  
            return false;  
    return true; 
} 
  
void findVertices(vector<vector<int>> adj, int s, int d) { 
    queue<vector<int>> que; 
    vector<int> path; 
    path.push_back(s); 
    que.push(path); 
    while (!que.empty()) { 
        path = que.front(); 
        que.pop(); 
        int current = path[path.size() - 1]; 
        if (current == d)  
            addVertices(path);         
        for (int i = 0; i < adj[current].size(); i++) { 
            if (notOnPath(adj[current][i], path)) { 
                if(canReach[adj[current][i]]){
                    addVertices(path);
                    continue;
                }
                vector<int> newP(path); 
                newP.push_back(adj[current][i]); 
                que.push(newP); 
            } 
        } 
    } 
} 
  
int main(){ 
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, l, r, s, d;
    cin >> n;
    vector<vector<int> > g(n); 
    while(true){
        cin >> l >> r;
        if(l == -1 && r == -1) break;
        g[l].push_back(r);
        g[r].push_back(l);
    }
    cin >> s >> d;
    findVertices(g, s, d); 
    for(int i = 0; i < n; i++)
        if(canReach[i])
            cout << i << '\n';
    return 0; 
}   
