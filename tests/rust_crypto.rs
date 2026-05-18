use advanced_cryptography::cipher_interface::Cipher;
use advanced_cryptography::ciphers::asymmetric::dh::{dh_compute_shared, dh_generate_keypair, dh_generate_params};
use advanced_cryptography::ciphers::asymmetric::elgamal::{elgamal_decrypt, elgamal_encrypt, elgamal_generate_keypair};
use advanced_cryptography::ciphers::asymmetric::rsa::{rsa_decrypt, rsa_encrypt, rsa_generate_keypair};
use advanced_cryptography::ciphers::classical::affine::{AffineCipher, AffineKey};
use advanced_cryptography::ciphers::classical::caesar::CaesarCipher;
use advanced_cryptography::ciphers::classical::hill::{HillCipher, HillKey};
use advanced_cryptography::ciphers::classical::playfair::PlayfairCipher;
use advanced_cryptography::ciphers::classical::substitution::{SubstitutionCipher, SubstitutionKey};
use advanced_cryptography::ciphers::classical::vigenere::VigenereCipher;
use advanced_cryptography::ciphers::hashing::hash::{md5_hash, sha256_hash, sha512_hash};
use advanced_cryptography::ciphers::symmetric::aes::{AesCipher, AesKey};
use advanced_cryptography::ciphers::symmetric::des::DesCipher;
use advanced_cryptography::ciphers::symmetric::rc4::Rc4Cipher;
use num_bigint::BigUint;

#[test]
fn classical_roundtrips() {
    let caesar = CaesarCipher::default();
    let text = b"Hello World";
    assert_eq!(caesar.decrypt(&caesar.encrypt(text)), text);

    let vig = VigenereCipher::new("KEY");
    assert_eq!(vig.decrypt(&vig.encrypt(text)), text);

    let substitution = SubstitutionCipher {
        key: SubstitutionKey::from_key("QWERTYUIOPASDFGHJKLZXCVBNM").unwrap(),
    };
    assert_eq!(substitution.decrypt(&substitution.encrypt(text)), text);

    let playfair = PlayfairCipher::default();
    let encrypted = playfair.encrypt(b"HIDETHEGOLD");
    let decrypted = playfair.decrypt(&encrypted);
    assert!(decrypted.starts_with(b"HIDETHEGOLD"));
}

#[test]
fn bytewise_roundtrips() {
    let affine = AffineCipher {
        key: AffineKey::new(5, 8, 256).unwrap(),
    };
    let bytes = [0u8, 1, 2, 3, 127, 200, 255];
    assert_eq!(affine.decrypt(&affine.encrypt(&bytes)), bytes);

    let hill = HillCipher {
        key: HillKey::from_matrix(vec![vec![1, 2], vec![3, 5]], 256).unwrap(),
    };
    let payload = b"binary payload".to_vec();
    assert_eq!(hill.decrypt(&hill.encrypt(&payload)), payload);
}

#[test]
fn symmetric_roundtrips() {
    let aes = AesCipher {
        key: AesKey::new(b"0123456789abcdef"),
    };
    let des = DesCipher::default();
    let rc4 = Rc4Cipher::default();
    let message = b"secret message";

    assert_eq!(aes.decrypt(&aes.encrypt(message)), message);
    assert_eq!(des.decrypt(&des.encrypt(message)), message);
    assert_eq!(rc4.decrypt(&rc4.encrypt(message)), message);
}

#[test]
fn asymmetric_roundtrips() {
    let rsa = rsa_generate_keypair(256, 65_537);
    let msg = b"hello rsa";
    assert_eq!(rsa_decrypt(&rsa_encrypt(msg, &rsa), &rsa), msg);

    let (p, g) = dh_generate_params(96);
    let alice = dh_generate_keypair(&p, &g);
    let bob = dh_generate_keypair(&p, &g);
    assert_eq!(dh_compute_shared(&alice, &bob.public), dh_compute_shared(&bob, &alice.public));

    let elgamal = elgamal_generate_keypair(96);
    let message = BigUint::from(42u32);
    let ciphertext = elgamal_encrypt(&message, &elgamal);
    assert_eq!(elgamal_decrypt(&ciphertext, &elgamal), message);
}

#[test]
fn hash_lengths() {
    assert_eq!(md5_hash(b"hello").len(), 16);
    assert_eq!(sha256_hash(b"hello").len(), 32);
    assert_eq!(sha512_hash(b"hello").len(), 64);
}
