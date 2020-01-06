#include "tcp_server.h"
#include <math.h>
#include <stdio.h>
#include <cstring>
#include <memory.h> 
#include <iostream>  
#include <stdlib.h>
#include "log.h"
#include "shell.h"
#include "http.h"
using namespace std;

tcp_server::tcp_server(int listen_port) {
        I("tcp_server::tcp_server({})",listen_port);
        if(( socket_fd = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0 ){
                E("socket() failed");
        }
        memset(&myserver,0,sizeof(myserver));
        myserver.sin_family = AF_INET;
        myserver.sin_addr.s_addr = htonl(INADDR_ANY);
        myserver.sin_port = htons(listen_port);

        if( bind(socket_fd,(sockaddr*) &myserver,sizeof(myserver)) < 0 ) {
                E("bind() failed");
        }

        if( listen(socket_fd,10) < 0 ) {
                E("listen() failed");
        }
}
string tcp_server::task(string msg)
{
        if(strcmp(msg.c_str(),"getmac")==0)return getmac();
        if(strcmp(msg.c_str(),"key:")>0)
        {
                msg=msg.erase(0, 4);
                I("Revice={}",msg);
                cmd_export(msg);
                return "ok";
        }
        return "cmd error";
}
int tcp_server::recv_msg() {

        socklen_t sin_size = sizeof(struct sockaddr_in);
        if(( accept_fd = accept(socket_fd,(struct sockaddr*) &remote_addr,&sin_size)) == -1 )
        {
                        E("Accept error!");
        }
        I("Received a connection from {}\n",inet_ntoa(remote_addr.sin_addr));

        char buffer[MAXSIZE];
        memset(buffer,0,MAXSIZE);
        if( ( read(accept_fd,buffer,MAXSIZE)) < 0 ) {
                E("Read() error!");
        } else {
                I("Received message: {}\n",buffer);
                string msg=buffer;
                string res=task(msg);
                if(res.size()!=0)
                {
                        if( send( accept_fd,res.c_str(),res.size(),0 ) < 0 ) 
                        {  
                                I("send message error\n");  
                        }  
                }
                
        }
        close(accept_fd);
        return 0;
}

