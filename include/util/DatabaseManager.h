#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include "duckdb.hpp"
#include <mutex>
#include <string>
#include <spdlog/spdlog.h>

class DatabaseManager {
public:
    DatabaseManager(const std::string& db_path = "k_div_results.db") : db(db_path), con(db), db_path(db_path) {
        // 创建结果表
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

    void saveResult(int n, int k, int trials, int64_t avg_steps) {
        std::lock_guard<std::mutex> lock(db_mutex);
        auto query = con.Prepare(R"(
            INSERT INTO protocol_results (n, k, trials, avg_steps)
            VALUES (?, ?, ?, ?)
        )");
        query->Execute(n, k, trials, avg_steps);
        // TODO: 
        //  log info instead of io stream
        spdlog::info("dump[{}] ok", db_path);
    }
private:
    duckdb::DuckDB db;
    duckdb::Connection con;
    std::mutex db_mutex;
    std::string db_path;
};

#endif