#include<bits/stdc++.h>

using namespace std;

int main(){
    long long t, a, b;
    cin >> t;
    while(t--){
        cin >> a >> b;
        if(a == b) cout << "-1\n";
        else if(a > b) cout << "1\n";
        else{
            if(a %(b - a) == 0){
                cout << (2*(a/(b- a)) + 1) << '\n';
            }else{
                cout << (2*(a/(b - a) + 1)) << '\n';
            }
        }
     }
     return 0;
}
