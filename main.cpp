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
    LOG(6);
    appconfig(argc,argv);
    while(true)
    {
        http_thread();
        sleep(1);
    }
    return 0;
}
