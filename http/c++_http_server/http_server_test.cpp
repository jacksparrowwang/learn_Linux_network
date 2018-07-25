#include "http_server.h"
#include <iostream>
using namespace http_server;

int main()
{
    HttpServer server;
    int ret = server.Start("0", 8888);
    std::cout<<ret<<std::endl;
}
