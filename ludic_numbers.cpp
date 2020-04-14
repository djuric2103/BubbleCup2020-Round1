#pragma GCC optimize ("O2")
#include <bits/stdc++.h>
using namespace std;

const int maxn = 5e7 + 17;
const int logn = 25;
int ludic[maxn / 64 + 10];
vector<int> offset;
int BIT_SIZE = maxn - 1;
int BIT[maxn];
vector<int> sol;
int greatest = 5e7;


void update(int x, int v) {
    while (x < BIT_SIZE) BIT[x] += v, x += x & -x;
}

int sum(int x) {
    int ret = 0;
    while (x) ret += BIT[x], x -= x & -x;
    return ret;
}

int query(int x) {
    int sum = 0, ret = 0;
    for (int i = logn; i >= 0; i--)
        if (ret + (1 << i) < BIT_SIZE && sum + BIT[ret + (1 << i)] < x)
            sum += BIT[ret + (1 << i)],
                    ret += 1 << i;
    return ++ret;
}

// Checks whether x is ludic
bool notLudic(int prime[], int x){
    return ( prime[x/64] & (1 << ((x >> 1) & 31)));
}

// Marks x not ludic
bool makeComposite(int prime[], int x){
    return prime[x/64] |= (1 << ((x >> 1) & 31));
}

int get_elem(int n, int j){
    //if(sum(n) < j) return n + 1;
    return query(j);
}

void bitWiseSieve(int n){
    update(1, 1);
    update(2, 1);
    sol.push_back(1);
    sol.push_back(2);
    for (int i = 3; i <= n; i+=2) update(i, 1);
    int curr = 2;
    int sum_n = -1;
    int left = sum(n);
    for (int i = 3; i <= n; i += 2) {
        if (!notLudic(ludic, i)) {
            curr++;
            sol.push_back(i);
            int max_k = i;// +  curr - 1;
            if(i <= n/20) {
                for (int j = curr + i; j < n; j += i) {
                    int k = get_elem(n, j);
                    if (k >= n + 1) break;
                    makeComposite(ludic, k);
                    j--;
                    update(k, -1);
                    left--;
                    max_k = k;
                }
                offset.push_back(sum(n) - sum(max_k));
                continue;
            }
            if(sum_n == -1) sum_n = sum(n);
            offset.push_back(sum_n - curr);
        }
    }

}

int binary(int q){
    int low = 0, high = sol.size() - 1, mid, ans = -1;
    while (low <= high) {
        mid = (low + high) / 2;

        if (sol[mid] <= q) ans = mid + 1, low = mid + 1;
        else high = mid - 1;
    }
    return ans;
}

int return_num(int n){
    int additional = n - greatest;
    int k = min(binary(n/20), (int)sol.size());
    //int cout = 0;
    for(int i = 1; i < k; i++){
        int toRem = (offset[i] + additional)/sol[i]; //+ ((offset[i] + additional) % sol[i] == 0 ? 1 : 0);
        //if(toRem == 0)cout++;
        //if(cout  == 10000)break;
        additional -= toRem;
    }
    return additional + sol.size();
}

void fastscan(int &number){
    /*register int c;
    number = 0;
    c = getchar();
    for (; (c>47 && c<58); c=getchar())
        number = number *10 + c - 48;
    */
    scanf("%d",&number);
}

int main(){
    ios_base::sync_with_stdio(false);
    int t;
    fastscan(t);
    int temp;
    offset.push_back(0);
    offset.push_back(greatest % 2);
    bitWiseSieve(greatest);
    while(t--){
        fastscan(temp);
        if(temp > greatest)
            printf("%d\n",return_num(temp));
        else
            printf("%d\n",binary(temp));
    }
    return 0;
}
