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
#include "square.h"
using namespace std;

#define DEST_PORT 80
#define DEST_IP_BY_NAME "dm.trtos.com"


string http_get(int fd,string text) {
int send_num;
    char send_buf [] = "helloworld";
    char recv_buf [4096];
    char str1[4096];
    string res;
    while (1) {
        printf("begin send\n");
        memset(str1,0,4096);
        //strcat(str1, "POST http://dm.trtos.com/php/value.php?name=servertool_key HTTP/1.1\r\n");
        strcat(str1, text.c_str());
        strcat(str1,"Host: dm.trtos.com\r\n");
        strcat(str1,"Content-Length: 65\r\n");
        strcat(str1,"Content-Type: application/x-www-form-urlencoded\r\n");
        strcat(str1,"\r\n");
        strcat(str1,"mathod=adb_signe&token=0E1FEECD0EE54E3B8568A536A7036D78B1AC7EEE");
        strcat(str1,"\r\n\r\n");
       // printf("str1 = %s\n",str1);
        send_num = send(fd, str1,strlen(str1),0);
        if (send_num < 0) {
            perror("send error");
            exit(1);
        } else {
            printf("send successful\n");
            printf("begin recv:\n");
            int recv_num = recv(fd,recv_buf,sizeof(recv_buf),0);
            if(recv_num < 0) {
                perror("recv");
                exit(1);
            } else {
                printf("recv sucess:%s\n",recv_buf);
                res=recv_buf;
            }
        }
        break;
        sleep(5);
    }
    return res;
}
string mygetaddrinfo()
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
int connect()
{
    int sock_fd;
    struct sockaddr_in addr_serv;
    sock_fd=socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
        std::cout << "sock error" << std::endl;
       return 0;
    }
    memset(&addr_serv, 0, sizeof(addr_serv));
    addr_serv.sin_family = AF_INET;
    addr_serv.sin_port = htons(DEST_PORT);
    string mac=mygetaddrinfo();
    printf("Ip address=%s",mac.c_str());
    in_addr_t addr=inet_addr(mac.c_str());
    memcpy(&addr_serv.sin_addr, &(addr), sizeof(addr));
    if (connect(sock_fd, (struct sockaddr*)(&addr_serv), sizeof(addr_serv)) < 0)
    {
        perror("connect error\n");
        exit(1);
    }
    return sock_fd;
}
int http()
{
    int sock_fd;
    string get="POST http://dm.trtos.com/php/value.php?name=servertool_key HTTP/1.1\r\n";
    string set="POST http://dm.trtos.com/php/value.php?name=servertool_key&value=454545 HTTP/1.1\r\n";
    string set_mac="POST http://dm.trtos.com/php/value.php?name=servertool_mac&value=55:34:33:33:33 HTTP/1.1\r\n";
    sock_fd=connect();
    string r=http_get(sock_fd,get);
    std::cout << r << std::endl;
    r=http_get(sock_fd,set_mac);
    std::cout << r << std::endl;
    encryption("sds");
    return 0;
}
