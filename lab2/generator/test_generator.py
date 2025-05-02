#!/usr/bin/env python3
import random

enc = 'cp1251'


class Accountant:
	def __init__(self):
		all_full_names = open('./generator/fullnames.txt', mode='r', encoding='utf-8').read().splitlines()
		all_job_titles = open('./generator/job_titles.txt', mode='r', encoding='utf-8').read().splitlines()
		all_subdivisions = open('./generator/subdivisions.txt', mode='r', encoding='utf-8').read().splitlines()
		self.full_name = random.choice(all_full_names)
		self.job_title = random.choice(all_job_titles)
		self.subdivision = random.choice(all_subdivisions)
		self.salary = random.randint(100000, 500000)

	def __str__(self):
		return '\n'.join([self.full_name, self.job_title, self.subdivision, str(self.salary)]) + '\n'


if __name__ == '__main__':
	tests_count = random.randint(11, 20)
	print(tests_count)
	# print('Количество тестов (первый - для проверки вручную):', tests_count)
	for test_number in range(tests_count):
		if test_number != 0:
			test_size = random.randint(100, 1000000//tests_count) * test_number
		else:
			test_size = random.randint(10, 20)
		with open(f'./tests/test_{test_number}.txt', 'w+', encoding='utf-8', errors='replace') as f:
			f.write(str(test_size) + '\n')
			for i in range(test_size):
				obj = Accountant()
				f.write(str(obj))
			f.write('\n')
		print(f'Size of test file {test_number}: {test_size}')
