#include <iostream>
#include <vector>
#include <random>
#include <chrono>

using namespace std;

// Usage: ./gen [n] [l] [r]
// Example: ./gen 10 1 100
int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Usage: " << argv[0] << " [n] [l] [r]" << endl;
        return 1;
    }

    // Parse arguments
    int n = stoi(argv[1]);
    long long l = stoll(argv[2]);
    long long r = stoll(argv[3]);

    // Seed with high precision clock
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

    // Define distribution
    uniform_int_distribution<long long> dist(l, r);

    cout << n << "\n";
    for (int i = 0; i < n; ++i) {
        cout << dist(rng) << (i == n - 1 ? "" : " ");
    }
    cout << "\n";

    return 0;
}
