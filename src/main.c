#include "ft_ping.h"

bool sigint;

void handle_sigint(int sig) {
    if (sig == SIGINT) {
        sigint = true;
    }
    return;
}

int main(void) {
    int     sockfd;
    t_host  host;

    signal(SIGINT, handle_sigint);

    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
        perror("Error creating the raw socket");
        return (EXIT_FAILURE);
    }
    ft_ping(sockfd, &host);
}
