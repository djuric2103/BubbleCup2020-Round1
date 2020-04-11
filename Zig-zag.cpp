#include<bits/stdc++.h>
 
int main(){
    long long t, a, b;
    scanf("%lld", &t);
    while(t--){
        scanf("%lld%lld", &a, &b);
        if(a == b) printf("-1\n");
        else if(a > b) printf("1\n");
        else 
        	if(a %(b - a) == 0) printf("%lld\n", 2*(a/(b- a)) + 1);
            else printf("%lld\n", 2*(a/(b - a) + 1));
        
     }
     return 0;
}
