#include <bits/stdc++.h>

#define f first
#define s second

#define left(x) ((x) << 1)
#define right(x) ((x) << 1 | 1)
#define mid(x, y) ((x) + (y) >> 1)

using namespace std;

typedef pair<int, int> pii;

const int LOGN = 27;
const int maxk = 1 << LOGN;
/// const int maxn = 1e9;
const int maxq = 1e3 + 17;

int N, Q;
int in[maxq];
int BIT[maxk];
map<int, int> ludic;

void update(int x, int v) {
    while (x <= maxk) BIT[x] += v, x += x & -x;
}

int sum(int x) {
    int ret = 0;
    while (x) ret += BIT[x], x -= x & -x;
    return ret;
}

int sumr(int l, int r) {
    return sum(r) - sum(l - 1);
}

int query(int v) {
    int sum = 0, pos = 0;
    for (int i = LOGN; i >= 0; i--)
        if (pos + (1 << i) <= maxk && sum + BIT[pos + (1 << i)] < v) {
            sum += BIT[pos + (1 << i)];
            pos += 1 << i;
        }
    return ++pos;
}

int rem(int v) {
    int x = query(v);
    update(x, -1);
    return x;
}

int main()
{
    ios_base::sync_with_stdio(false);

    /// 15, 13
    /// 2, 14
    /// 15, 27
    /// 28, 40

    /**
    i ... 0, N - 2
    BIT ... 1, N - 1

    maxk = 15
    2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14
    2: 3, 5, 7, 9, 11, 13
    3: 5, 7, 11, 13
    5: 7, 11, 13
    7: 11, 13
    11: 13
    13: _

    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27
    2: 15, 17, 19, 21, 24, 25, 27
    3: 17, 19, 24, 25
    5: 17, 24, 25
    7: ...
    **/

    scanf("%d", &Q);
    for (int i = 1; i <= Q; i++) scanf("%d", in + i);

    int mxq = 0;
    for (int i = 1; i <= Q; i++) mxq = max(mxq, in[i]);

    int max_it = (mxq + maxk - 3) / (maxk - 2);
    for (int it = 0; it < max_it; it++) {
        if (it) memset(BIT, 0, sizeof BIT);
        for (int i = 0; i < maxk - 2; i++) update(1 + i, 1);

        if (it) {
            for (map<int, int>::iterator it = ludic.begin(); it != ludic.end(); it++) {
                int x = it->f, y = x - it->s - 1, last = -1;

                int total = sum(maxk - 2);
                if (y >= total) {
                    ludic[x] = it->s + total;
                    /// cout << x << " " << y << " " << total << " >< " << y - total << endl;
                    continue;
                }

                while (true) {
                    int clast = rem(y + 1);
                    if (clast > maxk) break;
                    last = clast + 1;
                    /// printf("HAVE %d; CUR %d; REM %d\n", x, y, last + 1 * (maxk - 2));
                    y += x - 1;
                }

                ludic[x] = sumr(last - 1, maxk - 2);
            }
        }

        int l = it * (maxk - 2), r = (it + 1) * (maxk - 2);
        for (int i = 0; i < maxk - 2; i++) if (sumr(1 + i, 1 + i)) {
            int x = i + 2 + l, y = 0, last = x;

            while (true) {
                int clast = rem(y + 1);
                if (clast > maxk) break;
                last = clast + 1;
                y += x - 1;
            }

            ludic[x] = sumr(last - 1, maxk - 2);
        }

        /// cout << endl << ludic.size() << endl;
        /// for (auto it = ludic.begin(); it != ludic.end(); it++) cout << it->f << " " << it->s << endl;
    }

    vector<int> v;
    v.push_back(1);
    for (map<int, int>::iterator it = ludic.begin(); it != ludic.end(); it++) v.push_back(it->f);
    for (int i = 1; i <= Q; i++) {
        int q = in[i];

        int low = 0, high = v.size() - 1, mid, ans = -1;
        while (low <= high) {
            mid = low + high >> 1;

            if (v[mid] <= q) ans = mid + 1, low = mid + 1;
            else high = mid - 1;
        }

        printf("%d\n", ans);
    }


    /// cout << endl << ludic.size() << endl;
    /// for (auto it = ludic.begin(); it != ludic.end(); it++) cout << it->f << " " << it->s << endl;

    return 0;
}
