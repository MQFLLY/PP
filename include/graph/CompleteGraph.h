#ifndef COMPLETE_GRAPH_H
#define COMPLETE_GRAPH_H

#include "graph/Graph.h"
#include <random>

class CompleteGraph : public Graph {
public:
    explicit CompleteGraph(size_t n);
    std::pair<size_t, size_t> selectRandomEdge() override;
    bool hasEdges() const override;
private:
    size_t numNodes;
    mutable std::mt19937 rng{std::random_device{}()};
};

#endif // COMPLETE_GRAPH_H