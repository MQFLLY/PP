// Evaluator.h
#include "util/ThreadPool.h"
#include "util/AvgResultsDatabaseManager.h"
#include "util/FullResultsDatabaseManager.h"
#include "protocol/KDivisionProtocol.h"
#include "protocol/RatioKDivisionProtocol.h"
#include "protocol/RatioKDivisionParaProtocol.h"
#include "protocol/Rule.h"
#include <map>
#include <atomic>
#include <functional>
#include <type_traits>
#include <spdlog/spdlog.h>

template <typename ProtocolFactory>
class ConvergenceEvaluator {
public:
    explicit ConvergenceEvaluator(size_t threads = 
        std::thread::hardware_concurrency(), 
        std::string db_path = "protocol_results.db",
        std::function<bool(int, int)> log_condition = [](int n, int k) {
            // return n >= 50 || k >= 5;
            return true;
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

    void evaluate(int n, int k, std::vector<int> ratio, int trials = 1) {
        auto protocol = factory.create(k, n, ratio);
        for (int t = 0; t < trials; ++t) {
            auto task = [this, n, k, t, ratio, protocol]() -> std::pair<int, RuleCountMap> {
                /*
                if (this->log_condition(n, k)) {
                    spdlog::info("starting {}th trial, n = {}, k = {}", t, n, k);
                }
                */
                auto graph = std::make_unique<CompleteGraph>(n);
                Simulator<decltype(protocol)> simulator(std::move(graph), 
                                                      protocol, n);
                return simulator.run();
            };

            auto future = pool.enqueue(task);
            
            {
                std::lock_guard<std::mutex> lock(result_mutex);
                ratio_results[{n, k, ratio, trials}].emplace_back(std::move(future));
            }
        }
    }

    void printResults() {
        if constexpr (std::is_same_v<ProtocolFactory, KDivisionProtocolFactory>) {
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
        } else if constexpr (std::is_same_v<ProtocolFactory, RatioKDivisionProtocolFactory> ||  std::is_same_v<ProtocolFactory, RatioKDivisionParaProtocolFactory>) {
            std::lock_guard<std::mutex> lock(result_mutex);
            for (auto& [params, futures] : ratio_results) {
                auto [n, k, ratio, trials] = params;
                
                long long total_steps = 0;
                std::unordered_map<RuleKey, long long, PairHash> rule_counter;
                int trial_count = 0;
                
                auto merge = [](std::unordered_map<RuleKey, long long, PairHash>& a, std::unordered_map<RuleKey, long long, PairHash>& b) {
                    for (auto& [k, v]: b) {
                        if (!a.count(k)) {
                            a[k] = v;
                        }
                        else {
                            a[k] += v;
                        }
                    }
                };
                
                for (auto& future : futures) {
                    auto res = future.get();
                    int steps = res.first;
                    auto rule_count = res.second;
                    merge(rule_counter, rule_count);
                    // full_db.saveFullResults(n, k, ratio, steps);
                    total_steps += steps;
                    ++trial_count;
                }
                
                long long tot = 0;
                for (auto& [k, v]: rule_counter) {
                    tot += v;
                }
                for (auto& [k, v]: rule_counter) {
                    if (k.first > k.second) {
                        std::cout << "miss: " << k.first << ' ' << k.second << '\n';
                        rule_counter[{k.second, k.first}] += v;
                        v = 0;
                    }
                }
                for (auto& [k, v]: rule_counter) {
                    if (v)
                        spdlog::info("rule freq: [{}, {}]: {}", k.first, k.second, (double)v / tot);
                }
                long long avg_steps = total_steps / trial_count;
                if (log_condition(n, k)) {
                    spdlog::info("[ratio protocol] n={} k={} | Trials: {} | Avg Steps: {}", 
                        n, k, trial_count, avg_steps);
                }
                db.saveAvgResults(n, k, ratio, trials, avg_steps);
            }
        }
    }
private:
    struct RatioParams {
        int n;
        int k;
        std::vector<int> ratio;
        int trials;
        bool operator < (const RatioParams& other) const {
            if (n != other.n) return n < other.n;
            if (k != other.k) return k < other.k;
            if (ratio != other.ratio) return ratio < other.ratio; 
            return trials < other.trials;
        }
    };
    ThreadPool pool;
    ProtocolFactory factory;
    AvgResultsDatabaseManager db;
    FullResultsDatabaseManager full_db;
    std::function<bool(int, int)> log_condition;
    std::map<std::tuple<int, int, int>, std::vector<std::future<int>>> results;
    std::map<RatioParams, std::vector<std::future<std::pair<int, RuleCountMap>>>> ratio_results;
    std::mutex result_mutex;
};