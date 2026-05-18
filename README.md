# Advanced Cryptography

This repository is now a Rust rewrite of the original coursework project. It keeps the same broad scope:

- Classical ciphers: Affine, Caesar, Hill, Playfair, Substitution, Vigenere
- Symmetric ciphers: AES-CBC, DES-CBC, RC4, generic Feistel helper
- Asymmetric primitives: RSA, ElGamal, Diffie-Hellman
- Hashing: MD5, SHA-256, SHA-512, avalanche and benchmark helpers
- Demo chat binaries: a minimal DH + SHA-256 + AES exchange

## Layout

- `src/lib.rs`: library root
- `src/ciphers/`: cipher implementations by family
- `src/common/`: shared math and utility code
- `src/bin/server.rs`: chat server demo
- `src/bin/client.rs`: chat client demo
- `tests/rust_crypto.rs`: broad round-trip smoke tests

## Commands

```bash
make build
make test
make server
make client
```

You can also run Cargo directly:

```bash
cargo build
cargo test
cargo run
```

## Notes

This code is for educational use. Several algorithms are intentionally straightforward implementations without production hardening or modern padding/authentication schemes.
