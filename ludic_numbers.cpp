#include <bits/stdc++.h>
using namespace std;
const int maxn = 2e7 + 17;
const int logn = log(maxn) + 2;
int prime[maxn/64 + 10];
int BIT_SIZE = maxn - 1;
int BIT[maxn];

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

bool notLudic(int prime[], int x){
    return ( prime[x/64] & (1 << ((x >> 1) & 31)));
}

bool setNotLudic(int *prime, int x){
    prime[x/64] |= (1 << ((x >> 1) & 31));
}

int get_ind1(int arr[], int n, int j, int k){
    int counter = 0;
    for(int i = j + 2; i < n; i += 2){
        if(!notLudic(arr, i)){
            counter++;
            if(counter == k){
                return i;
            }
        }
    }
    return n;
}

int get_elem(int n, int j){
    if(sum(n) < j) return n + 1;
    int temp = query(j);
    update(temp, -1);
    return temp;
}


void initialize(int n){
    update(1, 1);
    update(2, 1);
    for (int i = 3; i <= n; i+=2) update(i, 1);
}


void sieve(int n){
    memset(prime, 0, sizeof(prime));
    int curr = 2;
    for (int i = 3; i <= n/2; i += 2) {
        if (!notLudic(prime, i)) {
            curr++;
            for (int j = curr + i; j < n; j += i) {
                int k = get_elem(n,j);
                if(k == n + 1) break;
                setNotLudic(prime, k);
                j--;
            }
        }
    }
}

int main(){
    int t;
    cin >> t;
    int greatest = 0;
    int temp;
    vector<int> test;
    while(t--){
        cin >> temp;
        if(temp > greatest) greatest =  temp;
        test.push_back(temp);
    }
    if (greatest > maxn - 17) throw "asdasd";
    initialize(greatest);
    sieve(greatest);
    for(int u : test){
        cout << sum(u) << endl;
    }
    return 0;
} 