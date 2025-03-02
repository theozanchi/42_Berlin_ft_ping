#include "ft_ping.h"

bool sigint;

void handle_sigint(int sig) {
    if (sig == SIGINT) {
        sigint = true;
    }
    return;
}

void init_data(char *host_string, t_host **host_struct) {
    *host_struct = malloc(sizeof(t_host));
    (*host_struct)->host = strdup(host_string);
}

int main(int argc, char **argv) {
    int     sockfd;
    t_host  *host;

    (void)argc;

    signal(SIGINT, handle_sigint);
    init_data(argv[1], &host);
    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
        perror("Error creating the raw socket");
        return (EXIT_FAILURE);
    }
    ft_ping(sockfd, host);
}
