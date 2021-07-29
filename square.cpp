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
#include "weburl.h"
#include "log.h"
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
string decryption(string str)
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
string encryption(string str)
{
    string temp;
    return temp;
}
string get_url_value(string url,const string &request)
{
	char buf[100],n=0;
	int i=url.find(request.c_str());
	if(i>=0)
	{
		i+=request.size()+1;
		for(n=0;n<100;n++)
		{
			char t=url[i+n];
			if(t=='&')break;
			if(t==0)break;
			buf[n]=t;
		}
	}
	buf[n]=0;
	T("get_url_value {},{},{}",i,n,string(buf));
	return string(buf);
	/*
	WebUrl web(url);
	return web.Request(request);
	*/
	
}

vector<string> split_c(const string &s, const string &seperator)
{
    vector<string> result;
    typedef string::size_type string_size;
    string_size i = 0;
 
    while(i != s.size())
    {
        //找到字符串中首个不等于分隔符的字母；
        int flag = 0;
        while(i != s.size() && flag == 0){
            flag = 1;
            for(string_size x = 0; x < seperator.size(); ++x)
            {
                if(s[i] == seperator[x])
                {
                    ++i;
                    flag = 0;
                    break;
                }
            }
        }
 
        //找到又一个分隔符，将两个分隔符之间的字符串取出；
        flag = 0;
        string_size j = i;
        while(j != s.size() && flag == 0)
        {
            for(string_size x = 0; x < seperator.size(); ++x)
                if(s[j] == seperator[x])
                {
                    flag = 1;
                    break;
                }
            if(flag == 0)
                ++j;
        }
        if(i != j)
        {
            result.push_back(s.substr(i, j-i));
            i = j;
        }
    }
    return result;
}
 
 
vector<string> split_c(const char *ss, const string &seperator)
{
    string s = ss;
    vector<string> result;
    typedef string::size_type string_size;
    string_size i = 0;
 
    while(i != s.size())
    {
        //找到字符串中首个不等于分隔符的字母；
        int flag = 0;
        while(i != s.size() && flag == 0){
            flag = 1;
            for(string_size x = 0; x < seperator.size(); ++x)
            {
                if(s[i] == seperator[x])
                {
                    ++i;
                    flag = 0;
                    break;
                }
            }
        }
 
        //找到又一个分隔符，将两个分隔符之间的字符串取出；
        flag = 0;
        string_size j = i;
        while(j != s.size() && flag == 0)
        {
            for(string_size x = 0; x < seperator.size(); ++x)
                if(s[j] == seperator[x])
                {
                    flag = 1;
                    break;
                }
            if(flag == 0)
                ++j;
        }
        if(i != j)
        {
            result.push_back(s.substr(i, j-i));
            i = j;
        }
    }
    return result;
}