.PHONY: all
all:
	$(MAKE) -C build

.PHONY: redep
redep:
	mkdir -p build/ ; cd build/ ; cmake .. ; cd ..

.PHONY: codeblocks
codeblocks:
	mkdir -p build/ ; cd build ; cmake .. -G "CodeBlocks - Unix Makefiles" ; cd ..

.PHONY: clean
clean:
	$(MAKE) -C build clean

.PHONY: prepare
	./prepare.sh

.PHONY: distclean
distclean:
	$(MAKE) -C build clean
	rm -rf build
