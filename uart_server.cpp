#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include<vector>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "tcp_server.h"
#include "uart_server.h"
#include "log.h"
#include "square.h"

using namespace std;
 


void handpipe(int sig)
{
}
int socketListen = 0;

vector<MySocketInfo> RaspySocket;
 
MySocketInfo more;
MySocketInfo last;
 
pthread_rwlock_t rwlock_RaspySocket = PTHREAD_RWLOCK_INITIALIZER;

int uart_server(int argc, char *argv[])
{   
    signal(SIGPIPE, handpipe);
 
    /* 创建TCP连接的Socket套接字 */
    socketListen = socket(AF_INET, SOCK_STREAM, 0);
    if(socketListen < 0){
        E("创建TCP套接字失败");
        return -1;
    }
 
    int on = 1;
    setsockopt(socketListen, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    /* 填充服务器端口地址信息，以便下面使用此地址和端口监听 */
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(struct sockaddr_in));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY); /* 这里地址使用全0，即所有 */
    server_addr.sin_port=htons(6666);
 
    if(bind(socketListen, (struct sockaddr *)&server_addr,sizeof(struct sockaddr)) != 0){
        E("绑定ip地址、端口号失败");
        return -2;
    }
    /* 开始监听相应的端口 */
    if(listen(socketListen, 100) != 0){
        I("开启监听失败");
        return -3;
    }else{
        I("服务器端开始监听");
    }
 
    pthread_t thrAccept_pthread, showState_pthread;
    pthread_create(&thrAccept_pthread, NULL, fun_thrAcceptHandler, &socketListen);      //新建一个线程，接受设备连接
    pthread_create(&showState_pthread, NULL, fun_ShowState, NULL);
 
  // pthread_t send_pthread;
   // pthread_create(&send_pthread, NULL, fun_SendInfo, NULL);
 
    while (true) {
        sleep(1);
    }
}
 
void *fun_thrAcceptHandler(void *socketListen)
{
    int socketCon = -1;
 
    while(true)
    {
        int sockaddr_in_size = sizeof(struct sockaddr_in);
        struct sockaddr_in client_addr;
        int _socketListen = *((int *)socketListen);
 
        struct timeval timeout={20,0};
        setsockopt(_socketListen,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(timeout));
        setsockopt(_socketListen,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));
 
        socketCon = accept(_socketListen, (struct sockaddr *)(&client_addr), (socklen_t *)(&sockaddr_in_size));
        if(socketCon >= 0)//int accept(int sockfd,struct sockaddr* addr,socklen_t* addrlen);
        {                 //sockfd：套接字描述符，该套接口在listen()后监听连接。addr：（可选）指针，指向一缓冲区，其中接收为通讯层所知的连接实体的地址。Addr参数的实际格式由套接口创建时所产生的地址族确定。addrlen：（可选）指针，输入参数，配合addr一起使用，指向存有addr地址长度的整型数。
 
            printf("连接成功 ip: %s:%d , 连接套接字为：%d\n",inet_ntoa(client_addr.sin_addr), client_addr.sin_port, socketCon);
 
            MySocketInfo socketInfo;
            socketInfo.socketCon = socketCon;
            socketInfo.ipaddr = inet_ntoa(client_addr.sin_addr);
            socketInfo.port = client_addr.sin_port;
 
            pthread_t thrReceive = 0;
            pthread_create(&thrReceive, NULL, fun_thrReceiveHandler, &socketInfo);
 
        }
 
        usleep(20);
    }
 
    close(socketCon);
    std::cout<< "socket 服务器监听线程退出成功"<< std::endl;
    return NULL;
}
 
