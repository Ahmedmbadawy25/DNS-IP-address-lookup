#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

int main (int argc, char *argv[]){
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET6_ADDRSTRLEN];

    if (argc < 2) {
        printf("Please provide a DNS name to lookup");
        return 1;
    }

    for (int i =1; i < argc; i++) {
        printf( "DNS name: %s\n", argv[i]);

        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        if ((status = getaddrinfo(argv[i], NULL, &hints, &res)) != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
            continue;
        }
        for (p = res; p != NULL; p = p->ai_next) {
            void *addr;
            char *ipver;

            if (p->ai_family == AF_INET) {
                struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
                addr = &(ipv4->sin_addr);
                ipver = "IPv4";
            } else {
                struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
                addr = &(ipv6->sin6_addr);
                ipver = "IPv6";
            }
            inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
            printf(" %s: %s \n", ipver, ipstr);
        }
        freeaddrinfo(res);
    }
    return 0;
}