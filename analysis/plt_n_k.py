import duckdb
import pandas as pd
import matplotlib.pyplot as plt

con = duckdb.connect('../build/protocol_results.db')
df = con.execute("SELECT * FROM protocol_results").fetchdf()

plt.figure(figsize=(10,6))
for k in df['k'].unique():
    subset = df[df['k'] == k]
    plt.plot(subset['n'], subset['avg_steps'], label=f'k={k}')

plt.xlabel('Number of Agents (n)')
plt.ylabel('Average Steps to Convergence')
plt.legend()
plt.savefig('results.png')
