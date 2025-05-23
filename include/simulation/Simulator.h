// Simulator.h
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <memory>
#include <vector>
#include <iostream>
#include "graph/Graph.h"
#include "protocol/BaseProtocol.h"
#include "agent/Agent.h"

template <typename Protocol>
class Simulator {
    std::unique_ptr<Graph> graph;
    Protocol protocol;
    std::vector<Agent<Protocol>> agents;
    size_t interaction_count = 0;

public:
    Simulator(std::unique_ptr<Graph> g, Protocol p, size_t numAgents)
        : graph(std::move(g)), protocol(std::move(p)), agents(numAgents) {
        protocol.initializeAgents(agents);
    }

    int run(size_t maxSteps = 1000000) {
        if (!graph->hasEdges()) {
            throw std::runtime_error("Graph has no edges");
        }

        for(interaction_count = 0; interaction_count < maxSteps; ++interaction_count) {
            if (protocol.isConverged(agents)) 
                return interaction_count;

            auto [a, b] = graph->selectRandomEdge();
            protocol.interact(agents[a], agents[b]);
        }
        return -1;
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