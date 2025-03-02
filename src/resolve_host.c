#include "ft_ping.h"

void resolve_host(t_host *host) {
    struct addrinfo hints;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    if (getaddrinfo(host->host, NULL, &hints, &(host->results)) != 0) {
        perror("Error while resolving host");
    }
}
