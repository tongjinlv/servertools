#include <stdlib.h>
#include <iostream>
using namespace std;

int main(){
	//开端口
	system("allowPortList=\"3306 8080 3389\";for port in $allowPortList;do iptables -I INPUT -p tcp --dport $port -j ACCEPT >>/dev/null 2>&1;done");
	//删端口
	system("allowPortList=\"121 122 123\";for port in $allowPortList;do iptables -D INPUT -p tcp --dport $port -j ACCEPT >>/dev/null 2>&1;done");
	return 0;
}
