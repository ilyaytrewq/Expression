default_target:
	cmake . -B build && cd build && make

test: default_target
	cd build && ./tests

main: default_target
	cd build && ./main

clear:
	rm -rf ./build