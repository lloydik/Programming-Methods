all: build

build: src/lab3.cpp
	mkdir -p output && g++ -o ./output/lab3 ./src/lab3.cpp

generate: ./generator/test_generator.py
	rm -rf tests && mkdir tests && ./generator/test_generator.py

analyse: ./output/lab3
	rm -rf results && mkdir results && ls tests | wc -l | xargs -I {} ./output/lab3 {}

make_docs:
	doxygen

draw: ./graphics/graphics.py
	./graphics/graphics.py

NIST: ./NIST_dieharder_tests/test_all.sh
	./NIST_dieharder_tests/test_all.sh