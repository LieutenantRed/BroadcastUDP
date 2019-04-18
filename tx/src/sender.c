#include <stdio.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 255

#define handle_error(msg) \
        do { fprintf(stderr, msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char **argv)
{
        if (argc != 3)
                handle_error("Usage: [IP] [port]\n");
        char* ip = argv[1];
        uint16_t port = atoi(argv[2]);
        
        int sock;
        if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
                handle_error("socket corrupted");

        int broadcast = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast,
            sizeof(broadcast)) < 0)
                handle_error("setsockopt corrupted");

        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(struct sockaddr_in));

        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(ip);
        addr.sin_port = htons(port);

        while (1) {
                char buffer[BUFFER_SIZE];
                fgets(buffer, BUFFER_SIZE, stdin);
                sendto(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&addr, sizeof(addr));
        }
}
