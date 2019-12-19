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
using namespace std;
#include "square.h"

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
 
std::vector<unsigned char> hexToBin(std::string hex)
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
string encryption(string text)
{
    string temp;
     char *strTest = "ceD2CAC7B8F6B4F3D0DCC3A8";
	std::vector<unsigned char> charVec=hexToBin(strTest);
    charVec[0]=12;
    for(int i=0;i<charVec.size();i++)printf("%d ",charVec[i]);
	std::cout << std::endl;
	string strHex = binToHex(charVec.data(), charVec.size());
	std::cout << strHex << std::endl;
    return temp;
} 
string decryption(string text)
{
    string temp;
    return temp;
}