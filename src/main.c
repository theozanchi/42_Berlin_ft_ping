#include "ft_ping.h"

bool sigint;

void handle_sigint(int sig) {
    if (sig == SIGINT) {
        sigint = true;
    }
    return;
}

int main(void) {
    int sockfd;
    signal(SIGINT, handle_sigint);

    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
        printf("Error creating the socket\n");
    }
    ft_ping("");
}
