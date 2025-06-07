SHELL := /usr/bin/zsh

.PHONY: build run build-run test

build:
	@cmake --build build/gcc-debug

run:
	@godot --path project

build-run: build run

test: build
	@godot --path project --headless --quit
