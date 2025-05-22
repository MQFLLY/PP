#ifndef FULL_RESULTS_DATABASE_MANAGER_H
#define FULL_RESULTS_DATABASE_MANAGER_H

#include <string>
#include <mutex>
#include <iostream>
#include "DatabaseManager.h"

class FullResultsDatabaseManager : public DatabaseManager {
public:
    FullResultsDatabaseManager(const std::string& db_path = "protocol_results.db");
    void saveFullResults(int n, int k, int steps);

private:
    int64_t getOrCreateGroup(int n, int k) {
        auto select_stmt = con.Prepare(
            R"(SELECT group_id FROM simulation_group WHERE n=$1 AND k=$2)"
        );
        auto select_result = select_stmt->Execute(n, k);
        
        auto chunk = select_result->Fetch();
        if (chunk && chunk->size() > 0) {
            return chunk->GetValue(0, 0).GetValue<int64_t>();
        }
        
        auto insert_stmt = con.Prepare(
            "INSERT INTO simulation_group (group_id, n, k) "
            "VALUES (nextval('seq_groupid'), $1, $2)"
        );
        insert_stmt->Execute(n, k);

        auto verify_stmt = con.Prepare(
            R"(SELECT group_id FROM simulation_group WHERE n=$1 AND k=$2)"
        );
        auto verify_result = verify_stmt->Execute(n, k);
        
        auto verify_chunk = verify_result->Fetch();
        if (!verify_chunk || verify_chunk->size() == 0) {
            throw std::runtime_error("Failed to retrieve inserted group_id");
        }
        
        return verify_chunk->GetValue(0, 0).GetValue<int64_t>();
    }
};

#endif // FULL_RESULTS_DATABASE_MANAGER_H