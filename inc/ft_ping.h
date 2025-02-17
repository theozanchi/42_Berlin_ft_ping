#ifndef FT_PING_H
# define FT_PING_H

#include <stdio.h>
#include <signal.h>
#include <stdbool.h>
#include <netinet/ip_icmp.h>

#define PING_PACKET_SIZE 64
#define PORT_NUMBER 0
#define PING_SLEEP_RATE 1000000
#define RECV_TIMEOUT 1

struct ping_packet {
    struct icmphdr  hdr;
    char            msg[PING_PACKET_SIZE - sizeof(struct icmphdr)];
};

// ft_ping.c
void ft_ping(char *ip);

// main.c
void handle_sigint(int sig);

#endif
