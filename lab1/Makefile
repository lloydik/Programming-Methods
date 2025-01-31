all: build

build: src/lab1.cpp
	mkdir -p output && g++ -o ./output/lab1 ./src/lab1.cpp

generate: ./generator/test_generator.py
	rm -rf tests && mkdir tests && ./generator/test_generator.py

analyse: ./output/lab1
	rm -rf results && mkdir results && ls tests | wc -l | xargs -I {} ./output/lab1 {}

make_docs:
	doxygen

draw_graphics: ./graphics/graphics.py
	./graphics/graphics.py
