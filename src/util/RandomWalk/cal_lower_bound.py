import numpy as np
import argparse
import os

def f_prime_at_one(B, C):
    """计算f'(1)"""
    n = B.shape[0]
    I = np.eye(n)
    
    I_minus_B = I - B
    if np.linalg.matrix_rank(I_minus_B) < n:
        raise ValueError("矩阵(I-B)不可逆")
    
    inv_I_minus_B = np.linalg.inv(I_minus_B)

    term1 = inv_I_minus_B @ C
    d_inv_dz = inv_I_minus_B @ B @ inv_I_minus_B
    term2 = d_inv_dz @ C
    
    return term1 + term2

def inv_I_minus_B_times_ones_simple(B):
    """简化版本"""
    n = B.shape[0]
    I = np.eye(n)
    ones = np.ones((n, 1))
    return np.linalg.solve(I - B, ones)  # 使用solve更稳定

def main():
    parser = argparse.ArgumentParser(description="计算f'(1)，其中f(z)=z(I-zB)^-1*C")
    parser.add_argument('-b', '--matrix_b', required=True, help='B矩阵文件')
    parser.add_argument('-c', '--vector_c', required=True, help='C向量文件')
    parser.add_argument('-f', '--format', choices=['txt', 'csv', 'npy'], default='csv', help='文件格式')
    parser.add_argument('-o', '--output', help='输出文件')
    
    args = parser.parse_args()
    
    # 读取文件
    try:
        if args.format == 'txt':
            # 简化的TXT读取
            B = np.loadtxt(args.matrix_b)
            C = np.loadtxt(args.vector_c).reshape(-1, 1)
        elif args.format == 'csv':
            B = np.loadtxt(args.matrix_b, delimiter=',')
            C = np.loadtxt(args.vector_c, delimiter=',').reshape(-1, 1)
        elif args.format == 'npy':
            B = np.load(args.matrix_b)
            C = np.load(args.vector_c).reshape(-1, 1)
        
        print(f"成功读取:")
        print(f"  B矩阵: {B.shape}")
        print(f"  C向量: {C.shape}")
        
        # 计算f'(1)
        result = inv_I_minus_B_times_ones_simple(B)
        
        print(f"\nf'(1)的结果:")
        print(result)
        # 计算最大值并打印（方便验证）
        max_value = np.max(result)
        print(f"result的最大值: {max_value:.8f}")
         
        # 保存结果（修复后的逻辑）
        if args.output:
            # 1. 先将result转为字符串（保持格式）
            result_str = '\n'.join([f"{x[0]:.8f}" for x in result])
            # 2. 以追加模式打开文件
            with open(args.output, 'a', encoding='utf-8') as f:
                # 隔一行追加（空行分隔）
                f.write('\n\n')
                # 写入result内容
                f.write(result_str)
                # 写入最大值（最后一行）
                f.write(f"\n最大值: {max_value:.8f}")
            
            print(f"\n结果已追加保存到 {args.output}")
            
    except Exception as e:
        print(f"错误: {e}")
        return 1
    
    return 0

if __name__ == "__main__":
    exit(main())