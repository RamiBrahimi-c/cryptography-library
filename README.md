# Cryptography Project Library

This project is a comprehensive C implementation of various cryptographic algorithms, ranging from classical ciphers to modern symmetric and asymmetric systems. It also includes hashing utilities and a secure chat application demonstrating a hybrid cryptographic protocol.

## Features

### Classical Ciphers
*   Affine Cipher
*   Caesar Cipher
*   Hill Cipher (Matrix-based)
*   Playfair Cipher
*   Substitution Cipher
*   Vigenere Cipher

### Symmetric Ciphers
*   AES (Advanced Encryption Standard) - Supporting 128-bit keys in CBC mode.
*   DES (Data Encryption Standard) - 16-round implementation.
*   RC4 (Rivest Cipher 4) - Stream cipher.

### Asymmetric Ciphers
*   RSA - Supporting key generation, encryption, and decryption with GMP.
*   ElGamal - Asymmetric encryption based on Diffie-Hellman.
*   Diffie-Hellman - Key exchange protocol for establishing shared secrets.

### Hashing and Integrity
*   MD5
*   SHA-256
*   SHA-512
*   Avalanche effect testing and benchmarking tools.

### Multimedia Support
The project includes external headers (stb) to demonstrate encryption on:
*   Images (PNG, JPG, BMP)
*   Audio (Ogg Vorbis)

---

## Dependencies

To compile and run this project, you must have the following libraries installed:

1.  **GMP (GNU Multiple Precision Arithmetic Library):** Used for large prime number calculations in asymmetric ciphers.
2.  **OpenSSL (libcrypto):** Used for modern hashing algorithms (MD5, SHA).

On Ubuntu/Debian, install them via:
```bash
sudo apt-get install libgmp-dev libssl-dev
```

---

## Building the Project

The provided Makefile manages the compilation of the library, tests, and the chat application.

### Main Library
To compile the core project implementation:
```bash
make
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