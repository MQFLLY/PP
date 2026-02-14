import matplotlib.pyplot as plt
import numpy as np

# 读取数据文件（假设文件名为 'data.txt'，与脚本同目录）
filename = 'sim_data/varing_k_data.txt'

N = []
T_s = []
T_r = []

with open(filename, 'r') as f:
    next(f)  # 跳过表头（如果第一行是 "N	T	T_l	T_u"）
    for line in f:
        parts = line.split()
        if len(parts) == 3:
            n_val = int(parts[0])
            t_val = int(parts[1])
            t_l_val = int(parts[2])
            N.append(n_val)
            T_s.append(t_val)
            T_r.append(t_l_val)

# 转换为 numpy 数组（可选，便于后续处理）
N = np.array(N)
T_s = np.array(T_s)
T_r = np.array(T_r)

# 开始绘图
plt.figure(figsize=(10, 6))
plt.plot(N, T_s, 'o-', label='T with Complete Graph', color='blue')
plt.plot(N, T_r, 's--', label='T with Random Graph', color='green')

# 添加图例、标签、网格
plt.xlabel('Groups Size $k$')
plt.ylabel('Time')
plt.title('Convergence Time Under Non-BS Protocol Based on Different Graph')
plt.legend()
plt.grid(True, linestyle='--', alpha=0.6)

# 可选：使用对数坐标（因为 T_u 增长很快）
# plt.yscale('log')

# 保存图像（可选）
plt.savefig('sim_data/varing_k_data.png', dpi=300, bbox_inches='tight')

# 显示图像
plt.tight_layout()
plt.show()