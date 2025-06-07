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
	@godot --path project --headless --quit

editor-run: build
	@godot --editor --windowed --path project
