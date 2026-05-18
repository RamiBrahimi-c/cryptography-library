use advanced_cryptography::ciphers::asymmetric::dh::{dh_compute_shared, dh_generate_keypair, dh_generate_params};
use advanced_cryptography::ciphers::hashing::hash::sha256_hash;
use advanced_cryptography::ciphers::symmetric::aes::{aes_decrypt, aes_encrypt};
use hex::{decode, encode};
use num_bigint::BigUint;
use std::io::{BufRead, BufReader, Read, Write};
use std::net::TcpListener;

fn main() {
    let listener = TcpListener::bind("127.0.0.1:7878").expect("bind server");
    let (p, g) = dh_generate_params(128);
    for stream in listener.incoming() {
        let mut stream = stream.expect("accept client");
        let party = dh_generate_keypair(&p, &g);
        let hello = format!("{}|{}|{}\n", p.to_str_radix(16), g.to_str_radix(16), party.public.to_str_radix(16));
        let _ = stream.write_all(hello.as_bytes());

        let mut reader = BufReader::new(stream.try_clone().expect("clone stream"));
        let mut line = String::new();
        let _ = reader.read_line(&mut line);
        let their_public = BigUint::parse_bytes(line.trim().as_bytes(), 16).expect("client public");
        let shared = dh_compute_shared(&party, &their_public);
        let digest = sha256_hash(&shared.to_bytes_be());
        let aes_key = &digest[..16];

        let mut len_buf = [0u8; 4];
        stream.read_exact(&mut len_buf).expect("read payload length");
        let frame_len = u32::from_be_bytes(len_buf) as usize;
        let mut frame = vec![0u8; frame_len];
        stream.read_exact(&mut frame).expect("read ciphertext");
        let plain = aes_decrypt(&frame, aes_key);

        let reply = format!("server received: {}", String::from_utf8_lossy(&plain));
        let ciphertext = aes_encrypt(reply.as_bytes(), aes_key);
        let _ = stream.write_all(&(ciphertext.len() as u32).to_be_bytes());
        let _ = stream.write_all(&ciphertext);
        let _ = stream.flush();

        let transcript = encode(reply.as_bytes());
        let _ = decode(transcript).expect("hex roundtrip");
    }
}
