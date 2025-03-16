#ifndef COMPLETE_GRAPH_H
#define COMPLETE_GRAPH_H

#include "Graph.h"
#include <random>

class CompleteGraph : public Graph {
    size_t numNodes;
    mutable std::mt19937 rng{std::random_device{}()};

public:
    explicit CompleteGraph(size_t n);
    std::pair<size_t, size_t> selectRandomEdge() override;
    bool hasEdges() const override;
};

#endif // COMPLETE_GRAPH_H