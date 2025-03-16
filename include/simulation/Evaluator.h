// Evaluator.h
#include "util/ThreadPool.h"
#include <map>
#include <atomic>

template <typename ProtocolFactory>
class ConvergenceEvaluator {
    ThreadPool pool;
    ProtocolFactory factory;
    std::map<std::pair<int, int>, std::future<int>> results;
    std::mutex result_mutex;

public:
    explicit ConvergenceEvaluator(size_t threads = 4) : pool(threads) {}

    void evaluate(int n, int k) {
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
            results[{n, k}] = std::move(future);
        }
    }

    void printResults() {
        std::lock_guard<std::mutex> lock(result_mutex);
        for(auto& [params, future] : results) {
            auto [n, k] = params;
            int steps = future.get();
            std::cout << "n=" << n << " k=" << k 
                      << " converged in " << steps << " steps\n";
        }
    }
};