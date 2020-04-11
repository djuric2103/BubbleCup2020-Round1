#include <bits/stdc++.h>

#define f first
#define s second

using namespace std;

typedef pair<int, int> pii;

const int maxn = 1e6 + 17;

int N, Q;
int in[maxn];

set<pii> s[maxn];
vector<int> v[maxn];

int main()
{
    ios_base::sync_with_stdio(false);

    cin >> N >> Q;
    for (int i = 1; i <= N; i++) cin >> in[i];
    for (int i = 1; i <= N; i++) v[in[i]].push_back(i), s[in[i]].insert({i, v[in[i]].size() - 1});

    while (Q--) {
        int L, R, P, K; cin >> L >> R >> P >> K;

        auto x = s[P].lower_bound({L, -1});
        if (x == s[P].end()) {
            cout << -1 << endl;
            continue;
        }

        int xx = x->s;
        if (xx + K - 1 >= v[P].size()) {
            cout << -1 << endl;
            continue;
        }

        int ans = v[P][xx + K - 1];
        if (ans > R) ans = -1;
        cout << ans << endl;
    }

    return 0;
}
