#include "simulation/Simulator.h"
#include "protocol/KDivisionProtocol.h"
#include "protocol/RatioKDivisionProtocol.h"
#include "graph/CompleteGraph.h"
#include "simulation/Evaluator.h"
#include <iostream>
#include <spdlog/spdlog.h>
#include <gflags/gflags.h>

DEFINE_int32(min_num_agents, 3, "Minimum number of agents");
DEFINE_int32(max_num_agents, 50, "Maximum number of agents");
DEFINE_int32(min_k, 3, "Minimum k");
DEFINE_int32(max_k, 5, "Maximum k");

void test_k_div_1(int min_n, int max_n,
  int min_k, int max_k) {
    auto start = std::chrono::high_resolution_clock::now();

    constexpr int trials = 100;

    ConvergenceEvaluator<KDivisionProtocolFactory> evaluator;

    for(int k = min_k; k <= max_k; k++) {
        for(int n = std::max(k, min_n); n <= max_n; n += k) {
            evaluator.evaluate(n, k, trials);
        }
    }

    evaluator.printResults();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
   
    // print evalute time (ms)
    double duration_time = duration.count();
    if (duration_time > 1000) {
        duration_time /= 1000; 
        spdlog::info("total cost: {} s", duration_time);
    }
    else {
        spdlog::info("total cost: {} ms", duration_time);
    }
}

void test_k_div_2(int min_n, int max_n,
    int min_k, int max_k, const std::vector<int>& ratio) {
      auto start = std::chrono::high_resolution_clock::now();
  
      constexpr int trials = 1;
  
      ConvergenceEvaluator<RatioKDivisionProtocolFactory> evaluator;
  
      for(int k = min_k; k <= max_k; k++) {
          for(int n = std::max(k, min_n); n <= max_n; n += k) {
              evaluator.evaluate(n, k, ratio, trials);
          }
      }
  
      evaluator.printResults();
  
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::milli> duration = end - start;
     
      // print evalute time (ms)
      double duration_time = duration.count();
      if (duration_time > 1000) {
          duration_time /= 1000; 
          spdlog::info("total cost: {} s", duration_time);
      }
      else {
          spdlog::info("total cost: {} ms", duration_time);
      }
  }


int main(int argc, char* argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    spdlog::info("Welcome to PopulationProtocol! SPDLOG v{}", SPDLOG_VERSION);

    /*
    test_k_div_1(FLAGS_min_num_agents, 
                 FLAGS_max_num_agents,
                 FLAGS_min_k,
                 FLAGS_max_k);
    */

    test_k_div_2(9, 
                 9,
                 3,
                 3,
                 {1, 3, 5});

    gflags::ShutDownCommandLineFlags();
    return 0;
}