#include <lwip/sockets.h>
#include <lwip/netdb.h>
#include <stdio.h>
#include <string.h>

int get_ip_from_url(const char *url, char *ip_address, size_t ip_len) {
    struct addrinfo hints, *res;
    void *ptr;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(url, NULL, &hints, &res);
    if (status != 0 || res == NULL) {
        printf("DNS lookup failed: %s\n", gai_strerror(status));
        return -1;
    }

    struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
    ptr = &(ipv4->sin_addr);
    inet_ntop(res->ai_family, ptr, ip_address, ip_len);

    freeaddrinfo(res);
    return 0;
}
