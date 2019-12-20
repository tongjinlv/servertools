#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_

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
extern string ap_serverip;
extern string ap_serverurl;
extern string ap_mac;
extern bool ap_debug;

int appconfig( int argc, char **argv);

#endif
