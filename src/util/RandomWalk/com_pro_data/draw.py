import matplotlib.pyplot as plt
import numpy as np

filename = 'n_200.txt'

K = []
T_o = []
T_y = []

# 1. 完整读取所有数据（保持三列对齐）
with open(filename, 'r') as f:
    next(f)  # 跳过表头
    for line in f:
        parts = line.split()
        if len(parts) == 3:
            k_val = float(parts[0])
            n_o_val = float(parts[1])
            n_y_val = float(parts[2])
            K.append(k_val)
            T_o.append(n_o_val)
            T_y.append(n_y_val)

# 转换为 NumPy 数组（便于向量化操作）
K = np.array(K)
T_o = np.array(T_o)
T_y = np.array(T_y)

# 2. 创建掩码：只保留非零值的位置
mask_o = T_o != 0
mask_y = T_y != 0

# 3. 开始绘图
plt.figure(figsize=(10, 6))

# 只绘制非零数据点
if np.any(mask_o):
    plt.plot(K[mask_o], T_o[mask_o], 'o-', label='T with Fluctuation Protocol', color='blue')
if np.any(mask_y):
    plt.plot(K[mask_y], T_y[mask_y], 's--', label='T with Yasumi\'s Protocol', color='green')

# 添加标签和样式
plt.xlabel('Group Size $k$')
plt.ylabel('Time')
plt.title('Convergence Time Under Different Protocols with $N$=200')
plt.legend()
plt.grid(True, linestyle='--', alpha=0.6)
plt.yscale('log')

# 保存与显示
plt.savefig('n_200_data.png', dpi=300, bbox_inches='tight')
plt.tight_layout()
plt.show()