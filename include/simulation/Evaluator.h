// Evaluator.h
#include "util/ThreadPool.h"
#include <map>
#include <atomic>

template <typename ProtocolFactory>
class ConvergenceEvaluator {
    ThreadPool pool;
    ProtocolFactory factory;

    std::map<std::pair<int, int>, std::vector<std::future<int>>> results;
    std::mutex result_mutex;

public:
    explicit ConvergenceEvaluator(size_t threads = 4) : pool(threads) {}

    void evaluate(int n, int k, int trials = 1) {
        for (int t = 0; t < trials; ++t) {
            auto task = [this, n, k]() -> int {
                auto graph = std::make_unique<CompleteGraph>(n);
                auto protocol = factory.create(k);
                Simulator<decltype(protocol)> simulator(std::move(graph), 
                                                      std::move(protocol), n);
                return simulator.run();
            };

            auto future = pool.enqueue(task);
            
            {
                std::lock_guard<std::mutex> lock(result_mutex);
                results[{n, k}].emplace_back(std::move(future));
            }
        }
    }

    void printResults() {
        std::lock_guard<std::mutex> lock(result_mutex);
        for (auto& [params, futures] : results) {
            auto [n, k] = params;
            
            long long total_steps = 0;
            int trial_count = 0;
            
            for (auto& future : futures) {
                total_steps += future.get();
                ++trial_count;
            }

            long long avg_steps = total_steps / trial_count;
            std::cout << "n=" << n << " k=" << k 
                     << " | Trials: " << trial_count
                     << " | Avg Steps: " << avg_steps << "\n";
        }
    }
};