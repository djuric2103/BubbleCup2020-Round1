#include<bits/stdc++.h>
#define MAX 1e9
using namespace std;


inline int reduce(vector<int>& diff){
    int pos = 0;
    for(int i = 0; i < diff.size(); i++) if(--diff[i] > 0) pos++;  
    return pos;
}

int countSubarrays(vector<int>& arr, const int m) {
    vector<int> diff(m, 0);
    map<vector<int>, int> map;
    map[diff]++;
    int result = 0;
    const int n = arr.size();
    int pos = 0;
    for (int i = 0; i < n; i++) {
        if(++diff[arr[i]] == 1){ 
            pos++;
            if(pos == m) pos = reduce(diff);
        }
        result += map[diff]++;
    }
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    cin >> n>> m;
    vector<int> arr(n);
    int t;
    for(int i = 0; i < n; i++){
        cin>>t;
        arr[i] = t - 1;
    }
    cout << countSubarrays(arr, m) << "\n";
    return 0;
}
