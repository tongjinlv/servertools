#include "tcp_server.h"
#include "log.h"


void tcp_thread1()
{
    I("tcp_thread1");
    tcp_server ts(8081);  
    ts.recv_msg();   
}  