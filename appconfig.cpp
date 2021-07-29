

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
#include "shell.h"
#include "log.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#define DEST_PORT 80
#define DEST_IP_BY_NAME "dm.trtos.com"
#define SOFT_VERSION "V0.1.1 \r\n"
string ap_serverip;
string ap_serverurl;
string ap_mac;
string ap_key;
bool ap_debug;



//配置参数
int appconfig( int argc, char **argv )
{
    ap_debug=false;
    for(int i=0;i<argc;i++)
    {
        if(strcmp(argv[i],"debug")==0)ap_debug=true;
        if(strcmp(argv[i],"log")==0)LOG(stoi(argv[i+1]));
        
        if(strcmp(argv[i],"--version")==0){printf("%s",SOFT_VERSION);exit(0);}
    }
    ap_serverurl=DEST_IP_BY_NAME;
    return 0;
}