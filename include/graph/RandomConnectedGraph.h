#ifndef RANDOM_CONNECTED_GRAPH_H
#define RANDOM_CONNECTED_GRAPH_H

#include "graph/Graph.h"
#include <random>
#include <utility>
#include <memory>

class RandomConnectedGraph : public Graph {
public:
    explicit RandomConnectedGraph(size_t n, size_t m);
    std::pair<size_t, size_t> selectRandomEdge() override;
    bool hasEdges() const override;
private:
    size_t numNodes;
    std::vector<std::pair<size_t, size_t>> edges; 
    mutable std::mt19937 rng{std::random_device{}()};
};

#endif // RANDOM_CONNECTED_GRAPH_H 