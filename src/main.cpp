#include "simulation/Simulator.h"
#include "protocol/KDivisionProtocol.h"
#include "graph/CompleteGraph.h"
#include "simulation/Evaluator.h"
#include <iostream>

int main() {
    constexpr int max_n = 20;
    constexpr int max_k = 5;
    constexpr int trials = 10;

    ConvergenceEvaluator<KDivisionProtocolFactory> evaluator;

    for(int k = 3; k <= max_k; k++) {
        for(int n = k; n <= max_n; n += k) {
            for(int t = 0; t < trials; ++t) {
                evaluator.evaluate(n, k);
            }
        }
    }

    evaluator.printResults();
    return 0;
}