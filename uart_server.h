#include <unistd.h>  
#include <iostream>  
#include <sys/socket.h>  
#include <arpa/inet.h>  

using namespace std;

struct MySocketInfo
{
    int socketCon; //不显示
    string ipaddr;
    int port; //不显示
    string macAddr;
    unsigned long storeCount;    //存储的图像数量
    string roomNumber;
    string context;
    string status;//状态
    unsigned long bagCount;
};
 
void *fun_thrAcceptHandler(void *socketListen);
void *fun_thrReceiveHandler(void *socketInfo);
void *fun_ShowState(void *tranInput);
void *fun_SendInfo(void *argv);
 

int uart_server(int argc, char *argv[]);