#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#define greatest 43956562
#define limit 2275147
#define logn 25
using namespace std;
const int maxn = greatest + 2;
vector<int> ludic(maxn / 64 + 2), offset(limit), sol(limit), BIT(maxn);
const int BIT_SIZE = maxn - 1;
const int sol_size = limit;
int k = limit, previous = greatest, prev_sol = sol_size;
unordered_map<int, int> a;

inline int sum(int x) {
    int ret = 0;
    while (x) ret += BIT[x], x -= x & -x;
    return ret;
}

void bitWiseSieve(const int n) {
    for (int i = 1; i <= n; ++i){
        BIT[i] = i & -i;
        if(i%2 == 0) BIT[i] /= 2;
    }
    for (int i = 0; i <= logn; ++i) ++BIT[1 << i];
    sol[0] = 1, sol[1] = 2, offset[0] = 0, offset[1] = 0;
    int curr = 2,left = sum(n), i;
    for (i = 3; i <= 2142957; i += 2) {
        if (!(ludic[i / 64] & (1 << ((i >> 1) & 31)))) {
            ++curr;
            sol[curr - 1] = i;
            int s = curr;
            for (int j = curr + i; j < left + 1; j += i - 1) {
                int sums = 0, k = 0;
                for (int m = logn; m >= 0; --m) {
                    if (k + (1 << m) < BIT_SIZE && sums + BIT[k + (1 << m)] < j)
                        sums += BIT[k + (1 << m)],
                                k += 1 << m;
                }
                ++k;
                if (k >= n) break;
                ludic[k / 64] |= (1 << ((k >> 1) & 31));
                --left;
                s += (i - 1);
                while (k < BIT_SIZE) BIT[k] -= 1, k += k & -k;
            }
            offset[curr - 1] = left - s;
        }
    }
    int sum_n = sum(n);
    for (; i <= n; i += 2) {
        if (!(ludic[i / 64] & (1 << ((i >> 1) & 31)))) {
            curr++;
            sol[curr - 1] = i;
            offset[curr - 1] = sum_n - curr;
        }
    }
}

inline int binary(const int q) {
    int low = 0, high = sol_size - 1, mid, ans = -1;
    while (low <= high) {
        mid = (low + high) / 2;
        if (sol[mid] <= q) ans = mid + 1, low = mid + 1;
        else high = mid - 1;
    }
    return ans;
}



inline int return_num(const int n, const vector<int>& sol1) {
    if (a.find(n) != a.end()) {
        return a.find(n)->second;
    }
    int additional = n - previous;

    int toRem;
    for (int i = 1; i < k; ++i) {
        toRem = (offset[i] + additional) / sol1[i];
        offset[i] = (offset[i]+additional) % sol1[i];
        additional -= toRem;
        if(additional == 0) break;
    }

    const int res = additional + prev_sol;
    prev_sol = res;
    previous = n;
    if (n % 2 == 1) {
        a.insert({n +1, res});
    }
    return res;
}

inline int return_num1(const int n, const vector<int>& sol1) {
    if (a.find(n) != a.end()) {
        return a.find(n)->second;
    }
    int additional = n - greatest;
    int cout = 0;
    int toRem;
    for (int i = 1; i < k; ++i) {
        toRem = (offset[i] + additional) / sol1[i];
        cout = toRem == 0 ? cout + 1 : 0;
        additional -= toRem;
    }
    k -= cout;
    const int res = additional + sol_size;
    if (n % 2 == 0) {
        a.insert({n - 1, res});
    }
    return res;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t;
    scanf("%d", &t);
    bitWiseSieve(greatest);
    const vector<int> sol1(sol);
    vector<int> qu(t);
    vector<int> so(t);
    for (int i = 0; i < t; ++i) {
        int temp;
        scanf("%d", &temp);
        qu[i] = temp;
        so[i] = temp;
    }
    sort(so.begin(), so.end());
    if(so[t - 1] - so[t/2] < 1e6) {
        for (int i = 0; i < t; ++i) {
            if (so[i] > greatest)a.insert({so[i], return_num(so[i], sol1)});
            else a.insert({so[i], binary(so[i])});
        }
    }else{
        for (int i = t - 1; i >= 0; --i) {
            if (so[i] > greatest)a.insert({so[i], return_num1(so[i], sol1)});
            else a.insert({so[i], binary(so[i])});
        }
    }
    for (int i = 0; i < t; ++i) {
        printf("%d\n", a.find(qu[i])->second);
    }
    return 0;
}
