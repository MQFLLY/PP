#include "simulation/Simulator.h"
#include "protocol/KDivisionProtocol.h"
#include "graph/CompleteGraph.h"
#include "simulation/Evaluator.h"
#include <iostream>
#include <chrono>

void test_k_div_1() {
    constexpr int max_n = 20;
    constexpr int max_k = 5;
    constexpr int trials = 100;

    ConvergenceEvaluator<KDivisionProtocolFactory> evaluator;

    for(int k = 3; k <= max_k; k++) {
        for(int n = k; n <= max_n; n += k) {
            auto start = std::chrono::high_resolution_clock::now();
            
            for(int t = 0; t < trials; ++t) {
                evaluator.evaluate(n, k);
            }
            
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> duration = end - start;
           
            // print evalute time (ms)
            std::cout << "n=" << n << ", k=" << k 
                      << ", trials=" << trials 
                      << ", total cost: " << duration.count() << " ms" 
                      << std::endl;
        }
    }

    evaluator.printResults();
}

int main() {
    test_k_div_1();
    return 0;
}