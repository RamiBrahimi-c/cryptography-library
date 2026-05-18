build:
	cargo build

run:
	cargo run

test:
	cargo test

server:
	cargo run --bin server

client:
	cargo run --bin client

clean:
	cargo clean

.PHONY: build run test server client clean
