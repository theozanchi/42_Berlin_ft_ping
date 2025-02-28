#include "ft_ping.h"

extern bool sigint;

unsigned short checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char *)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

void fill_packet(t_ping_pckt *pckt, int *msg_count) {
    long unsigned int i;

    bzero(pckt, sizeof(*pckt));
    pckt->hdr.type = ICMP_ECHO;
    pckt->hdr.un.echo.id = getpid();

    for (i = 0; i < sizeof(pckt->msg) - 1; i++) {
        pckt->msg[i] = i + '0';
    }
    pckt->msg[i] = 0;

    pckt->hdr.un.echo.sequence = (*msg_count)++;
    pckt->hdr.checksum = checksum(&pckt, sizeof(pckt));
}

void send_packet(t_ping_pckt *pckt, int sockfd, t_host *host, bool *pckt_sent) {
    struct sockaddr_in  addr;
                        addr.sin_family = AF_INET;
                        addr.sin_addr.s_addr = host->ip;
                        addr.sin_port = 0;

    clock_gettime(CLOCK_MONOTONIC, &(host->sent));
    if (sendto(sockfd, &pckt, sizeof(pckt), 0, (struct sockaddr*)&addr, sizeof(addr)) <= 0) {
        perror("Error while sending packet");
        *pckt_sent = true;
    }
}

void recv_packet(int sockfd, t_host *host, bool *pckt_sent, int *msg_count) {
    char                rbuffer[RECV_BUFFER_SIZE];
    struct sockaddr_in  r_addr;

    if (recvfrom(sockfd, rbuffer, sizeof(rbuffer), 0, (struct sockaddr_in *)&r_addr, sizeof(r_addr)) <= 0 && *msg_count > 1) {
        perror("Error while receiving packet");
    } else {
        clock_gettime(CLOCK_MONOTONIC, &(host->received));
    }
}

void ft_ping(int sockfd, t_host *host) {
    int             msg_count = 0;
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
    while (!sigint) {
        pckt_sent = false;
        fill_packet(&pckt, &msg_count);
        usleep(PING_SLEEP_RATE);
        send_packet(&pckt, sockfd, host, &pckt_sent);
        printf("Timestamp: %li.%li\n", host->sent.tv_sec, host->sent.tv_nsec);
        recv_packet(sockfd, host, &pckt_sent, &msg_count);
    }
}
