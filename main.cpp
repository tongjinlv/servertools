#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include "square.h"
#include "http.h"
int main ( int argc, char **argv )
{
    std::vector<int> v(3, 0);
    v[2] = 2;
    v.push_back(3);
    v.push_back(4);

    for( std::vector<int>::iterator it = v.begin(); it != v.end(); ++it ) {
        std::cout << *it << "^2\t--->\t" << square(*it) << std::endl;
    }
    http();
    // getchar();
    return 0;
}
