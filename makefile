EXEC = wayland-boomer

.DEFAULT_GOAL := default
.PHONY: default
default:
	@echo "Available targets:"
	@echo "  - make run"
	@echo "  - make build"
	@echo "  - make install"

.PHONY: run
run: $(EXEC)
	grim - | ASAN_OPTIONS=detect_leaks=1 LSAN_OPTIONS=suppressions=lsan.supp ./$(EXEC)

.PHONY: build
build: $(EXEC)

.PHONY: install
install: $(EXEC)
	sudo cp $(EXEC) /usr/bin/

$(EXEC):
	clang ./src/main.c ./src/image.c ./src/window.c \
		-o $(EXEC) \
		-std=c23 -pedantic -Wall -Wextra -Wpedantic -ggdb -O3 -Wno-gnu \
		-flto -lraylib \
		-fsanitize=address,undefined
