#ifndef FT_PING_H
# define FT_PING_H

#include <stdio.h>
#include <signal.h>
#include <stdbool.h>

// ft_ping.c
void ft_ping(char *ip);

// main.c
void handle_sigint(int sig);

#endif
