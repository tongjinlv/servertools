

#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h> 
using namespace std;
#include "appconfig.h"
#include "http.h"


#define DEST_PORT 80
#define DEST_IP_BY_NAME "dm.trtos.com"
string ap_serverip;
string ap_serverurl;
int ap_sock_fd;


string appgetipbyname(string name)
{
    struct addrinfo *ai, *aip;
    struct addrinfo hint;
    struct sockaddr_in *sinp;
    const char *addr;
    int err;
    char buf[1024];
    hint.ai_flags = AI_CANONNAME;
    hint.ai_family = 0;
    hint.ai_socktype = 0;
    hint.ai_protocol = 0;
    hint.ai_addrlen = 0;
    hint.ai_canonname = NULL;
    hint.ai_addr = NULL;
    hint.ai_next = NULL;
    if((err = getaddrinfo(DEST_IP_BY_NAME, NULL, &hint, &ai)) != 0)printf("ERROR: getaddrinfo error: %s\n", gai_strerror(err));
    for(aip = ai; aip != NULL; aip = aip->ai_next)
    {
        printf("Canonical Name: %s\n", aip->ai_canonname);
        if(aip->ai_family == AF_INET)
        {
            sinp = (struct sockaddr_in *)aip->ai_addr;
            addr = inet_ntop(AF_INET, &sinp->sin_addr, buf, sizeof buf);
            return addr;
        }
        printf("\n");
    }
    return 0;
}

int appconfig( int argc, char **argv )
{
    ap_serverurl=DEST_IP_BY_NAME;
    ap_serverip=appgetipbyname(ap_serverurl);
    ap_sock_fd=connect_ip(ap_serverip);
    return 0;
}