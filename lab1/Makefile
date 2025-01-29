all: build

build: src/lab1.cpp
	mkdir output && g++ -o ./output/lab1 ./src/lab1.cpp

generate: ./generator/test_generator.py
	rm -rf tests && mkdir tests && ./generator/test_generator.py

analyse: ./output/lab1
	./output/lab1

docs:
	doxygen

graphics:
	python3 -m pip install -r ./graphics/requirements.txt && ./graphics/graphics.py
