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
#include "appconfig.h"
using namespace std;

#define DEST_PORT 80
#define DEST_IP_BY_NAME "dm.trtos.com"
#define GET_KEY "POST http://dm.trtos.com/php/value.php?name=servertool_key HTTP/1.1\r\n"
#define SET_KEY "POST http://dm.trtos.com/php/value.php?name=servertool_key&value=454545 HTTP/1.1\r\n";
#define SET_MAC "POST http://dm.trtos.com/php/value.php?name=servertool_mac&value=55:34:33:33:33 HTTP/1.1\r\n";


string http_get(int fd,string text) {
int send_num;
    char send_buf [] = "helloworld";
    char recv_buf [4096];
    char str1[4096];
    string res;
    while (1) {
        if(ap_debug)printf("begin send\n");
        memset(str1,0,4096);
        strcat(str1, text.c_str());
        strcat(str1,"Host: dm.trtos.com\r\n");
        strcat(str1,"Content-Length: 65\r\n");
        strcat(str1,"Content-Type: application/x-www-form-urlencoded\r\n");
        strcat(str1,"\r\n");
        strcat(str1,"mathod=adb_signe&token=0E1FEECD0EE54E3B8568A536A7036D78B1AC7EEE");
        strcat(str1,"\r\n\r\n");
        if(ap_debug)printf("str1 = %s\n",str1);
        send_num = send(fd, str1,strlen(str1),0);
        if (send_num < 0) {
            perror("send error");
            exit(1);
        } else {
            if(ap_debug)printf("send successful\n");
            if(ap_debug)printf("begin recv:\n");
            int recv_num = recv(fd,recv_buf,sizeof(recv_buf),0);
            if(recv_num < 0) {
                if(ap_debug)perror("recv");
                exit(1);
            } else {
                if(ap_debug)printf("recv sucess:%s\n",recv_buf);
                res=recv_buf;
            }
        }
        break;
        sleep(5);
    }
    close(fd);
    return res;
}

int connect_ip(string ip)
{
    int sock_fd;
    struct sockaddr_in addr_serv;
    sock_fd=socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
       std::cout << "sock error" << std::endl;
       exit(1);
    }
    memset(&addr_serv, 0, sizeof(addr_serv));
    addr_serv.sin_family = AF_INET;
    addr_serv.sin_port = htons(DEST_PORT);
    if(ap_debug)printf("Ip address=%s",ip.c_str());
    in_addr_t addr=inet_addr(ip.c_str());
    memcpy(&addr_serv.sin_addr, &(addr), sizeof(addr));
    if (connect(sock_fd, (struct sockaddr*)(&addr_serv), sizeof(addr_serv)) < 0)
    {
        perror("connect error\n");
        exit(1);
    }
    return sock_fd;
}
string http_getkey(string get)
{
    int fd=connect_ip(ap_serverip);
    string str=http_get(fd,get);
    if(ap_debug)printf("start=%d,end=%d\r\n",(int)str.find("["),(int)str.find("]"));
    str = str.substr(str.find("[")+1 ,str.find("]")-str.find("[")-1);
    return str;
}
void http_sendmac()
{
    char str1[512];
    int fd=connect_ip(ap_serverip);
    memset(str1,0,512);
    strcat(str1, "POST http://dm.trtos.com/php/value.php?name=servertool_mac&value=");
    strcat(str1, ap_mac.c_str());
    strcat(str1, " HTTP/1.1\r\n");
    string r=http_getkey(str1);
    if(ap_debug)std::cout <<r << std::endl;
}
void http_thread()
{
 
    while(true)
    {
        //string r=http_get(ap_sock_fd,get);
    // ..std::cout << r << std::endl;
    // string r=http_get(ap_sock_fd,set_mac);

        string r=http_getkey(GET_KEY);
        std::cout << "KEY:"<< r << std::endl;
    // std::cout << r << std::endl;
        r=encryption(r);
        std::cout << r << std::endl;
    }
}
