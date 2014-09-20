#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>


#include <signal.h>
#include <typeinfo>


#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <iphlpapi.h>

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

    struct sockaddr_in name;
    name.sin_family = PF_INET;
    name.sin_port = htons(30000);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(listener_d, (struct sockaddr *) &name, sizeof(name));

    //listen
    if(listen(listener_d, 10) == -1) {
        error("Can't listen");
    }
    puts("Waiting for connection");

    while(1) {
        //accept
        struct sockaddr_storage client_addr;
        int address_size = sizeof(client_addr);
        int connect_d = accept(listener_d, (struct sockaddr *)&client_addr, &address_size);
        char *msg = "Why, Hello there?";

        send(connect_d, msg, strlen(msg), 0);
        //close(connect_d);
        closesocket(connect_d);
    }
    return 0;
}