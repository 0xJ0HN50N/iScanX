#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
    if (argc != 5) {
        printf("usage: %s start_ip end_ip start_port end_port\n", argv[0]);
        return 1;
    }

    struct in_addr start_addr;
    struct in_addr end_addr;

    if (!inet_aton(argv[1], &start_addr)) return 1;
    if (!inet_aton(argv[2], &end_addr)) return 1;

    unsigned int start_ip = ntohl(start_addr.s_addr);
    unsigned int end_ip   = ntohl(end_addr.s_addr);

    int start_port = atoi(argv[3]);
    int end_port   = atoi(argv[4]);

    for (unsigned int ip = start_ip; ip <= end_ip; ip++) {

        struct in_addr current_addr;
        current_addr.s_addr = htonl(ip);
        char *ip_str = inet_ntoa(current_addr);

        for (int port = start_port; port <= end_port; port++) {

            int sock = socket(AF_INET, SOCK_STREAM, 0);
            if (sock < 0) continue;

            struct sockaddr_in target;
            target.sin_family = AF_INET;
            target.sin_port = htons(port);
            target.sin_addr = current_addr;

            if (connect(sock, (struct sockaddr *)&target, sizeof(target)) == 0) {
                printf("open %s:%d\n", ip_str, port);
                fflush(stdout);
            }

            close(sock);
        }
    }

    return 0;
}
