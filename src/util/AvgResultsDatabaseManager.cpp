#include "util/AvgResultsDatabaseManager.h"

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
}

void AvgResultsDatabaseManager::saveAvgResults(int n, int k, int trials, int64_t avg_steps) {
    std::lock_guard<std::mutex> lock(db_mutex);
    auto query = con.Prepare(R"(
        INSERT INTO protocol_results (n, k, trials, avg_steps)
        VALUES (?, ?, ?, ?)
    )");
    query->Execute(n, k, trials, avg_steps);
}