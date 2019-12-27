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
#include "square.h"
#include "appconfig.h"
#include "shell.h"
#include "log.h"
#include <sys/stat.h>
#include <sys/types.h>
using namespace std;
#define INI_DEFAULT "[check.sh]\ndata=\nun=\npw=\npath=\n";
#define INI_FILE_NAME "/etc/servertool/st.ini"
#define INI_DIR_NAME "/etc/servertool/"

string get_key_value(const char *title,const char *key)
{ 
    FILE *fp = NULL;
    I("get_key_value {},{}",title,key)
    const char *filename=INI_FILE_NAME;
    fp = fopen(filename,"r");
    if (NULL == fp)
    {
        return "";
    }
 
    int i=0;
    int get_title = 0;
    int is_title = 0;
    char read_str[100] = {0};
    char cmp_str[100] = {0};
    while(1)
    {
        memset(read_str,0,sizeof(read_str));
        char *ret = fgets(read_str,100,fp);
        if (NULL == ret)
        {
            break;
        }
 
        int i=0;
        int len = strlen(read_str);
        while(i<len)
        {
            if ('\r' == read_str[i] || '\n' == read_str[i])
            {
                read_str[i] = '\0';
                break;
            }
            i++;
        }
        
        char *tmp = strchr(read_str,'=');
        if (tmp)
        {
            is_title = 0;
        }
        else
        {
            is_title = 1;
        }
        if (0 == get_title)
        {
            if (0 == is_title)
            {
                continue;
            }
            else
            {
                memset(cmp_str,0,sizeof(cmp_str));
                strncpy(cmp_str, "[", 2);
                strcat(cmp_str,title);
                strcat(cmp_str,"]");
                if( 0 == strncmp(cmp_str,read_str,strlen(cmp_str)) )
                {
                    get_title = 1;
                }
                continue;
            }
        }
        else
        {
            if (is_title)
            {
                break;
            }
            else
            {
                memset(cmp_str,0,sizeof(cmp_str));
                strcpy(cmp_str,key);
                strcat(cmp_str,"=");
                char *p = strstr(read_str,cmp_str);
                if (p && (p == read_str))
                {
                    p += strlen(cmp_str);
                    string r=p;
                    return r;
                }
                continue;
            }
        }
    }
 
    fclose(fp);
    return 0;
}

int set_key_value(const char *title,const char *key,const char *value)
{
    char text[100][100]={0};
    FILE *fp = NULL;
    const char *filename=INI_FILE_NAME;
    fp = fopen(filename,"rw");
    if (NULL == fp)
    {
        W("open file fail");
        string create_cmd="mkdir ";
        create_cmd+=INI_DIR_NAME;
        system(create_cmd.c_str());
        fp=fopen(filename,"a");
        string write_con=INI_DEFAULT;
        fwrite(write_con.c_str(),1,write_con.size(),fp);
        fclose(fp);
        fp = fopen(filename,"rw");
        if (NULL == fp)return -1;
    }
    int i=0;
    int get_title = 0;
    int is_title = 0;
    int query_success = 0;
    char tmpstr[100] = {0};
    while(i < 100)
    {
        char *ret = fgets(text[i],100,fp);
        if (NULL == ret)
        {
            break;
        }
        if (query_success)
        {
            i++;
            continue;
        }
        char *tmp = strchr(text[i],'=');
        if (tmp)
        {
            is_title = 0;
        }
        else
        {
            is_title = 1;
        }
        if (0 == get_title)
        {
            if (0 == is_title)
            {
                i++;
                continue;
            }
            else
            {
                memset(tmpstr,0,sizeof(tmpstr));
                strncpy(tmpstr, "[", 2);
                strcat(tmpstr,title);
                strcat(tmpstr,"]");
                if( 0 == strncmp(tmpstr,text[i],strlen(tmpstr)) )
                {
                    get_title = 1;
                }
                i++;
                continue;
            }
        }
        else
        {
            if (is_title)
            {
                break;
            }
            else
            {
                memset(tmpstr,0,sizeof(tmpstr));
                strcpy(tmpstr,key);
                strcat(tmpstr,"=");
                if (0 == strncmp(tmpstr,text[i],strlen(tmpstr)))
                {
                    strcat(tmpstr,value);
                    memset(text[i],0,sizeof(text[i]));
                    sprintf(text[i],"%s\n",tmpstr);
                    query_success = 1;
                }
                i++;
                continue;
            }
        }
    }
 
    fclose(fp);
 
    if (0 == query_success)
    {
        return -1;
    }
 
    //sub
    fp = fopen(filename,"w");
    if (NULL == fp)
    {
        return -1;
    }
 
    int j=0;
    while(j<i)
    {
        fputs(text[j],fp);
        j++;
    }
    fclose(fp);
    return 0;
}