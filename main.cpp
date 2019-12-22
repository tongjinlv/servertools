#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include "square.h"
#include "http.h"
#include "shell.h"
#include "log.h"
#include "appconfig.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
using namespace std; 


int main ( int argc, char **argv )
{
    
    appconfig(argc,argv);
    /*std::vector<int> v(3, 0);
    v[2] = 2;
    v.push_back(3);
    v.push_back(4);

    for( std::vector<int>::iterator it = v.begin(); it != v.end(); ++it ) {
        std::cout << *it << "^2\t--->\t" << square(*it) << std::endl;
    }*/
   // getmac();
    while(true)
    {
        http_thread();
        sleep(1);
    }
    return 0;
}
