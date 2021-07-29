
#ifndef _SQUARE_H_
#define _SQUARE_H_
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

double square( double x );

string encryption(string str);
string decryption(string str);
string get_url_value(string url,const string &request);
vector<string> split_c(const string &s, const string &seperator);
vector<string> split_c(const char *ss, const string &seperator);
#endif