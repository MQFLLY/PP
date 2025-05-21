#include "simulation/Simulator.h"
#include "protocol/KDivisionProtocol.h"
#include "graph/CompleteGraph.h"
#include "simulation/Evaluator.h"
#include <iostream>
#include <spdlog/spdlog.h>

void test_k_div_1() {
    auto start = std::chrono::high_resolution_clock::now();

    constexpr int max_n = 20;
    constexpr int max_k = 5;
    constexpr int trials = 100;

    ConvergenceEvaluator<KDivisionProtocolFactory> evaluator;

    for(int k = 3; k <= max_k; k++) {
        for(int n = k; n <= max_n; n += k) {
            evaluator.evaluate(n, k, trials);
        }
    }

    evaluator.printResults();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
   
    // print evalute time (ms)
    spdlog::info("total cost: {} ms", duration.count());
}

int main() {
    spdlog::info("Welcome to PopulationProtocol! SPDLOG v{}", SPDLOG_VERSION);
    test_k_div_1();
    return 0;
}