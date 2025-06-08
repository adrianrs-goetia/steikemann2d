SHELL := /usr/bin/zsh

.PHONY: build run build-run test cmake-configure-all

cmake-configure-all:
	@cmake --preset=gcc-debug
	@cmake --preset=gcc-release
	@cmake --preset=mingw-debug
	@cmake --preset=mingw-release

build:
	@cmake --build build/gcc-debug

run:
	@godot --path project

build-run: build run

test: build
	@godot-dbg --path project --headless --quit -- --runtests=yes

editor-run: build
	@godot --editor --windowed --path project --position 1720,0 --resolution 1700x1350
