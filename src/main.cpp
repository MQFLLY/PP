#include "simulation/Simulator.h"
#include "protocol/KDivisionProtocol.h"
#include "graph/CompleteGraph.h"
#include "simulation/Evaluator.h"
#include <iostream>

void test_k_div_1() {
    auto start = std::chrono::high_resolution_clock::now();

    constexpr int max_n = 20;
    constexpr int max_k = 5;
    constexpr int trials = 100;

    ConvergenceEvaluator<KDivisionProtocolFactory> evaluator;

    for(int k = 3; k <= max_k; k++) {
        for(int n = k; n <= max_n; n += k) {
            for(int t = 0; t < trials; ++t) {
                evaluator.evaluate(n, k);
            }
        }
    }

    evaluator.printResults();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
   
    // print evalute time (ms)
    std::cout << "total cost: " << duration.count() << " ms" 
              << std::endl;
}

int main() {
    test_k_div_1();
    return 0;
}