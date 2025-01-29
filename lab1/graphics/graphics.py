#!/usr/bin/env python3
import matplotlib.pyplot as plt

analytics_file = './analytics.txt'

with open(analytics_file) as f:
	data = list(map(str.split, map(str.strip, f.readlines())))

all_points = {
}
for line in data:
	count, insertion_time, shaker_time, merge_time, std_time = map(float, line)
	all_points[count] = {
		'Insertion': insertion_time,
		'Shaker': shaker_time,
		'Merge': merge_time,
		'std::sort': std_time,
	}
# line 1 points
volumes = list(all_points.keys())
volumes.sort()

# Да, я прохожусь 4 раза по одному и тому же массиву, но зато так красивее написано
insertion_times = [all_points[v]['Insertion'] for v in volumes]
plt.plot(volumes, insertion_times, label="Insertion Sort")

shaker_times = [all_points[v]['Shaker'] for v in volumes]
plt.plot(volumes, shaker_times, label="Shaker Sort")

merge_times = [all_points[v]['Merge'] for v in volumes]
plt.plot(volumes, merge_times, label="Merge Sort")

std_sort_times = [all_points[v]['std::sort'] for v in volumes]
plt.plot(volumes, std_sort_times, label="std::sort")

plt.xlabel('Rows in data')
plt.ylabel('Time of working algorithm in second')
# plt.title('Two lines on same graph!')
plt.legend()
plt.show()
plt.savefig('../output/')
