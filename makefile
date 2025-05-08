EXEC       = wayland-boomer
GIT_COMMIT = $(shell git rev-parse HEAD)

.DEFAULT_GOAL := default
.PHONY: default
default:
	@echo "Available targets:"
	@echo "  - make run"
	@echo "  - make build"
	@echo "  - make install"

.PHONY: run
run: $(EXEC)
	grim - | ./$(EXEC)

.PHONY: build
build: $(EXEC)

.PHONY: install
install: $(EXEC)
	sudo cp $(EXEC) /usr/bin/

$(EXEC):
	clang ./src/main.c ./src/globals.c ./src/args.c ./src/controls.c ./src/image.c ./src/window.c ./src/draw.c \
		-o $(EXEC) \
		-std=c23 -pedantic -Wall -Wextra -Wpedantic -ggdb -O3 -Wno-gnu \
		-flto -lraylib \
		-DVERSION="\"$(GIT_COMMIT)\""
