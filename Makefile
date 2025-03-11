default_target:
	cmake . -B build && cd build && make

test: default_target
	cd build && ./tests

test: default_target
	cd build && ./main

clear:
	rm -rf ./build