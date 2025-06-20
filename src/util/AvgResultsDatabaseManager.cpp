#include "util/AvgResultsDatabaseManager.h"
#include "duckdb/common/types/value.hpp"
#include "duckdb/common/vector.hpp" 

AvgResultsDatabaseManager::AvgResultsDatabaseManager(const std::string& db_path) 
    : DatabaseManager(db_path) {
    con.Query(R"(
        CREATE TABLE IF NOT EXISTS protocol_results (
            n INTEGER,
            k INTEGER,
            trials INTEGER,
            avg_steps INTEGER,
            timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        )
    )");
    con.Query(R"(
        CREATE TABLE IF NOT EXISTS ratio_protocol_results (
            n INTEGER,
            k INTEGER,
            ratio INTEGER[],
            trials INTEGER,
            avg_steps INTEGER,
            timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        )
    )");
}

void AvgResultsDatabaseManager::saveAvgResults(int n, int k, int trials, int64_t avg_steps) {
    std::lock_guard<std::mutex> lock(db_mutex);
    auto query = con.Prepare(R"(
        INSERT INTO protocol_results (n, k, trials, avg_steps)
        VALUES (?, ?, ?, ?)
    )");
    query->Execute(n, k, trials, avg_steps);
}

void AvgResultsDatabaseManager::saveAvgResults(int n, int k, std::vector<int>ratio, int trials, int64_t avg_steps) {
    std::lock_guard<std::mutex> lock(db_mutex);
    auto query = con.Prepare(R"(
        INSERT INTO protocol_results (n, k, ratio, trials, avg_steps)
        VALUES (?, ?, ?, ?, ?)
    )");

    duckdb::vector<duckdb::Value> ratio_values;
    for (const int val: ratio) {
        ratio_values.push_back(duckdb::Value::INTEGER(val));
    }

    auto result = query->Execute(
        duckdb::Value::INTEGER(n),
        duckdb::Value::INTEGER(k),
        duckdb::Value::LIST(duckdb::LogicalType::INTEGER, ratio_values), 
        duckdb::Value::INTEGER(trials),
        duckdb::Value::BIGINT(avg_steps)
    );
}