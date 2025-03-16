#ifndef GRAPH_H
#define GRAPH_H

#include <utility>
#include <cstddef>

class Graph {
public:
    virtual ~Graph() = default;
    virtual std::pair<size_t, size_t> selectRandomEdge() = 0;
    virtual bool hasEdges() const = 0;
};

#endif // GRAPH_H