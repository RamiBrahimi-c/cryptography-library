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

int main(int argc, char** argv) {
    printf("=== Secure Chat Client ===\n");
    
    const char* server_ip = "127.0.0.1";
    if (argc > 1) server_ip = argv[1];

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr = {AF_INET, htons(PORT)};
    inet_pton(AF_INET, server_ip, &serv_addr.sin_addr);
    
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect failed");
        return 1;
    }
    printf("Connected to %s!\n\n", server_ip);

    //  Receive p and g from server ---
    char* p_hex = recv_str(sock);
    char* g_hex = recv_str(sock);
    
    mpz_t p, g;
    mpz_inits(p, g, NULL);
    mpz_set_str(p, p_hex, 16);
    mpz_set_str(g, g_hex, 16);
    free(p_hex);
    free(g_hex);

    //  Generate keypair using SERVER's p and g 
    DHParty client_dh;
    mpz_inits(client_dh.p, client_dh.private_key, client_dh.public_key, NULL);
    mpz_set(client_dh.p, p);
    dh_generate_keypair(&client_dh, p, g);

    //  Send public key 
    char* client_hex = mpz_get_str(NULL, 16, client_dh.public_key);
    send_str(sock, client_hex);
    free(client_hex);

    //  Receive server's public key 
    char* server_hex = recv_str(sock);
    mpz_t server_pub;
    mpz_init(server_pub);
    mpz_set_str(server_pub, server_hex, 16);
    free(server_hex);

    //  Compute shared secret 
    mpz_t shared;
    mpz_init(shared);
    dh_compute_shared(shared, &client_dh, server_pub);

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
    char input[BUFFER_SIZE];
    while (1) {
        printf("You: ");
        if (!fgets(input, BUFFER_SIZE, stdin)) break;
        input[strcspn(input, "\n")] = 0;
        if (strlen(input) == 0) continue;

        int orig_len = strlen(input);
        unsigned char hash[32];
        sha256_hash((unsigned char*)input, orig_len, hash);

        int total_len = orig_len + 32;
        unsigned char plaintext[BUFFER_SIZE];
        memcpy(plaintext, input, orig_len);
        memcpy(plaintext + orig_len, hash, 32);

        unsigned char encrypted[BUFFER_SIZE];
        aes_encrypt(plaintext, encrypted, total_len, &aes);
        int padded_len = ((total_len + 15) / 16) * 16;

        uint32_t net = htonl(orig_len);
        send(sock, &net, 4, 0);
        send(sock, encrypted, padded_len, 0);

        if (strcmp(input, "bye") == 0) break;
    }

    free(aes.key_bytes);
    close(sock);
    dh_clear_party(&client_dh);
    mpz_clears(p, g, server_pub, shared, NULL);
    printf("Disconnected.\n");
    return 0;
}