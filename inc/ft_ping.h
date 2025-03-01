#ifndef FT_PING_H
# define FT_PING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <netinet/in.h>
#include <strings.h>
#include <time.h>
#include <sys/time.h>
#include <netdb.h>
#include <sys/types.h>

#define PING_PACKET_SIZE 64
#define PORT_NUMBER 0
#define PING_SLEEP_RATE 1000000
#define RECV_TIMEOUT 1
#define RECV_BUFFER_SIZE 128

typedef struct s_host {
    char            *host;
    struct addrinfo *results;
    int             msg_count;
    struct timespec first_sent;
    struct timespec sent;
    struct timespec received;
    struct timespec last_received;
    double          min_time_us;
    double          max_time_us;
} t_host;

typedef struct s_ping_packet {
    struct icmphdr  hdr;
    char            msg[PING_PACKET_SIZE - sizeof(struct icmphdr)];
} t_ping_pckt;

// ft_ping.c
void fill_packet(t_ping_pckt *pckt, t_host *host);
void send_packet(t_ping_pckt *pckt, int sockfd, t_host *host, bool *pckt_sent);
void recv_packet(int sockfd, t_host *host, bool *pckt_sent);
void ft_ping(int sockfd, t_host *host);

// main.c
void handle_sigint(int sig);

// resolve_host.c
void resolve_host(t_host *host);

// utils.c
unsigned short checksum(void *b, int len);

#endif
