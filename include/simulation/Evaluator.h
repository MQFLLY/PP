// Evaluator.h
#include "util/ThreadPool.h"
#include "util/AvgResultsDatabaseManager.h"
#include "util/FullResultsDatabaseManager.h"
#include <map>
#include <atomic>
#include <functional>
#include <spdlog/spdlog.h>

template <typename ProtocolFactory>
class ConvergenceEvaluator {
public:
    explicit ConvergenceEvaluator(size_t threads = 
        std::thread::hardware_concurrency(), 
        std::string db_path = "protocol_results.db",
        std::function<bool(int, int)> log_condition = [](int n, int k) {
            // return n >= 50 || k >= 5;
            return false;
        }
    ) : pool(threads), db(db_path), full_db(db_path), log_condition(log_condition) {}

    void evaluate(int n, int k, int trials = 1) {
        for (int t = 0; t < trials; ++t) {
            auto task = [this, n, k, t]() -> int {
                if (this->log_condition(n, k)) {
                    spdlog::info("starting {}th trial, n = {}, k = {}", t, n, k);
                }
                auto graph = std::make_unique<CompleteGraph>(n);
                auto protocol = factory.create(k);
                Simulator<decltype(protocol)> simulator(std::move(graph), 
                                                      std::move(protocol), n);
                return simulator.run();
            };

            auto future = pool.enqueue(task);
            
            {
                std::lock_guard<std::mutex> lock(result_mutex);
                results[{n, k, trials}].emplace_back(std::move(future));
            }
        }
    }

    void printResults() {
        std::lock_guard<std::mutex> lock(result_mutex);
        for (auto& [params, futures] : results) {
            auto [n, k, trials] = params;
            
            long long total_steps = 0;
            int trial_count = 0;
            
            for (auto& future : futures) {
                int steps = future.get();
                full_db.saveFullResults(n, k, steps);
                total_steps += steps;
                ++trial_count;
            }

            long long avg_steps = total_steps / trial_count;
            if (log_condition(n, k)) {
                spdlog::info("n={} k={} | Trials: {} | Avg Steps: {}", 
                     n, k, trial_count, avg_steps);
            }
            db.saveAvgResults(n, k, trials, avg_steps);
        }
    }
private:
    ThreadPool pool;
    ProtocolFactory factory;
    AvgResultsDatabaseManager db;
    FullResultsDatabaseManager full_db;
    std::function<bool(int, int)> log_condition;
    std::map<std::tuple<int, int, int>, std::vector<std::future<int>>> results;
    std::mutex result_mutex;
};