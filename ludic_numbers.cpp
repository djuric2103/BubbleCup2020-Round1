#include <bits/stdc++.h>
#define greatest 43956562
#define limit 2275147
#define logn 25
#define lastRem 2142957
using namespace std;
const int maxn = greatest + 2, BIT_SIZE = maxn - 1;
int ludic[maxn / 64 + 10], offset[limit], BIT[maxn], sol[limit];
int k = limit, sol_size = limit, previous = greatest, prev_sol = sol_size;
unordered_map<int, int> a;

inline void update(int x, const int v) {
    while (x < BIT_SIZE) BIT[x] += v, x += x & -x;
}

inline int sum(int x) {
    int ret = 0;
    while (x) ret += BIT[x], x -= x & -x;
    return ret;
}

inline int query(const int x) {
    int sum = 0, ret = 0;
    for (int i = logn; i >= 0; --i)
        if (ret + (1 << i) < BIT_SIZE && sum + BIT[ret + (1 << i)] < x)
            sum += BIT[ret + (1 << i)],
                    ret += 1 << i;
    return ++ret;
}

void bitWiseSieve(const int n){
    for (int i = 1; i <= n; ++i) BIT[i] = i & -i;
    for (int i = 2; i <= n; i += 2) BIT[i] /= 2;
    for (int i = 0; i <= logn; ++i) BIT[1 << i]++;
    sol[0] = 1, sol[1] = 2, offset[0] = 0, offset[1] = 0;
    int sum_n = -1, curr = 2, left = sum(n);

    for (int i = 3; i <= n; i += 2) {
        if (!(ludic[i/64] & (1 << ((i >> 1) & 31)))) {
            ++curr;
            sol[curr - 1] = i;
            if(i <= lastRem) {
                int s = curr;
                for (int j = curr + i; j < left + 1; j += i-1) {
                    int k = query(j);
                    if (k >= n + 1) break;
                    ludic[k/64] |= (1 << ((k >> 1) & 31));
                    --left;
                    s += (i - 1);
                    update(k, -1);
                }
                offset[curr - 1] = left - s;
                continue;
            }
            if(sum_n == -1) sum_n = sum(n);
            offset[curr - 1] = sum_n - curr;
        }
    }
}

inline int binary(int q){
    int low = 0, high = sol_size - 1, mid, ans = -1;
    while (low <= high) {
        mid = (low + high) / 2;
        if (sol[mid] <= q) ans = mid + 1, low = mid + 1;
        else high = mid - 1;
    }
    return ans;
}

int search_incr(const int n) {
    if (a.find(n) != a.end()) {
        return a.find(n)->second;
    }
    int additional = n - previous;

    int toRem;
    for (int i = 1; i < k; ++i) {
        toRem = (offset[i] + additional) / sol[i];
        offset[i] = (offset[i]+additional) % sol[i];
        additional -= toRem;
        if(additional == 0) break;
    }

    int res = additional + prev_sol;
    prev_sol = res;
    previous = n;
    if (n % 2 == 1) {
        a.insert({n +1, res});
    }
    return res;
}

int search_dec(const int n) {
    if (a.find(n) != a.end()) {
        return a.find(n)->second;
    }
    int additional = n - greatest;
    int cout = 0;
    int toRem;
    for (int i = 1; i < k; ++i) {
        toRem = (offset[i] + additional) / sol[i];
        cout = toRem == 0 ? cout + 1 : 0;
        additional -= toRem;
    }
    k -= cout;
    int res = additional + sol_size;
    if (n % 2 == 0) {
        a.insert({n - 1, res});
    }
    return res;
}


int main(){
    int t;
    scanf("%d",&t);
    bitWiseSieve(greatest);

    vector<int> qu(t), so(t);
    for(int i = 0; i < t; ++i){
        int temp;
        scanf("%d",&temp);
        qu[i] = temp;
        so[i] = temp;
    }

    sort(so.begin(), so.end());

    if(so[t - 1] - so[t/2] < 1e6) {
        for (int i = 0; i < t; ++i) {
            if (so[i] > greatest)a.insert({so[i], search_incr(so[i])});
            else a.insert({so[i], binary(so[i])});
        }
    }else{
        for (int i = t - 1; i >= 0; --i) {
            if (so[i] > greatest)a.insert({so[i], search_dec(so[i])});
            else a.insert({so[i], binary(so[i])});
        }
    }
    for(int  i = 0; i < t; ++i){
        printf("%d\n", a.find(qu[i]) -> second);
    }

    return 0;
}
