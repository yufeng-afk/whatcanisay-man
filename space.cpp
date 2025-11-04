#include <bits/stdc++.h>
using namespace std;

int main() {
    long long n, m, a;
    if (!(cin >> n >> m >> a)) return 0;
    long long tiles_n = (n + a - 1) / a;
    long long tiles_m = (m + a - 1) / a;
    unsigned long long result = (unsigned long long)tiles_n * tiles_m;
    cout << result << '\n';
    return 0;
}1