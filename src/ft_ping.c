#include "ft_ping.h"

extern bool sigint;

void ft_ping(char *ip) {
    (void)ip;
    while (!sigint) {
        printf("sthg happening\n");
    }
}
