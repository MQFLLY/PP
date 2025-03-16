// Simulator.h
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <memory>
#include <vector>
#include <iostream>
#include "Graph.h"
#include "BaseProtocol.h"
#include "Agent.h"

template <typename Protocol>
class Simulator {
    std::unique_ptr<Graph> graph;
    Protocol protocol;
    std::vector<Agent<Protocol>> agents;

public:
    Simulator(std::unique_ptr<Graph> g, Protocol p, size_t numAgents)
        : graph(std::move(g)), protocol(std::move(p)), agents(numAgents) {
        protocol.initializeAgents(agents);
    }

    void run(int maxSteps = 100000) {
        if (!graph->hasEdges()) {
            throw std::runtime_error("Graph has no edges");
        }

        for (int i = 0; i < maxSteps; ++i) {
            if (protocol.isConverged(agents)) break;

            auto [a, b] = graph->selectRandomEdge();
            protocol.interact(agents[a], agents[b]);
        }
    }

    bool isConverged() const {
        return protocol.isConverged(agents);
    }

    void printStates() const {
        for (size_t i = 0; i < agents.size(); ++i) {
            const auto* s = dynamic_cast<const typename ProtocolTraits<Protocol>::StateType*>(agents[i].getState());
            std::cout << "Agent " << i << ": "
                      << (s ? s->getValue() : "INVALID") << "\n";
        }
    }
};

#endif