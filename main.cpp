#include <iostream>
#include <sys/socket.h>
#include <sys/errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

void error(char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
}

int catch_signal(int sig, void (*handler) (int))
{
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    return sigaction(sig, &action, NULL);
}

void end_server(int sig)
{
    printf("\nShutting down the server");
    exit(0);
}

int main() {
    catch_signal(SIGINT, end_server);

    //bind to a port
    int listener_d = socket(PF_INET, SOCK_STREAM, 0);
    if(listener_d == -1) {
        error("Can't open socket");
    }

    //listen
    if(listen(listener_d, 10) == -1) {
        error("Can't listen");
    }

    //accept
    struct sockaddr_storage client_addr;
    socklen_t address_size = sizeof(client_addr);
    int connect_d = accept(listener_d, (struct sockaddr *)&client_addr, &address_size);
    if(connect_d == -1)
        error("Can't open secondary socket");
  return 0;
}