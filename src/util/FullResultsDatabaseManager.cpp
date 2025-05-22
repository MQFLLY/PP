#include "util/FullResultsDatabaseManager.h"
#include <iostream>

FullResultsDatabaseManager::FullResultsDatabaseManager(const std::string& db_path) 
    : DatabaseManager(db_path) {
    con.Query("CREATE SEQUENCE IF NOT EXISTS seq_groupid");
    con.Query("CREATE SEQUENCE IF NOT EXISTS seq_resultid");

    con.Query(R"(
        CREATE TABLE IF NOT EXISTS simulation_group (
            group_id INTEGER PRIMARY KEY,
            n INTEGER NOT NULL,
            k INTEGER NOT NULL,
            UNIQUE(n, k)
        )
    )");

    con.Query(R"(
        CREATE TABLE IF NOT EXISTS simulation_result (
            result_id INTEGER PRIMARY KEY,
            group_id INTEGER NOT NULL,
            steps INTEGER NOT NULL,
            timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (group_id) REFERENCES simulation_group(group_id)
        )
    )");

    con.Query("CREATE INDEX IF NOT EXISTS idx_group ON simulation_group(n, k)");
    con.Query("CREATE INDEX IF NOT EXISTS idx_result ON simulation_result(group_id, timestamp)");
}

void FullResultsDatabaseManager::saveFullResults(int n, int k, int steps) {
    std::lock_guard<std::mutex> lock(db_mutex);
    
    const auto group_id = getOrCreateGroup(n, k);

    auto stmt = con.Prepare(R"(
        INSERT INTO simulation_result (result_id, group_id, steps)
        VALUES (nextval('seq_resultid'), $1, $2)
    )");
    stmt->Execute(group_id, steps);
}