#include "ft_ping.h"

extern bool sigint;

void fill_packet(t_ping_pckt *pckt, t_host *host) {
    long unsigned int i;

    bzero(pckt, sizeof(*pckt));
    pckt->hdr.type = ICMP_ECHO;
    pckt->hdr.un.echo.id = getpid();

    for (i = 0; i < sizeof(pckt->msg) - 1; i++) {
        pckt->msg[i] = i + '0';
    }
    pckt->msg[i] = 0;

    pckt->hdr.un.echo.sequence = host->msg_count++;
    pckt->hdr.checksum = checksum(&pckt, sizeof(pckt));
}

void send_packet(t_ping_pckt *pckt, int sockfd, t_host *host, bool *pckt_sent) {
    static struct addrinfo *curr_addr = NULL;

    if (!curr_addr) {
        curr_addr = host->results;
    }

    clock_gettime(CLOCK_MONOTONIC, &(host->sent));
    if (sendto(sockfd, &pckt, sizeof(pckt), 0, curr_addr->ai_addr, curr_addr->ai_addrlen) <= 0) {
        perror("Error while sending packet");
        curr_addr = curr_addr->ai_next;
    }
    printf("Pckt sent\n");
    *pckt_sent = true;
}

void recv_packet(int sockfd, t_host *host, bool *pckt_sent) {
    char                rbuffer[RECV_BUFFER_SIZE];
    struct sockaddr_in  r_addr;
    socklen_t           addr_len = sizeof(r_addr);

    if (recvfrom(sockfd, rbuffer, sizeof(rbuffer), 0, (struct sockaddr *)&r_addr, &addr_len) <= 0 && host->msg_count > 1) {
        perror("Error while receiving packet");
    } else {
        clock_gettime(CLOCK_MONOTONIC, &(host->received));
        printf("Pckt received\n");
        host->msg_count++;
    }
    (void)pckt_sent;
}

void ft_ping(int sockfd, t_host *host) {
    int             ttl_val = 64;
    struct timeval  recv_timeout;
                    recv_timeout.tv_sec = RECV_TIMEOUT;
                    recv_timeout.tv_usec = 0;
    t_ping_pckt     pckt;
    bool            pckt_sent;

    if (setsockopt(sockfd, SOL_IP, IP_TTL, &ttl_val, sizeof(ttl_val)) < 0) {
        perror("Error setting socket options to TTL");
        return;
    }
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&recv_timeout, sizeof(recv_timeout)) < 0) {
        perror("Error setting receive timeout value");
        return;
    }
    resolve_host(host);
    while (!sigint) {
        pckt_sent = false;
        fill_packet(&pckt, host);
        usleep(PING_SLEEP_RATE);
        send_packet(&pckt, sockfd, host, &pckt_sent);
        recv_packet(sockfd, host, &pckt_sent);
    }
}
