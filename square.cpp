#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <string>
#include <sstream>
#include <iomanip>
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
#include <regex>
#include "square.h"
#include "appconfig.h"
#include "WebUrl.h"
using namespace std;
using namespace crystal;
double square( double x )
{
    return x*x;
}
std::string binToHex(const unsigned char *data,size_t size)
{
	std::ostringstream strHex;
	strHex << std::hex << std::setfill('0');
	for (size_t i = 0; i < size; ++i) {
		strHex << std::setw(2)<<static_cast<unsigned int>(data[i]);
	}
	return strHex.str();
}
 
std::vector<unsigned char> hexToBin(const std::string hex)
{
	std::vector<unsigned char> dest;
	int len = hex.size();
	dest.reserve(len / 2);
	for (int i = 0; i < len; i += 2)
	{
		unsigned int element;
		std::istringstream strHex(hex.substr(i, 2));
		strHex >> std::hex >> element;
		dest.push_back(static_cast<unsigned char>(element));
	}
	return dest;
}
string encryption(string str)
{
    string temp;
    const char *strTest =str.c_str();
	std::vector<unsigned char> charVec=hexToBin(strTest);
	string mac=ap_mac;
	for(int i=0;i<mac.size();i++)if(mac[i]==':')mac[i]='_';
    for(int i=0;i<charVec.size();i++)
	{

		charVec[i]=charVec[i]^mac[i%17];
	}
	std::string str2( charVec.begin(), charVec.end());
	if(ap_debug)cout << str2<< endl;
	//string strHex = binToHex(charVec.data(), charVec.size());
    return str2;
} 
string decryption(string str)
{
    string temp;
    return temp;
}
string get_url_value(string url,const string &request)
{
	WebUrl web(url);
	return web.Request(request);
}