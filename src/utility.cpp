//
// Created by IscG on 10/01/2022.
//

#include "utility.h"
#ifdef DEBUG
#include <cstdio>
#endif

static unsigned long x = 12345689, y = 362436069, z = 521288629;
unsigned long xorshf96() {
    unsigned long t;
    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;
    t = x;
    x = y;
    y = z;
    z = t ^ x ^ y;
    return z;
}

int randinteger(int x) {
    return static_cast<int>(xorshf96()%static_cast<unsigned long>(x));
}

double randreal() {
    return static_cast<double>(static_cast<long double>(xorshf96())/static_cast<long double>(ULONG_MAX));
}

std::vector<std::pair<int, int>> randomlyPair(int n, int k) {
    std::vector<std::pair<int, int>> pairs;
    std::vector<int> availablePlayers(n);
    for (int i = 0; i < n; i++) availablePlayers[i] = i;
    std::vector<int> availablePairs{};

    int idx1, idx2, p1, p2, det, candidate, nPlayers;
    // Vertex i is connected to vertices (i+j)%n and (i-j)%n for 1 <= j <= k
    while (availablePlayers.size() > 1) {
        nPlayers = availablePlayers.size();

#ifdef DEBUG
        printf("1: %d\n", nPlayers);
#endif
        // Select a random player not already assigned
        idx1 = randinteger(nPlayers);
        p1 = availablePlayers[idx1];
        // Determine which pairs are possible
        availablePairs.clear();
        for (int i = 1; i <= k; i++) {
            candidate = (idx1+i)%nPlayers;
            det = availablePlayers[candidate] - p1;
            if (k >= det && det > 0 || det < 0 && det+n <= k)
                availablePairs.push_back(candidate);
            candidate = (idx1-i)%nPlayers;
            while (candidate < 0) candidate += nPlayers;
            det = p1 - availablePlayers[candidate];
            if (k >= det && det > 0 || det < 0 && det+n <= k)
                availablePairs.push_back(candidate);
        }

#ifdef DEBUG
        printf("2\n");
#endif
        if (availablePairs.empty()) {
            availablePlayers.erase(availablePlayers.begin()+idx1);
        } else {
            idx2 = availablePairs[randinteger(availablePairs.size())];
            p2 = availablePlayers[idx2];
#ifdef DEBUG
            printf("3: %d\n", idx2);
#endif
            availablePlayers.erase(availablePlayers.begin()+idx1);
            availablePlayers.erase(availablePlayers.begin()+(idx2 > idx1 ? idx2-1 : idx2));
#ifdef DEBUG
            printf("4: %d, %d\n", p1, p2);
#endif
            pairs.emplace_back(p1, p2);
        }
    }
    return pairs;
}

void display(int n, int k) {
    printf("\n%d, %d: \n", n, k);
    for (auto& p : randomlyPair(n, k)) {
        printf("%d, %d\n", p.first, p.second);
    }
}

