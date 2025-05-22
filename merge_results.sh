#!/bin/bash

DB_FILE="build/protocol_results.db"

duckdb "${DB_FILE}" <<EOF
BEGIN TRANSACTION;

CREATE TEMP TABLE merged_results AS 
SELECT 
    n,
    k,
    SUM(trials) AS trials,
    SUM(trials * avg_steps) / SUM(trials) AS avg_steps,
    MAX(timestamp) AS timestamp
FROM protocol_results
GROUP BY n, k;

DELETE FROM protocol_results;

INSERT INTO protocol_results 
SELECT * FROM merged_results;

COMMIT;

-- SELECT * FROM protocol_results ORDER BY avg_steps DESC;
EOF

echo "merge done"
