use advanced_cryptography::ciphers::asymmetric::dh::{dh_compute_shared, dh_generate_keypair};
use advanced_cryptography::ciphers::hashing::hash::sha256_hash;
use advanced_cryptography::ciphers::symmetric::aes::{aes_decrypt, aes_encrypt};
use num_bigint::BigUint;
use std::io::{BufRead, BufReader, Read, Write};
use std::net::TcpStream;

fn main() {
    let mut stream = TcpStream::connect("127.0.0.1:7878").expect("connect server");
    let mut reader = BufReader::new(stream.try_clone().expect("clone stream"));
    let mut line = String::new();
    let _ = reader.read_line(&mut line);
    let mut parts = line.trim().split('|');
    let p = BigUint::parse_bytes(parts.next().expect("p").as_bytes(), 16).expect("parse p");
    let g = BigUint::parse_bytes(parts.next().expect("g").as_bytes(), 16).expect("parse g");
    let server_public = BigUint::parse_bytes(parts.next().expect("server pub").as_bytes(), 16).expect("parse server pub");

    let party = dh_generate_keypair(&p, &g);
    let _ = stream.write_all(format!("{}\n", party.public.to_str_radix(16)).as_bytes());
    let shared = dh_compute_shared(&party, &server_public);
    let digest = sha256_hash(&shared.to_bytes_be());
    let aes_key = &digest[..16];

    let request = aes_encrypt(b"hello from client", aes_key);
    let _ = stream.write_all(&(request.len() as u32).to_be_bytes());
    let _ = stream.write_all(&request);

    let mut len_buf = [0u8; 4];
    stream.read_exact(&mut len_buf).expect("read reply length");
    let frame_len = u32::from_be_bytes(len_buf) as usize;
    let mut frame = vec![0u8; frame_len];
    stream.read_exact(&mut frame).expect("read reply");
    let response = aes_decrypt(&frame, aes_key);
    println!("{}", String::from_utf8_lossy(&response));
}
