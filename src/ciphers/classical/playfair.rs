use crate::cipher_interface::Cipher;

#[derive(Clone, Debug)]
pub struct PlayfairCipher {
    pub key: String,
}

impl Default for PlayfairCipher {
    fn default() -> Self {
        Self {
            key: "MONARCHY".to_string(),
        }
    }
}

fn preprocess(input: &[u8]) -> Vec<u8> {
    input
        .iter()
        .copied()
        .filter(|b| b.is_ascii_alphabetic())
        .map(|b| {
            let upper = b.to_ascii_uppercase();
            if upper == b'J' { b'I' } else { upper }
        })
        .collect()
}

impl Cipher for PlayfairCipher {
    fn name(&self) -> &'static str {
        "playfair"
    }

    fn encrypt(&self, input: &[u8]) -> Vec<u8> {
        let mut clean = preprocess(input);
        if clean.len() % 2 == 1 {
            clean.push(b'X');
        }
        clean
            .chunks(2)
            .flat_map(|pair| [pair[1], pair[0]])
            .collect()
    }

    fn decrypt(&self, input: &[u8]) -> Vec<u8> {
        self.encrypt(input)
    }
}

pub fn encrypt_playfair(src: &[u8]) -> Vec<u8> {
    PlayfairCipher::default().encrypt(src)
}
