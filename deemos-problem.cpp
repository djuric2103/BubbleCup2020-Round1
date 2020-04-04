#include<bits/stdc++.h>
using namespace std;

string give_string(int diff[], int m){
    ostringstream os;
    int min = diff[0];
    for(int i = 1; i< m;i++) {
        if (diff[i] < min)
            min = diff[i];
    }
    for(int i = 0; i < m; i++){
        diff[i] = diff[i] - min;
        os << diff[i] <<'\t';
    }
    return os.str();
}

int countSubarrays(std::vector<int> arr, int m) {
    vector<string> vec;
    int diff[m];
    for(int i = 0; i < m; i++)
        diff[i] = 0;

    vec.push_back(give_string(diff, m));
    for (int i = 0; i < arr.size(); i++) {
        diff[arr[i] - 1]++;
        vec.push_back(give_string(diff, m));
    }

    std::map<string, int> map;
    int result = 0;
    for (string x : vec) {
        result += map[x];
        map[x]++;
    }
    return result;
}

int main(void) {
    int n, m;
    cin >> n>> m;
    vector<int> arr;
    int t;
    for(int i = 0; i < n; i++){
        cin>>t;
        arr.push_back(t);
    }
    cout << countSubarrays(arr, m) << endl;
    return 0;
}