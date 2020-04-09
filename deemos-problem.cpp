#include<bits/stdc++.h>
using namespace std;

void reduce(vector<int>& diff){
    const int n = diff.size();
    int min = 1 << 20;
    for(int i = 0; i < n; i++) {
        if(diff[i] == 0) return;
        if (diff[i] < min) min = diff[i];
    }
    for(int i = 0; i < n; i++) diff[i] -= min;
}

int countSubarrays(vector<int>& arr, const int m) {
    vector<int> diff(m, 0);
    map<vector<int>, int> map;
    map[diff]++;
    int result = 0;
    const int n = arr.size();
    for (int i = 0; i < n; i++) {
        if(++diff[arr[i]] == 1) reduce(diff);
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
