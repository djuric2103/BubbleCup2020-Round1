#include <bits/stdc++.h>
 
#define f first
#define s second
#define pb push_back
 
using namespace std;
 
typedef long long ll;
typedef long double ld;
typedef pair<ld, int> pdi;
typedef pair<int, int> pii;
 
const ld  p    = (ld) 0.17;
const int nbs  = 10;
const int maxn = 1e4 + 17;
 
int Q;
int N, M, K, L;
int pos[2][maxn];
int link[2][maxn];
 
inline ld dist(int i, int j) {
    return sqrt((pos[0][i] - pos[0][j]) * (pos[0][i] - pos[0][j]) + (pos[1][i] - pos[1][j]) * (pos[1][i] - pos[1][j]));
}
 
inline bool ccw(int i, int j, int k) {
    return (pos[1][k] - pos[1][i]) * (pos[0][j] - pos[0][i]) > (pos[1][j] - pos[1][i]) * (pos[0][k] - pos[0][i]);
}
 
inline bool intersect(int i, int j) {
    return ccw(link[0][i], link[0][j], link[1][j]) != ccw(link[1][i], link[0][j], link[1][j]) && ccw(link[0][i], link[1][i], link[0][j]) != ccw(link[0][i], link[1][i], link[1][j]);
}
 
int par[maxn];
 
int find(int x) {
    if (x == par[x]) return x;
    return par[x] = find(par[x]);
}
 
inline bool merge(int x, int y) {
    if ((x = find(x)) == (y = find(y))) return false;
    par[x] = y;
    return true;
}
 
int main()
{
    ios_base::sync_with_stdio(false);
    srand(time(NULL));
 
    cin >> Q;
    for (int q = 1; q <= Q; q++) {
        cin >> N >> M >> K >> L;
        for (int i = 1; i <= N; i++) cin >> pos[0][i] >> pos[1][i];
        for (int i = 1; i <= M; i++) cin >> link[0][i] >> link[1][i];
 
        vector<pdi> v;
        for (int i = 1; i <= M; i++) {
            ld d = K * dist(link[0][i], link[1][i]);
            for (int j = 1; j <= M; j++) if (intersect(i, j)) d += L / 2 + L % 2;
            v.push_back({d, i});
        }
 
        for (int i = 1; i <= N; i++) par[i] = i;
 
        vector<int> ans;
        sort(v.begin(), v.end());
        for (int i = 0; i < v.size(); i++) {
            int x = v[i].second;
            if (merge(link[0][x], link[1][x])) ans.push_back(x);
        }
 
 
        cout << "case " << q << " Y" << endl;
        for (int x : ans) cout << x << " ";
        cout << endl;
    }
 
    return 0;
} 
