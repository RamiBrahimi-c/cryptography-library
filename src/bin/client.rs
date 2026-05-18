use advanced_cryptography::ciphers::hashing::hash::sha256_hash;
use advanced_cryptography::common::utils::xor_repeating;
use std::io::{Read, Write};
use std::net::TcpStream;

fn main() {
    let mut stream = TcpStream::connect("127.0.0.1:7878").expect("connect server");
    let key = sha256_hash(b"rust-chat-demo");
    let request = xor_repeating(b"hello from client", &key);
    let _ = stream.write_all(&request);

    let mut buf = [0u8; 1024];
    let read = stream.read(&mut buf).unwrap_or(0);
    let response = xor_repeating(&buf[..read], &key);
    println!("{}", String::from_utf8_lossy(&response));
}
