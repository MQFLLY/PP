import matplotlib.pyplot as plt
import numpy as np

# 读取数据文件（假设文件名为 'data.txt'，与脚本同目录）
filename = 'theo_data_diff_g.txt'

N = []
T = []
T_l = []
T_u = []

with open(filename, 'r') as f:
    next(f)  # 跳过表头（如果第一行是 "N	T	T_l	T_u"）
    for line in f:
        parts = line.split()
        if len(parts) == 4:
            n_val = int(parts[0])
            t_val = int(parts[1])
            t_l_val = int(parts[2])
            t_u_val = int(parts[3])
            N.append(n_val)
            T.append(t_val)
            T_l.append(t_l_val)
            T_u.append(t_u_val)

# 转换为 numpy 数组（可选，便于后续处理）
N = np.array(N)
T = np.array(T)
T_l = np.array(T_l)
T_u = np.array(T_u)

# 开始绘图
plt.figure(figsize=(10, 6))
plt.plot(N, T, 'o-', label='T with Star Graph', color='blue')
plt.plot(N, T_l, 's--', label='T with Complete Graph', color='green')
plt.plot(N, T_u, '^-', label='T with Random Graph', color='red')

# 添加图例、标签、网格
plt.xlabel('Population Size $N$')
plt.ylabel('Time')
plt.title('Theoretical Bounds with Diff Graphs')
plt.legend()
plt.grid(True, linestyle='--', alpha=0.6)

# 可选：使用对数坐标（因为 T_u 增长很快）
# plt.yscale('log')

# 保存图像（可选）
plt.savefig('theo_data_diff_g.png', dpi=300, bbox_inches='tight')

# 显示图像
plt.tight_layout()
plt.show()