#include <bits/stdc++.h>
using namespace std;
int count_factor(long long x, int p){
    if(x==0) return 0;
    int c=0;
    while(x%p==0){
        x/=p; c++;
    }
    return c;
}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if(!(cin>>n)) return 0;
    const int INF = 1000000000;
    vector<vector<int>> a2(n, vector<int>(n)), a5(n, vector<int>(n));
    int zx=-1, zy=-1;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            long long v; cin>>v;
            if(v==0){
                zx=i; zy=j;
                a2[i][j]=a5[i][j]=INF;
            } else {
                a2[i][j]=count_factor(v,2);
                a5[i][j]=count_factor(v,5);
            }
        }
    }
    // DP for 2s
    vector<vector<int>> dp2(n, vector<int>(n, INF)), dp5(n, vector<int>(n, INF));
    vector<vector<char>> p2(n, vector<char>(n,0)), p5(n, vector<char>(n,0));
    dp2[0][0]=a2[0][0];
    dp5[0][0]=a5[0][0];
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(i==0 && j==0) continue;
            // dp2
            if(i>0){
                int cand = dp2[i-1][j];
                if(cand + a2[i][j] < dp2[i][j]){
                    dp2[i][j] = cand + a2[i][j];
                    p2[i][j] = 'U';
                }
            }
            if(j>0){
                int cand = dp2[i][j-1];
                if(cand + a2[i][j] < dp2[i][j]){
                    dp2[i][j] = cand + a2[i][j];
                    p2[i][j] = 'L';
                }
            }
            // dp5
            if(i>0){
                int cand = dp5[i-1][j];
                if(cand + a5[i][j] < dp5[i][j]){
                    dp5[i][j] = cand + a5[i][j];
                    p5[i][j] = 'U';
                }
            }
            if(j>0){
                int cand = dp5[i][j-1];
                if(cand + a5[i][j] < dp5[i][j]){
                    dp5[i][j] = cand + a5[i][j];
                    p5[i][j] = 'L';
                }
            }
        }
    }
    int ans2 = dp2[n-1][n-1];
    int ans5 = dp5[n-1][n-1];
    int best = min(ans2, ans5);
    string path;
    if(zx!=-1 && best>1){
        // use zero cell to get product 0 -> one trailing zero
        cout<<1<<"\n";
        // path: go down zx times, right zy times to zero, then down remaining, right remaining
        path.append(zx, 'D');
        path.append(zy, 'R');
        path.append(n-1-zx, 'D');
        path.append(n-1-zy, 'R');
        cout<<path<<"\n";
        return 0;
    }
    // reconstruct from chosen DP
    cout<<best<<"\n";
    int i=n-1, j=n-1;
    if(ans2<=ans5){
        while(i>0 || j>0){
            if(p2[i][j]=='U'){
                path.push_back('D'); i--;
            } else {
                path.push_back('R'); j--;
            }
        }
    } else {
        while(i>0 || j>0){
            if(p5[i][j]=='U'){
                path.push_back('D'); i--;
            } else {
                path.push_back('R'); j--;
            }
        }
    }
    reverse(path.begin(), path.end());
    cout<<path<<"\n";
    return 0;
}