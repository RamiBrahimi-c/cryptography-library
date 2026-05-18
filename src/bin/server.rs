use advanced_cryptography::ciphers::hashing::hash::sha256_hash;
use advanced_cryptography::common::utils::xor_repeating;
use std::io::{Read, Write};
use std::net::TcpListener;

fn main() {
    let listener = TcpListener::bind("127.0.0.1:7878").expect("bind server");
    let key = sha256_hash(b"rust-chat-demo");
    for stream in listener.incoming() {
        let mut stream = stream.expect("accept client");
        let mut buf = [0u8; 1024];
        let read = stream.read(&mut buf).unwrap_or(0);
        if read == 0 {
            continue;
        }
        let plain = xor_repeating(&buf[..read], &key);
        let reply = format!("server received: {}", String::from_utf8_lossy(&plain));
        let sealed = xor_repeating(reply.as_bytes(), &key);
        let _ = stream.write_all(&sealed);
    }
}
