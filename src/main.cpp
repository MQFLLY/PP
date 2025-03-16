#include "Simulator.h"
#include "KDivisionProtocol.h"
#include "CompleteGraph.h"
#include <iostream>

int main() {
    constexpr int n = 40;
    constexpr int k = 4;

    auto graph = std::make_unique<CompleteGraph>(n);
    auto protocol = KDivisionProtocol(k);

    Simulator<KDivisionProtocol> simulator(std::move(graph), std::move(protocol), n);
    simulator.run();

    if (simulator.isConverged()) {
        std::cout << "Successfully divided into " << k << " groups!\n";
    } else {
        std::cout << "Failed to converge\n";
    }

    simulator.printStates();
    return 0;
}