#include "graph/CompleteGraph.h"

CompleteGraph::CompleteGraph(size_t n) : numNodes(n) {}

std::pair<size_t, size_t> CompleteGraph::selectRandomEdge() {
    std::uniform_int_distribution<size_t> dist(0, numNodes - 1);
    size_t u = dist(rng);
    size_t v;
    do {
        v = dist(rng);
    } while (u == v);
    return {u, v};
}

bool CompleteGraph::hasEdges() const {
    return numNodes >= 2;
}