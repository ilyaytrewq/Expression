default_target:
	cmake -S . -B build && cd build && make

main:
	cd build && ./main

test:
	cd build && ./test

clean:
	rm -rf build

.PHONY: default_target main pi test clean