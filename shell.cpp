
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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

#define GET_HOST_MAC "cat /sys/class/net/eth0/address || cat /sys/class/net/eth1/address"



 
int set_system(const char* pCmd, char* pResult, int size)
{
   int fd[2];
   printf("a\r\n");
   if(pipe(fd))   {
      printf("pipe error!\n");
      return -1;
   }
   int bak_fd = dup(STDOUT_FILENO);
   int new_fd = dup2(fd[1], STDOUT_FILENO);
   printf("a\r\n");
   system(pCmd);
   printf("a\r\n");
   read(fd[0], pResult, size-1);
   printf("b\r\n");
   pResult[strlen(pResult)-1] = 0;
   dup2(bak_fd, new_fd);
   return 0;
}


string getshell(string text)
{
    string r;
    FILE* fp = NULL;
    char cmd[512];
    memset(cmd,0,512);
    strcat(cmd, GET_HOST_MAC);
    strcat(cmd, " 2>/dev/null; echo $?");
    if ((fp = popen(cmd, "r")) != NULL)
    {
        fgets(cmd, sizeof(cmd), fp);
        pclose(fp);
    }
    r=cmd;
    return r;
}

string getmac(void)
{
    string res=getshell(GET_HOST_MAC);
    return res;
}

