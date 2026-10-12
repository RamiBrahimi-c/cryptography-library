# Cryptography Project Library

A self-contained cryptography library written from scratch in C. Classical ciphers, modern symmetric ciphers, hashing, and asymmetric ciphers — all implemented without external crypto dependencies. The arbitrary-precision arithmetic under the asymmetric ciphers is handled by my own bignum library, [big-ar9am](https://github.com/RamiBrahimi-c/big-ar9am).


# Important Disclaimer

This project is **NOT** intended for professional or production use ; if you need that , then you should be checking [this](https://openssl-library.org/) .


## Why this exists

The project started as an assignment from the class *Advanced Cryptography* in the 3rd year 2nd semester cybersecurity Ing major (2025/2026) by the Prof [Halim Zaidi](https://www.linkedin.com/in/halim-zaidi-5975a32b3/) .

Though under the time pressure and other circumstances I was forced to rely on dependencies and some AI help at the beginning just to deliver the project by the deadline (was approximately on 21/5/2026).

**BUT** , just after finishing exams and all of that I came back and I stripped every dependency : tinyAES , GMP , Openssl . Just so that I own every aspect of this project .

So it is worth noting that you should also know this project is made for fun/learning ; it is **NOT optimized** or fancy at all , in fact it is made ugly and slow too (in AES for example you will find me calculating the S-box using my defined functions for handling operations in GF(2^8)) but correct and verified against official test vectors . 

## Other fun program(s) that use this lib?

**NOTE AGAIN !** As mentioned this is **NOT** for production , the program(s) listed below are also mine and they serve the goal : *proof of concept* or made just for fun !!!! 

1. **Flutter app for demonstrating Cryptography** : a Flutter app that uses this lib exactly via FFI to let you encrypt text/images or visualize ciphers effects and so on , Check it [here](https://github.com/RamiBrahimi-c/sifar-c-crypto-lib-in-flutter) 


## Current Status

Stable version .

## Features

### Classical Ciphers
*   Affine Cipher
*   Caesar Cipher
*   Hill Cipher (Matrix-based)
*   Playfair Cipher
*   Substitution Cipher
*   Vigenère Cipher

### Symmetric Ciphers
*   AES (Advanced Encryption Standard) - Supporting 128 , 192 , 256 -bit keys.
*   DES (Data Encryption Standard) - 16-round implementation.
*   3DES (Triple Data Encryption Standard) .
*   RC4 (Rivest Cipher 4) - Stream cipher.
*   Red Pike - UK's GCHQ Block cipher .
*   TEA (Tiny Encryption Algorithm) - Block Cipher  .
*   XTEA (eXtended TEA) - Block Cipher  .
*   Blowfish 🐡 - Block Cipher  .

### Block Modes Operations for Block Ciphers 
* ECB
* CBC
* OFB
* CFB
* CTR

### Asymmetric Ciphers
- Supporting arbitrary length numbers (thanks to [**big-ar9am**](https://github.com/RamiBrahimi-c/big-ar9am))

*   RSA - Supporting key generation, encryption, and decryption .
*   ElGamal - Asymmetric encryption based on Diffie-Hellman.
*   Diffie-Hellman - Key exchange protocol for establishing shared secrets.

### Hashing and Integrity
*   MD4
*   MD5
*   SHA-256
*   SHA-512
*   HMAC (SHA-256 , SHA-512)
*   HKDF (SHA-256 , SHA-512)
*   Avalanche effect testing and benchmarking tools.

### Multimedia Support
The project includes external headers (stb) to demonstrate encryption on:
*   Images (PNG, JPG, BMP)
*   Audio (Ogg Vorbis)

---


## Dependencies

**No external crypto dependencies.**

The library depends only on the standard C library and my own [bignum library](https://github.com/RamiBrahimi-c/big-ar9am) (`big-ar9am`), which is vendored in `third-party/`.

GMP is **not** a dependency of the library. It is used only in the asymmetric reference tests, as an oracle to verify my ciphers produce the same results as a trusted implementation. If you don't run those tests, you don't need GMP installed.



---

## Building

### Requirements

- GCC or Clang (GCC recommended but works with both)
- GNU Make
- Standard C library (libc)

Optional (only for `make test_gmp_*`):
- GMP — used as a reference for verifying asymmetric ciphers

### Build everything

```bash
make all
```

This produces four libraries in `lib/`:

| File | What it is | Depends on |
|------|------------|------------|
| `libsifra.a` | Full static library — everything | `libbigra9m` at link time |
| `libsifra.so` | Full shared library — everything, bignum baked in | nothing else at link time |
| `libsifra-lite.a` | Static, classical + symmetric ciphers only | `-lm` |
| `libsifra-lite.so` | Shared, classical + symmetric ciphers only | `-lm` |

### Which one do I link?

- **`libsifra.so`** — easiest. One file, self-contained. Link with `-lsifra -lm` and you're done.
- **`libsifra.a`** — a plain archive. You must also link `-lbigra9m` yourself, and it has to come **after** `-lsifra` on the command line:

  ```
  gcc your_prog.c -lsifra -lbigra9m -lm
  ```

  The reason: `libsifra.a` is just a bag of `.o` files with unresolved `bigra9m_*` symbols. The linker fills those holes by pulling from `libbigra9m.a` — but only if it comes afterward.

- **`libsifra-lite.*`** — if you only want classical and symmetric ciphers. No bignum, no asymmetric, no hashing. Just `-lm`.

### Clean

```bash
make clean
```

Removes `obj/`, `bin/`, `lib/`, and `tests/output/`.

---

## Testing

### Regular tests

Each test compiles and runs automatically:

```bash
make test_aes
make test_rsa
make test_sha
make test_hash
```

Test binaries are placed in `tests/output/bin/`.

### GMP reference tests

The asymmetric ciphers have a second set of tests that verify their output against GMP. These live in `tests/asymmetric-gmp/`:

```bash
make test_gmp_rsa
make test_gmp_dh
make test_gmp_elgamal
```

These require GMP installed and link against it. Everything else doesn't.

---


## Project Structure




```
.
├── include/                               # Public headers
│   ├── ciphers/                           # One header per cipher, grouped by category
│   ├── common/                            # Shared utilities and math
│   ├── sifar.h                            # Our API interface that exposes all ciphers functions/Data Types
│   ├── sifar_ffi.h                        # Our API interface for Flutter FFI
│   ├── cipher_interface.h                 # High level API definitions
│   └── block_cipher_modes_operation.h     # High level API definitions
├── src/                                   # Implementation
│   ├── ciphers/                           # Cipher implementations
│   ├── common/                            # Shared utilities
│   ├── main.c                             # Useless
│   └── sifar_ffi.c                        # Our API interface for Flutter FFI
├── tests/                                 # Test sources
│   ├── asymmetric-gmp/                    # GMP-verified asymmetric tests
│   ├── third-party/                       # Vendored stb headers
│   ├── output/                            # Generated test binaries and results
│   ├── test_*.c                           # Unit-tests for ciphers/hashing utilities/encrypting images 
│   ├── test_utils.h                       # Useful macros and functions for testing 
│   └── test_utils.c                       # Useful functions for encrypting on images
├── lib/                                   # Built libraries (generated)
├── obj/                                   # Object files (generated)
└── third-party/                           # Third party libs used
    └── big-ar9am/                         # My bignum library, vendored


```

## References
*   *Stb*   :
    *   https://github.com/nothings/stb

*   *BLOCK CIPHERS : MODES OF OPERATION*  :
    *   Understanding Cryptography **Christof Paar , Jan Pelzl** -- page 123 -- 


*   *HKDF*  :
    *   https://datatracker.ietf.org/doc/html/rfc5869#autoid-4
    *   (useful for testing) https://asecuritysite.com/kdf/HKDF
    *   (official paper) https://eprint.iacr.org/2010/264

*   *HMAC*  :
    *   https://csrc.nist.gov/files/pubs/fips/198/final/docs/fips-198a.pdf
    *   (useful for testing) https://www.devglan.com/online-tools/hmac-sha256-online

*   *SHA*  :
    *   (the manual SHS for SHA family ) https://nvlpubs.nist.gov/nistpubs/fips/nist.fips.180-4.pdf
    *   (intermediate values for debugging ) https://csrc.nist.gov/projects/cryptographic-standards-and-guidelines/example-values
*   *MD4* :
    *   https://datatracker.ietf.org/doc/html/rfc1186#autoid-4

*   *MD5* :
    *   https://www.rfc-editor.org/info/rfc1321/


*   *XTEA* : 
    *   https://commons.wikimedia.org/wiki/File:XTEADataFlow.jpg
    *   https://www.cix.co.uk/~klockstone/xtea.pdf
    *   https://www.iacr.org/archive/fse2004/30170298/30170298.pdf
    *   https://web.archive.org/web/20071228053741/http://143.53.36.235:8080/tea.htm
    *   https://www.cix.co.uk/~klockstone/teavect.htm
    *   (the wiki) https://en.wikipedia.org/wiki/XTEA

*   *Blowfish* : 
    *   (the wiki) https://en.wikipedia.org/wiki/Blowfish_(cipher)
    *   https://opencourses.emu.edu.tr/pluginfile.php/47497/mod_resource/content/2/Block%20ciphers%20(Blowfish).pdf
    *   https://lsisreviving.weebly.com/uploads/2/3/6/8/23689241/blowfish_algorithm_1.pdf
    *   https://www.schneier.com/academic/blowfish/ (contains a lot of useful informations)
    *   https://www.schneier.com/wp-content/uploads/2015/12/vectors-2.txt (test vectors)
    *   https://www.schneier.com/wp-content/uploads/2015/12/vectors2-1.txt
    *   https://www.schneier.com/wp-content/uploads/2015/12/constants-2.txt (first (1024 + 18)*32 bits of PI)
    *   http://www.geom.uiuc.edu/~huberty/math5337/groupe/digits.html (100,000 Digits of Pi)
    *   https://raw.githubusercontent.com/glycerine/sshego/v4.0.3/_vendor/golang.org/x/crypto/blowfish/blowfish_test.go (test vectors program in GO)

*   *3DES* :
    *   http://tripledes.online-domain-tools.com/ (for validating)
