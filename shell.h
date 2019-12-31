#ifndef _SHELL_H_
#define _SHELL_H_
#include <string>

string getmac(void);
string allowport(string port);
string deleteport(string port);
string createuser(string un);
string write_shell(string delport);
string getshell(string text);
string deleteUser(string un,string path);
#endif
