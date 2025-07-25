default_target:
	cmake . -B build && cd build && make

test: default_target
	cd build && ./tests


differentiator: default_target
	cd build && ./differentiator --help

clean:
	rm -rf ./build