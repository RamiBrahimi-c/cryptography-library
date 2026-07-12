#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "ciphers/asymmetric/dh.h"
#include "ciphers/symmetric/aes.h"
#include "ciphers/hashing/hash.h"

#define PORT 8080
#define BUFFER_SIZE 65536

static int read_exact(int fd, void* buf, int len) {
    int total = 0;
    while (total < len) {
        int n = read(fd, (char*)buf + total, len - total);
        if (n <= 0) return -1;
        total += n;
    }
    return total;
}

static void send_str(int fd, const char* str) {
    uint32_t len = strlen(str);
    uint32_t net = htonl(len);
    send(fd, &net, 4, 0);
    send(fd, str, len, 0);
}

static char* recv_str(int fd) {
    uint32_t net;
    if (read_exact(fd, &net, 4) < 0) return NULL;
    uint32_t len = ntohl(net);
    char* str = malloc(len + 1);
    if (read_exact(fd, str, len) < 0) { free(str); return NULL; }
    str[len] = '\0';
    return str;
}

int main() {
    printf("=== Secure Chat Server ===\n");
    
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    struct sockaddr_in address = {AF_INET, htons(PORT), {INADDR_ANY}};
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 1);
    printf("Listening on port %d...\n", PORT);
    
    int addrlen = sizeof(address);
    int client_fd = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    printf("Client connected!\n");

    //  Generate DH params ONCE on server 
    mpz_t p, g;
    mpz_inits(p, g, NULL);
    dh_generate_params(p, g, 512);

    //  Send p and g to client 
    char* p_hex = mpz_get_str(NULL, 16, p);
    char* g_hex = mpz_get_str(NULL, 16, g);
    send_str(client_fd, p_hex);
    send_str(client_fd, g_hex);
    free(p_hex);
    free(g_hex);

    //  Server generates keypair 
    DHParty server_dh;
    mpz_inits(server_dh.p, server_dh.private_key, server_dh.public_key, NULL);
    mpz_set(server_dh.p, p);
    dh_generate_keypair(&server_dh, p, g);

    //  Receive client's public key 
    char* client_hex = recv_str(client_fd);
    mpz_t client_pub;
    mpz_init(client_pub);
    mpz_set_str(client_pub, client_hex, 16);
    free(client_hex);

    //  Send server's public key 
    char* server_hex = mpz_get_str(NULL, 16, server_dh.public_key);
    send_str(client_fd, server_hex);
    free(server_hex);

    //  Compute shared secret 
    mpz_t shared;
    mpz_init(shared);
    dh_compute_shared(shared, &server_dh, client_pub);

    //  Derive AES key 
    unsigned char shared_bytes[1024];
    size_t count;
    mpz_export(shared_bytes, &count, 1, 1, 0, 0, shared);
    unsigned char aes_key[16];
    sha256_hash(shared_bytes, count, aes_key);
    printf("Shared key established.\n");
    printf("AES key: ");
    for (int i = 0; i < 16; i++) printf("%02x", aes_key[i]);
    printf("\n\n");

    //  AES setup 
    AesKey aes;
    aes.key_len = 16;
    aes.key_bytes = malloc(16);
    memcpy(aes.key_bytes, aes_key, 16);
    uchar_t iv[16] = {0};
    AES_init_ctx_iv(&aes.ctx, aes.key_bytes, iv);

    //  Chat loop 
    while (1) {
        uint32_t orig_len_net;
        if (read_exact(client_fd, &orig_len_net, 4) < 0) break;
        uint32_t orig_len = ntohl(orig_len_net);

        int padded_len = ((orig_len + 32 + 15) / 16) * 16;
        unsigned char encrypted[BUFFER_SIZE];
        if (read_exact(client_fd, encrypted, padded_len) < 0) break;

        unsigned char decrypted[BUFFER_SIZE];
        aes_decrypt(encrypted, decrypted, padded_len, &aes);

        unsigned char received_hash[32];
        memcpy(received_hash, decrypted + orig_len, 32);

        unsigned char computed_hash[32];
        sha256_hash(decrypted, orig_len, computed_hash);

        if (memcmp(received_hash, computed_hash, 32) == 0) {
            decrypted[orig_len] = '\0';
            printf("Client: %s\n", decrypted);
        } else {
            printf("[CORRUPTED]\n");
        }

        if (orig_len == 3 && memcmp(decrypted, "bye", 3) == 0) break;
    }

    free(aes.key_bytes);
    close(client_fd);
    close(server_fd);
    dh_clear_party(&server_dh);
    mpz_clears(p, g, client_pub, shared, NULL);
    printf("Connection closed.\n");
    return 0;
}