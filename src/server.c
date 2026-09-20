#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "ciphers/asymmetric/dh.h"
#include "ciphers/symmetric/aes.h"
#include "ciphers/hashing/hash.h"

#define PORT 8080
#define BUFFER_SIZE 1024

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
    BigInt p, g;
    bigra9m_inits(&p, &g, NULL);
    dh_generate_params(&p, &g, 512);

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //  Send p and g to client
    char* p_str = bigra9m_get_str(&p);
    char* g_str = bigra9m_get_str(&g);
    send_str(client_fd, p_str);
    send_str(client_fd, g_str);
    free(p_str);
    free(g_str);


    //  Server generates keypair
    DHParty server_dh;
    bigra9m_inits(&server_dh.p, &server_dh.private_key, &server_dh.public_key, NULL);
    bigra9m_assign(&server_dh.p, p);

    dh_generate_keypair(&server_dh, &p, &g);

    //  Receive client's public key
    char* client_hex = recv_str(client_fd);
    BigRa9m client_pub;
    bigra9m_init(&client_pub);

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    bigra9m_assign_str(&client_pub, client_hex);
    free(client_hex);

    //  Send server's public key
    char* server_hex = bigra9m_get_str(&server_dh.public_key);
    send_str(client_fd, server_hex);
    free(server_hex);

    //  Compute shared secret
    BigRa9m shared;
    bigra9m_init(&shared);
    dh_compute_shared(&shared, &server_dh, &client_pub);

    //  Derive AES key
    unsigned char shared_bytes[1024];
    size_t count;
    bigra9m_export( shared_bytes, &count , 1, &shared);
    unsigned char aes_key[16];
    sha256_hash(shared_bytes, count, aes_key);
    printf("Shared key established.\n");
    printf("AES key: ");
    for (int i = 0; i < 16; i++) printf("%02x", aes_key[i]);
    printf("\n\n");

    //  AES setup
    AesKey *aes = malloc(sizeof(AesKey));
    if (aes == NULL) {
        fprintf(stderr , "ERROR: malloc failed to allocate %ld bytes\n" ,sizeof(AesKey) ) ;
        exit(EXIT_FAILURE) ; 
    }

    int res =  aes_set_key((void *) aes ,  aes_key , 16 );


    //  Chat loop
    while (1) {
        uint32_t orig_len_net;
        if (read_exact(client_fd, &orig_len_net, 4) < 0) break;
        uint32_t orig_len = ntohl(orig_len_net);

        int padded_len = ((orig_len + 32 + 15) / 16) * 16;
        unsigned char encrypted[BUFFER_SIZE];
        if (read_exact(client_fd, encrypted, padded_len) < 0) break;

        unsigned char decrypted[BUFFER_SIZE];
        aes_decrypt(encrypted, decrypted, padded_len, (void *) aes);

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

    close(client_fd);
    close(server_fd);
    dh_clear_party(&server_dh);
    bigra9m_clears(&p, &g, &client_pub, &shared, NULL);
    printf("Connection closed.\n");
    return 0;
}
