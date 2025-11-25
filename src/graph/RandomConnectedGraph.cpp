#include "graph/RandomConnectedGraph.h"
#include <iostream>

RandomConnectedGraph::RandomConnectedGraph(size_t n, size_t m): numNodes(n) {
    if (!(m >= n - 1))
        throw std::runtime_error("m >= n - 1 required!");
    for (int i = 1; i < n; i++) {
        size_t u = i + 1;
        std::uniform_int_distribution<size_t> dist(1, i);
        size_t v = dist(rng) ;
        std::cout << u << ' ' << v << '\n';
        edges.push_back({v, u});
    }
    std::uniform_int_distribution<size_t> dist(1, n);
    for (int i = 0; i < m - (n - 1); i++) {
        while (true) {
            size_t u = dist(rng);
            size_t v = dist(rng);
            while (v == u) {
                v = dist(rng);
            }
            if (v < u) {
                std::swap(u, v);
            }
            bool is_exist = false;
            for (const auto [fi, se]: edges) {
                if (fi == u && se == v) {
                    is_exist = true;
                    break;
                }
            }
            if (!is_exist) {
                std::cout << u << ' ' << v  << '\n';
                edges.push_back({u, v});
                break;
            }
        }
    }
}

bool RandomConnectedGraph::hasEdges() const {
    return numNodes >= 2;
}

std::pair<size_t, size_t> RandomConnectedGraph::selectRandomEdge() {
    /*
    std::cout << "debug edges ... " << std::endl;
    for (std::pair<size_t, size_t> p: edges) {
        std::cout << p.first << " " << p.second << '\n';
    }
    */

    std::uniform_int_distribution<size_t> dist(0, edges.size() - 1);
    size_t i = dist(rng);
    return { edges[i].first - 1, edges[i].second - 1};
}