
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

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
 
#include <string.h>
#include <errno.h>
#include "appconfig.h"
#include "ini.h"
#include "log.h"
using namespace std;

#define GET_HOST_MAC "cat /sys/class/net/eth0/address || cat /sys/class/net/eth1/address"
#define ALLOW_PORT "allowPortList='%s %s %s';for port in $allowPortList;do iptables -I INPUT -p tcp --dport $port -j ACCEPT;done"
#define CHECK_SH_FILE "/etc/servertool/check.sh"
#define CHECK_SH_DIR "/etc/servertool/"

 
int set_system(const char* pCmd, char* pResult, int size)
{
   int fd[2];
   I("a\r\n");
   if(pipe(fd))   {
      I("pipe error!\n");
      return -1;
   }
   int bak_fd = dup(STDOUT_FILENO);
   int new_fd = dup2(fd[1], STDOUT_FILENO);
   I("a\r\n");
   system(pCmd);
   I("a\r\n");
   read(fd[0], pResult, size-1);
   I("b\r\n");
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
    strcat(cmd, text.c_str());
    strcat(cmd, " 2>/dev/null;echo 'finished'; echo $?");
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
    string cmd="allowPortList='"+port+"';for port in $allowPortList;do /sbin/iptables -D INPUT -p tcp --dport $port -j DROP;/sbin/iptables -I INPUT -p tcp --dport $port -j ACCEPT >>/dev/null 2>&1;done";
    if(ap_debug)cout<<cmd<<endl;
    string res=getshell(cmd);
    return res;
}

string deleteport(string port)
{
    string cmd="deletePortList='"+port+"';for port in $deletePortList;do /sbin/iptables -D INPUT -p tcp --dport $port -j ACCEPT >>/dev/null 2>&1;/sbin/iptables -I INPUT -p tcp --dport $port -j DROP >>/dev/null 2>&1;done";
    if(ap_debug)cout<<cmd<<endl;
    string res=getshell(cmd);
    return res;
}

string&  replace_all(string&   str, const  string&  old_value, const  string&  new_value)     
{     
    I("replace_all {},{}",old_value,new_value);
    int i=10;
    while(i--)   
   {     
        string::size_type   pos(0);     
        if((pos=str.find(old_value)) != string::npos)     
        { 
            str.replace(pos,old_value.length(),new_value);  
        }   
        else{break;}
    }  
    return str;     
}   
string createuser(string un)
{
    string shell="(/bin/sed -i '/\\[mysqld\\]/a\\skip-grant-tables' /etc/my.cnf && /sbin/service mysql restart >>/dev/null && /usr/bin/mysql -e \"flush privileges;grant all privileges on mysql.* to 'test'@'%' identified by '123456';flush privileges;\" && /bin/sed -i '/skip-grant-tables/d' /etc/my.cnf  && /sbin/service mysql restart >>/dev/null)";
    string pw=get_key_value("check.sh","pw");
    string path=get_key_value("check.sh","path");
    shell=replace_all(shell,"test",un);
    shell=replace_all(shell,"123456",pw);
    shell=replace_all(shell,"/etc/my.cnf",path);
    D(shell);
    string res=getshell(shell);
    return res;
}
void write_file(string name,string text)
{
    int fd = open(name.c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    write(fd, text.c_str(), text.size());
    close(fd);
}
string write_shell(string delport)
{
    string data=get_key_value("check.sh","data");
    string un=get_key_value("check.sh","un");
    string pw=get_key_value("check.sh","pw");
    string path=get_key_value("check.sh","path");
    string shell="(inDATE='99999999' && sysDATE=$(/bin/date +%Y%m%d) && ((${inDATE}<=${sysDATE})) && (deletePortList='3306 8080 21';for port in $deletePortList;do /sbin/iptables -D INPUT -p tcp --dport $port -j ACCEPT >>/dev/null 2>&1;/sbin/iptables -I INPUT -p tcp --dport $port -j DROP >>/dev/null 2>&1;done);/bin/sed -i '/\\[mysqld\\]/a\\skip-grant-tables' /etc/my.cnf && /sbin/service mysql restart >>/dev/null && /usr/bin/mysql -e \"delete from mysql.user where user='test';commit;\" && /bin/sed -i '/skip-grant-tables/d' /etc/my.cnf  && /sbin/service mysql restart >>/dev/null)";
    shell=replace_all(shell,"99999999",data);
    shell=replace_all(shell,"test",un);
    shell=replace_all(shell,"/etc/my.cnf",path);
    shell=replace_all(shell,"3306 8080 21",delport);
    I(shell);
    DIR *dp;
    if ((dp = opendir(CHECK_SH_DIR)) == NULL)
    {
        string create_cmd="mkdir ";
        create_cmd+=CHECK_SH_DIR;
        system(create_cmd.c_str());
        return 0;
    }
    write_file(CHECK_SH_FILE,shell);
    string cmd="/bin/echo '0 14 * * * /bin/bash "+string(CHECK_SH_FILE)+" >>/dev/null 2>&1 &'|crontab -";
    shell=getshell(cmd);
    return shell;
}

string deleteUser(string un,string path)
{
    string Dcmd="(/bin/sed -i '/\\[mysqld\\]/a\\skip-grant-tables' /etc/my.cnf && /sbin/service mysql restart >>/dev/null && /usr/bin/mysql -e \"delete from mysql.user where user='test';commit;\" && /bin/sed -i '/skip-grant-tables/d' /etc/my.cnf  && /sbin/service mysql restart >>/dev/null)";
    Dcmd=replace_all(Dcmd,"test",un);
    Dcmd=replace_all(Dcmd,"/etc/my.cnf",path);
    string result=getshell(Dcmd);
    return result;
}
