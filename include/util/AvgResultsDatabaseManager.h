#ifndef AVG_RESULTS_DATABASE_MANAGER_H
#define AVG_RESULTS_DATABASE_MANAGER_H

#include <string>
#include <mutex>
#include "DatabaseManager.h" 
#include "spdlog/spdlog.h" 

class AvgResultsDatabaseManager : public DatabaseManager {
public:
    AvgResultsDatabaseManager(const std::string& db_path = "protocol_results.db");
    void saveAvgResults(int n, int k, int trials, int64_t avg_steps);

private:
    std::mutex db_mutex;
};

#endif // AVG_RESULTS_DATABASE_MANAGER_H