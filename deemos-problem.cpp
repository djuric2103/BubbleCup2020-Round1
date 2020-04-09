#include<bits/stdc++.h>
using namespace std;

void reduce(vector<int>& diff){
    const int m = diff.size();
    int min = diff[0];
    for(int i = 1; i< m;i++) {
        if(diff[i] == 0) return;
        if (diff[i] < min) min = diff[i];
    }
    for(int i = 0; i < m; i++) diff[i] = diff[i] - min;
}

int countSubarrays(vector<int> arr, int m) {
    vector<int> diff(m, 0);
    std::map<vector<int>, int> map;

    map[diff]++;

    int result = 0;
    const int n = arr.size();
    for (int i = 0; i < n; i++) {
        diff[arr[i] - 1]++;
        reduce(diff);
        result += map[diff]++;
    }
    return result;
}

int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    cin >> n>> m;
    vector<int> arr;
    int t;
    for(int i = 0; i < n; i++){
        cin>>t;
        arr.push_back(t);
    }
    cout << countSubarrays(arr, m) << "\n";
    return 0;
}