void *fun_thrReceiveHandler(void *socketInfo)
{
    char buffer[1024];
    long buffer_length;
    MySocketInfo _socketInfo = *((MySocketInfo *)socketInfo);
 
    while(true)
    {
        memset(buffer, 0x00, sizeof(buffer));
 
        buffer_length = recv(_socketInfo.socketCon, buffer, 1024, 0);		//接收信息，判断是什么设备,第一个参数为指定接收端套接字描述符；
        if(buffer_length == 0)
        {
            I("{},{} 客户端关闭1\n", _socketInfo.ipaddr.c_str(), _socketInfo.port);
            if(RaspySocket.size() > 0) {
                vector<MySocketInfo>::iterator it;
                for(it=RaspySocket.begin();it!=RaspySocket.end();it++) {
                    if(it->ipaddr == _socketInfo.ipaddr) {
                        it->status = "off-line";
                    }
                }
            }
            break;
        }
        else if(buffer_length < 0)
        {
            usleep(10);
            continue;
        }
        else
        {
            I("message:{}",buffer);//info_macAddress_ipAddress_505505_ff00aa00
            vector<string> tmp_status = split_c(buffer, "_");
 
            if(tmp_status.size() == 6) {//创建
                if(RaspySocket.size() == 0) {
                    _socketInfo.macAddr = tmp_status[1];
                    _socketInfo.ipaddr = tmp_status[2];
                    _socketInfo.storeCount = stol(tmp_status[3]);
                    _socketInfo.roomNumber = tmp_status[4];
                    _socketInfo.context = tmp_status[5];
                    _socketInfo.status = "on-line";
                    _socketInfo.bagCount++;
                    RaspySocket.push_back(_socketInfo);
                } else {
                    bool flag = false;
 
                    for(int i=0;i<RaspySocket.size();i++) {//更新
                        if(RaspySocket[i].macAddr == tmp_status[1]) {
                            RaspySocket[i].macAddr = tmp_status[1];
                            RaspySocket[i].ipaddr = tmp_status[2];
                            RaspySocket[i].storeCount = stol(tmp_status[3]);
                            RaspySocket[i].roomNumber = tmp_status[4];
                            _socketInfo.macAddr = tmp_status[1];
                            _socketInfo.ipaddr = tmp_status[2];
                            _socketInfo.roomNumber = tmp_status[4];
                            _socketInfo.context = tmp_status[5];
                            RaspySocket[i].context = tmp_status[5];
                            RaspySocket[i].status = "on-line";
                            RaspySocket[i].bagCount++;
                            RaspySocket[i].socketCon=_socketInfo.socketCon;
                            RaspySocket[i].ipaddr=_socketInfo.ipaddr;
                            RaspySocket[i].port=_socketInfo.port;
                            RaspySocket[i].macAddr=_socketInfo.macAddr;
                            flag = true;
                            break;
                        }
                    }
 
                    if(!flag) {//添加
                        _socketInfo.macAddr = tmp_status[1];
                        _socketInfo.ipaddr = tmp_status[2];
                        _socketInfo.storeCount = stol(tmp_status[3]);
                        _socketInfo.roomNumber = tmp_status[4];
                        _socketInfo.context = tmp_status[5];
                        _socketInfo.status = "on-line";
                        _socketInfo.bagCount++;
                        RaspySocket.push_back(_socketInfo);
                    }
                }
                if(tmp_status[0] == "msg")
                {
                    for(int i=0;i<RaspySocket.size();i++) {//更新
                        if(RaspySocket[i].macAddr != _socketInfo.macAddr)//排除自己
                        if(RaspySocket[i].ipaddr != _socketInfo.ipaddr)//排除自己
                        if(RaspySocket[i].roomNumber==_socketInfo.roomNumber) {
                            if(RaspySocket[i].status == "on-line")//只发送给在线的
                            {
                                char buffer[1024] = {'0'};
                                sprintf(buffer, "%s_%s_%s",_socketInfo.macAddr.c_str(),_socketInfo.ipaddr.c_str(),_socketInfo.context.c_str());
                                if(send(RaspySocket[i].socketCon, buffer, strlen(buffer), 0) <= 0)
                                {
                                    char buffer[1024] = {'0'};
                                    sprintf(buffer, "send fail(_%s_%s)", RaspySocket[i].macAddr.c_str(), RaspySocket[i].ipaddr.c_str());
                                    printf("信息发送失败\n");
                                    send(_socketInfo.socketCon,buffer,strlen(buffer),0);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
 
    printf("连接 %s:%d 客户端 的线程结束了\n", _socketInfo.ipaddr.c_str(), _socketInfo.port);
    close(_socketInfo.socketCon);
 
    return NULL;
}
 
void *fun_ShowState(void *tranInput)	//定时刷新屏幕，显示当前设备工作情况
{
    sleep(3);
    while (true)
    {
        if(system("clear") < 0) {
            sleep(1);
            continue;
        }
 
 
        std::cout<< std::endl<< "-------------------------------------------------------------------------------------"<<std::endl;
 
        time_t now;                     //实例化time_t结构
        tm *timenow;                    //实例化tm结构指针
        time(&now);                     //time函数读取现在的时间(国际标准时间非北京时间)，然后传值给now
        timenow = localtime(&now);      //localtime函数把从time取得的时间now换算成你电脑中的时间(就是你设置的地区)
        printf("%04d-%02d-%02d   %02d:%02d:%02d\n", timenow->tm_year+1900, timenow->tm_mon + 1, timenow->tm_mday, timenow->tm_hour, timenow->tm_min, timenow->tm_sec);
        std::cout<< "-------------------------------------------------------------------------------------"<<std::endl;
        std::cout << "编号\tmac地址\t\t\tIP地址\t\t图片数量\t房间号\t状态\t计数\t数据" << std::endl;
        pthread_rwlock_rdlock(&rwlock_RaspySocket);
        vector<MySocketInfo> RaspySocketTemp;
        if(RaspySocket.size()) {
            RaspySocketTemp.push_back(RaspySocket[0]);
 
            for(int i=1; i<RaspySocket.size(); i++) {
                vector<MySocketInfo>::iterator it;
 
                for (it = RaspySocketTemp.begin(); it != RaspySocketTemp.end(); it++) {
                    if(it->storeCount < RaspySocket[i].storeCount)
                        break;
                }
                if(it != RaspySocketTemp.end()) {
                    RaspySocketTemp.insert(it, RaspySocket[i]);
                } else {
                    RaspySocketTemp.push_back(RaspySocket[i]);
                }
            }
 
        }
        pthread_rwlock_unlock(&rwlock_RaspySocket);
        if(RaspySocketTemp.size() > 0) {
            for (int i = 0; i < RaspySocketTemp.size(); i++)
            {
                printf("No.%-3d\t%s\t%s\t%ld\t%s\t%s\t%ld\t%s\n",
                       i+1, RaspySocketTemp[i].macAddr.c_str(), RaspySocketTemp[i].ipaddr.c_str(),
                       RaspySocketTemp[i].storeCount,RaspySocketTemp[i].roomNumber.c_str(),RaspySocketTemp[i].status.c_str(),
                       RaspySocketTemp[i].bagCount,RaspySocketTemp[i].context.c_str());
            }
            if(RaspySocketTemp.size() >= 2) {
                more = RaspySocketTemp[0];
                last = RaspySocketTemp[RaspySocketTemp.size() - 1];
            }
        }
 
        RaspySocketTemp.clear();
 
        std::cout << "-------------------------------------------------------------------------------------"<<std::endl;
        sleep(1);
    }
    std::cout << "显示进程退出成功"<<std::endl;
    return 0;
}
 
void *fun_SendInfo(void *argv)
{
    sleep(5);
    while (true) {
        if(!more.ipaddr.empty() && !last.ipaddr.empty() && more.status != "off-line" && last.status != "off-line") {
 
            cout<<more.ipaddr<<" "<<last.ipaddr<<endl;
 
            int socketfd = socket(AF_INET, SOCK_STREAM, 0);    //创建TCP连接的Socket套接字
            if(socketfd < 0){
                std::cout<< "创建TCP连接套接字失败"<< std::endl;
                close(socketfd);
                continue;
            }
            // 填充客户端端口地址信息，以便下面使用此地址和端口监听
            struct sockaddr_in server_addr;
            bzero(&server_addr,sizeof(struct sockaddr_in));
            server_addr.sin_family=AF_INET;
            cout<<more.ipaddr<<endl;
            server_addr.sin_addr.s_addr=inet_addr(more.ipaddr.c_str());
            server_addr.sin_port=htons(7777);
 
            struct timeval timeout={5,0};
            setsockopt(socketfd,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(timeout));
            setsockopt(socketfd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));
 
            printf("等待连接\n");
 
            int res_con = connect(socketfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr));
            if(res_con == 0){
                printf("连接成功,连接结果为：%d\n",res_con);
            }
 
            char buffer[1024] = {'0'};
            sprintf(buffer, "sub_%s_%s_%ld", last.macAddr.c_str(), last.ipaddr.c_str(), last.storeCount);
 
            std::cout<<buffer<<std::endl;
 
            if(send(socketfd, buffer, strlen(buffer), 0) <= 0)
            {
                printf("信息发送失败\n");
            }
 
            close(socketfd);
        }
        sleep(10);
    }
 
}