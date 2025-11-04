#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    vector<pair<string,int>> rounds;
    rounds.reserve(n);
    for (int i = 0; i < n; ++i) {
        string name;
        int score;
        cin >> name >> score;
        rounds.emplace_back(name, score);
    }

    unordered_map<string, long long> total;
    for (auto &r : rounds) total[r.first] += r.second;

    long long maxScore = LLONG_MIN;
    for (auto &p : total) if (p.second > maxScore) maxScore = p.second;

    unordered_set<string> candidates;
    for (auto &p : total) if (p.second == maxScore) candidates.insert(p.first);

    unordered_map<string, long long> running;
    for (auto &r : rounds) {
        running[r.first] += r.second;
        if (candidates.count(r.first) && running[r.first] >= maxScore) {
            cout << r.first << '\n';
            return 0;
        }
    }

    return 0;
}