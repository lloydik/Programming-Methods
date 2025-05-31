#!/usr/bin/env python3
import random

if __name__ == '__main__':
	tests_count = random.randint(11, 20)
	print(tests_count)
	# print('Количество тестов (первый - для проверки вручную):', tests_count)
	for test_number in range(tests_count):
		with open(f'./tests/test_{test_number}.txt', 'w+', encoding='utf-8', errors='replace') as f:
			if test_number != 0:
				test_size = random.randint(100, 1000000//tests_count) * test_number
			else:
				test_size = random.randint(10, 20)
			f.write(str(test_size) + '\n')
			f.write('\n')
		print(f'Count in test file {test_number}: {test_size}')
