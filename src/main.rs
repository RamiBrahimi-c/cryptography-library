use advanced_cryptography::registered_ciphers;

fn main() {
    println!("advanced-cryptography Rust rewrite");
    for cipher in registered_ciphers() {
        println!("- {}", cipher.name);
    }
}
