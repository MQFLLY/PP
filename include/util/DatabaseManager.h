#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include "duckdb.hpp"
#include <mutex>
#include <string>
#include <spdlog/spdlog.h>

class DatabaseManager {
public:
    DatabaseManager(const std::string& db_path): db(db_path), con(db), db_path(db_path)
     {}
    duckdb::DuckDB db;
    duckdb::Connection con;
    std::mutex db_mutex;
    std::string db_path;
};

#endif