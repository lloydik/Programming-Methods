#!/usr/bin/env python3
import matplotlib.pyplot as plt

analytics_file = './analytics.txt'

with open(analytics_file) as f:
	data = list(map(str.split, map(str.strip, f.readlines())))

all_points = {}
algo_types = ['Linear', 'Binary tree', 'Red-black tree', 'Hashmap', 'Multimap']
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
# plt.title('Two lines on same graph!')
plt.legend()
plt.yscale('log')
plt.savefig('./graphics/graphics.png')
