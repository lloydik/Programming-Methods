#!/usr/bin/env python3
import numpy as np
import matplotlib.pyplot as plt
from scipy import stats
import os

analytics_file = './analytics.txt'
results_folder = './results/'

    
def chi_square_test(data, num_bins=10):
    significance_level = 0.05
    # Разбиваем данные на интервалы (бины)
    hist, bin_edges = np.histogram(data, bins=num_bins)
    
    # Ожидаемые частоты (равномерное распределение)
    expected_freq = np.mean(hist)
    
    # Критерий хи-квадрат
    chi2_stat, p_value = stats.chisquare(hist, f_exp=expected_freq)
    
    # Проверяем гипотезу
    return p_value > significance_level

def plot_distribution(data, title, filename):
    plt.figure(figsize=(10, 6))
    
    # Строим гистограмму
    plt.hist(data, bins=50, density=True, alpha=0.7, color='blue')
    
    # Добавляем равномерное распределение для сравнения
    x = np.linspace(min(data), max(data), 100)
    uniform_pdf = np.ones_like(x) / (max(data) - min(data))
    plt.plot(x, uniform_pdf, 'r--', label='Равномерное распределение')
    
    plt.title(title)
    plt.xlabel('Значение')
    plt.ylabel('Частота')
    plt.legend()
    plt.grid(True)
    
    # Сохраняем график
    plt.savefig(filename)
    plt.close()
    
with open(analytics_file) as f:
	data = list(map(str.split, map(str.strip, f.readlines())))

all_points = {}
algo_types = ['LCG', 'QCG', 'XOR', 'STD']
for line in data:
	line_data = list(map(float, line))
	count = line_data.pop(0)
	all_points[count] = dict(zip(algo_types, line_data))
# line 1 points
volumes = list(all_points.keys())
volumes.sort()

plt.figure()

# Да, я прохожусь 4 раза по одному и тому же массиву, но зато так красивее написано

for t in algo_types:
	times = [all_points[v][t] for v in volumes]
	plt.plot(volumes, times, label=t)

plt.xlabel('Rows in data')
plt.ylabel('Time of working algorithm in second')
plt.legend()
plt.yscale('log')
plt.savefig('./graphics/time_graphics.png')

all_files = [f for f in os.listdir(results_folder) if os.path.isfile(os.path.join(results_folder, f))]
tests_count = len(all_files) // len(algo_types)
for i in range(tests_count):
    with open(os.path.join(results_folder, f'result_a_{i}.txt')) as f:
        lcg_data = list(map(int, f.read().strip().split()))
    print(f'TEST LCG #{i}', 'YES' if chi_square_test(lcg_data) else 'NO')
    plot_distribution(lcg_data, f'lcg {i}', f'./graphics/LCG/LCG_{i}.png')
    
    
    with open(os.path.join(results_folder, f'result_b_{i}.txt')) as f:
        qcg_data = list(map(int, f.read().strip().split()))
    print(f'TEST QCG #{i}', 'YES' if chi_square_test(qcg_data) else 'NO')
    plot_distribution(qcg_data, f'qcg {i}', f'./graphics/QCG/QCG_{i}.png')
    
    with open(os.path.join(results_folder, f'result_c_{i}.txt')) as f:
        xor_data = list(map(int, f.read().strip().split()))
    print(f'TEST XORSHIFT #{i}', 'YES' if chi_square_test(xor_data) else 'NO')
    plot_distribution(xor_data, f'xor {i}', f'./graphics/xor/xor_{i}.png')
    
    with open(os.path.join(results_folder, f'result_d_{i}.txt')) as f:
        std_data = list(map(int, f.read().strip().split()))
    print(f'TEST STD #{i}', 'YES' if chi_square_test(std_data) else 'NO')
    plot_distribution(std_data, f'std {i}', f'./graphics/std/std_{i}.png')
    
