#include <bits/stdc++.h>
using namespace std;

string colToLetters(long long col) {
    string s;
    while (col > 0) {
        col--; 
        s.push_back(char('A' + (col % 26)));
        col /= 26;
    }
    reverse(s.begin(), s.end());
    return s;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;
    string token;
    while (n--) {
        cin >> token;
        bool isRxCy = false;
        if (!token.empty() && token[0] == 'R') {
            size_t posC = token.find('C', 1);
            if (posC != string::npos && posC > 1) {
                bool ok = true;
                for (size_t i = 1; i < posC; ++i) if (!isdigit((unsigned char)token[i])) { ok = false; break; }
                for (size_t i = posC + 1; i < token.size(); ++i) if (!isdigit((unsigned char)token[i])) { ok = false; break; }
                if (ok) isRxCy = true;
            }
        }

        if (isRxCy) {
            size_t posC = token.find('C', 1);
            long long row = stoll(token.substr(1, posC - 1));
            long long col = stoll(token.substr(posC + 1));
            cout << colToLetters(col) << row << '\n';
        } else {
            size_t i = 0;
            while (i < token.size() && isalpha((unsigned char)token[i])) ++i;
            string letters = token.substr(0, i);
            string rows = token.substr(i);
            long long col = 0;
            for (char c : letters) col = col * 26 + (c - 'A' + 1);
            cout << 'R' << rows << 'C' << col << '\n';
        }
    }
    return 0;
}