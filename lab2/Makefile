all: build

build: src/lab2.cpp
	mkdir -p output && g++ -o ./output/lab2 ./src/lab2.cpp

generate: ./generator/test_generator.py
	rm -rf tests && mkdir tests && ./generator/test_generator.py

analyse: ./output/lab2
	rm -rf results && mkdir results && ls tests | wc -l | xargs -I {} ./output/lab2 {}

make_docs:
	doxygen

draw: ./graphics/graphics.py
	./graphics/graphics.py
