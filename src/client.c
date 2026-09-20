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
    
    BigInt p, g;
    
    bigra9m_inits(&p, &g, NULL);
    bigra9m_assign_str(&p, p_hex);
    bigra9m_assign_str(&g, g_hex);
    free(p_hex);
    free(g_hex);

    //  Generate keypair using SERVER's p and g 
    DHParty client_dh;
    bigra9m_inits(&client_dh.p, &client_dh.private_key, &client_dh.public_key, NULL);
    bigra9m_assign(&client_dh.p, p);
    dh_generate_keypair(&client_dh, &p, &g);

    //  Send public key 
    char* client_hex = bigra9m_get_str( &client_dh.public_key);
    send_str(sock, client_hex);
    free(client_hex);

    //  Receive server's public key 
    char* server_hex = recv_str(sock);
    BigInt server_pub;
    bigra9m_init(&server_pub);
    bigra9m_assign_str(&server_pub, server_hex);
    free(server_hex);

    //  Compute shared secret 
    BigInt shared;
    bigra9m_init(&shared);
    dh_compute_shared(&shared, &client_dh, &server_pub);

    //  Derive AES key 
    unsigned char shared_bytes[1024];
    size_t count;
    bigra9m_export(shared_bytes, &count, 1 ,  &shared);
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
    printf("key set with code %d \n" , res ) ; 

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
        aes_encrypt(plaintext, encrypted, total_len,(void *) aes);
        int padded_len = ((total_len + 15) / 16) * 16;

        uint32_t net = htonl(orig_len);
        send(sock, &net, 4, 0);
        send(sock, encrypted, padded_len, 0);

        if (strcmp(input, "bye") == 0) break;
    }

    close(sock);
    dh_clear_party(&client_dh);
    bigra9m_clears(&p, &g, &client_pub, &shared, NULL);
    printf("Disconnected.\n");
    return 0;
}