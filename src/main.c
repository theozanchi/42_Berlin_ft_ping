#include "ft_ping.h"

bool sigint;

void handle_sigint(int sig) {
    if (sig == SIGINT) {
        sigint = true;
    }
    return;
}

int main(void) {
    signal(SIGINT, handle_sigint);
    ft_ping("");
}
