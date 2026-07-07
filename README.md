# Cryptography Project Library

This project is a comprehensive C implementation of various cryptographic algorithms, ranging from classical ciphers to modern symmetric and asymmetric systems. It also includes hashing utilities and a secure chat application demonstrating a hybrid cryptographic protocol.
The current version has many dependecies and Ai help which will be removed eventually . 

## Features

### Classical Ciphers
*   Affine Cipher
*   Caesar Cipher
*   Hill Cipher (Matrix-based)
*   Playfair Cipher
*   Substitution Cipher
*   Vigenere Cipher

### Symmetric Ciphers
*   AES (Advanced Encryption Standard) - Supporting 128 , 192 , 256 -bit keys in ECB mode.
*   DES (Data Encryption Standard) - 16-round implementation.
*   RC4 (Rivest Cipher 4) - Stream cipher.
*   Red Pike - UK's GCHQ Block cipher .
*   TEA (Tiny Encryption Algorithm) - Block Cipher  .
*   XTEA (eXtended TEA) - Block Cipher  .
*   Blowfish 🐡 - Block Cipher  .

### Asymmetric Ciphers
*   RSA - Supporting key generation, encryption, and decryption with GMP.
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

To compile and run this project, you must have the following libraries installed:

1.  **GMP (GNU Multiple Precision Arithmetic Library):** Used for large prime number calculations in asymmetric ciphers.

On Ubuntu/Debian, install it via:
```bash
sudo apt-get install libgmp-dev 
```

---

Here's the updated "Building the Project" section for your README:

## Building the Project

The provided Makefile manages the compilation of the library, tests, and the chat application.

### Main Library
To compile the core project implementation:
```bash
make
```

### Static Libraries

To build the full static library (requires GMP):
```bash
make libcrypto.a
```

To build the lite static library (classical and symmetric ciphers only, no external dependencies):
```bash
make libcrypto-lite.a
```

### Running Tests
To run the full test suite (which includes verification of all ciphers):
```bash
make test
```

To run a specific test (e.g., for RSA):
```bash
make test-rsa
```

### Chat Application
To build both the server and client:
```bash
make chat
```

To build them individually:
```bash
make server
make client
```

Run the server in one terminal and the client in another:
```bash
./server
./client
```

### Cleaning Build Files
To remove object files and compiled binaries:
```bash
make clean
```
---

## Secure Chat Application

The project includes a server-client chat application. It establishes a secure connection using:
1.  **Diffie-Hellman** to exchange a shared secret.
2.  **SHA-256** to derive an AES key from that secret.
3.  **AES-CBC** to encrypt messages.
4.  **HMAC-style SHA-256** signatures to verify message integrity.

### How to run:
1.  Open two terminals.
2.  In terminal 1, start the server:
    ```bash
    make server
    ./server
    ```
3.  In terminal 2, start the client:
    ```bash
    make client
    ./client
    ```

---

## Project Structure

*   `include/`: Header files organized by cipher category.
*   `src/`: Source code implementations.
    *   `ciphers/`: Implementation of the cryptographic logic.
    *   `common/`: Shared utilities and mathematical constants.
*   `tests/`: Test files and test runners using the `test_utils.h` framework.
*   `Makefile`: Build configuration.

## Legal Disclaimer
This library is for educational and academic purposes only. Do not use this implementation for securing sensitive production data, as it may lack protection against advanced side-channel attacks.

## References
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
    *   https://www.schneier.com/academic/blowfish/ (contains a lot of useful stuff)
    *   https://www.schneier.com/wp-content/uploads/2015/12/vectors-2.txt (test vectors)
    *   https://www.schneier.com/wp-content/uploads/2015/12/vectors2-1.txt
    *   https://www.schneier.com/wp-content/uploads/2015/12/constants-2.txt (first (1024 + 18)*32 bit of PI)
    *   http://www.geom.uiuc.edu/~huberty/math5337/groupe/digits.html (100,000 Digits of Pi)
    *   https://raw.githubusercontent.com/glycerine/sshego/v4.0.3/_vendor/golang.org/x/crypto/blowfish/blowfish_test.go (test vectors program in GO lang)

