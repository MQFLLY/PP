#include "simulation/Simulator.h"
#include "protocol/KDivisionProtocol.h"
#include "graph/CompleteGraph.h"
#include "simulation/Evaluator.h"
#include <iostream>
#include <spdlog/spdlog.h>
#include <gflags/gflags.h>

DEFINE_int32(num_agents, 100, "Number of agents");

void test_k_div_1(int num_agents) {
    auto start = std::chrono::high_resolution_clock::now();

    int max_n = num_agents;
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

int main(int argc, char* argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    spdlog::info("Welcome to PopulationProtocol! SPDLOG v{}", SPDLOG_VERSION);
    test_k_div_1(FLAGS_num_agents);
    gflags::ShutDownCommandLineFlags();
    return 0;
}