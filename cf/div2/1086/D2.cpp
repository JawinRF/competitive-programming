#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> parent, size;
    int components;
    DSU(int n) {
        parent.resize(n);
        size.assign(n, 1);
        iota(parent.begin(), parent.end(), 0);
        components = n;
    }
    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }
    bool uni(int i, int j) {
        int ri = find(i), rj = find(j);
        if (ri != rj) {
            if (size[ri] < size[rj]) swap(ri, rj);
            parent[rj] = ri;
            size[ri] += size[rj];
            components--;
            return true;
        }
        return false;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;

        vector<vector<int>> m(n, vector<int>(n, 0));
        vector<vector<int>> adj(n);
        int diag = 0;

        for (int i = 0; i < n; i++) {
            string row;
            cin >> row;
            for (int j = 0; j < n; j++) {
                if (row[j] == '1') {
                    if (j != i) m[i][j] = 1;
                    else diag++;
                }
            }
        }

        if (diag != n) {
            cout << "No\n";
            continue;
        }

        int edge_count = 0;

        vector<pair<int,int>> sz;
        for (int i = 0; i < n; i++) {
            int cnt = 0;
            for (int j = 0; j < n; j++) if (m[i][j]) cnt++;
            sz.push_back({cnt, i});
        }

        sort(sz.rbegin(), sz.rend());

        for (int i = 0; i < n; i++) {
            int u = sz[i].second;
            vector<int> masked(n, 0);
            for (int j = i + 1; j < n; j++) {
                int v = sz[j].second;
                if (m[u][v] == 1 && !masked[v]) {
                    adj[u].push_back(v);
                    edge_count++;
                    if (edge_count >= n) {
                        cout << "No\n";
                        goto next_case;
                    }
                    for (int w = 0; w < n; w++) {
                        if (m[v][w] == 1) masked[w] = 1;
                    }
                }
            }
        }

        if (edge_count != n - 1 || diag != n) {
            cout << "No\n";
            continue;
        }

        {
            DSU dsu(n);
            for (int i = 0; i < n; i++) {
                for (int j : adj[i]) {
                    dsu.uni(i, j);
                }
            }
            if (dsu.components != 1) {
                cout << "No\n";
                continue;
            }
        }

        for (int i = 0; i < n; i++) {
            vector<int> visited(n, 0);
            queue<int> q;
            visited[i] = 1;
            q.push(i);

            while (!q.empty()) {
                int curr = q.front();
                q.pop();
                for (int j : adj[curr]) {
                    if (!visited[j]) {
                        visited[j] = 1;
                        q.push(j);
                    }
                }
            }

            for (int j = 0; j < n; j++) {
                if (i != j) {
                    if (visited[j] != m[i][j]) {
                        cout << "No\n";
                        goto next_case;
                    }
                }
            }
        }

        cout << "Yes\n";
        for (int i = 0; i < n; i++) {
            for (int j : adj[i]) {
                cout << i + 1 << " " << j + 1 << "\n";
            }
        }

        next_case:;
    }

    return 0;
}
