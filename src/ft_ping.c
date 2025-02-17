#include "ft_ping.h"

extern bool sigint;

void fill_packet(struct ping_packet *pckt, int *msg_count) {
    int i;

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

void ft_ping(char *ip) {
    struct ping_packet  pckt;
    int                 msg_count = 0;

    while (!sigint) {
        fill_packet(&pckt, &msg_count);
    }
}
