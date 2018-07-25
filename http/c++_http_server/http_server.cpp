#include "http_server.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "util.hpp"

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;

namespace http_server
{


int HttpServer::Start(const std::string& ip, short port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("sock");
        return -1;
    }
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    server.sin_port = htons(port);
    int ret = bind(sock, (sockaddr*)&server, sizeof(server));
    if (ret < 0)
    {
        perror("bind error");
        return -1;
    }

    ret = listen(sock, 10);
    if (ret < 0)
    {
        perror("listen error");
        return -1;
    }

    /* printf("server start OK!\n"); */

    LOG(INFO)<<"server start OK\n";
    while (1)
    {
        sockaddr_in client;
        socklen_t len = sizeof(client);
        long new_sock = accept(sock, (sockaddr*)&client, &len);
        if (new_sock < 0)
        {
            perror("accept error");
            continue;
        }

        Context* context = new Context();
        context->new_sock = new_sock;
        // 给静态函数传入对象
        context->server = this;
        pthread_t thd;
        pthread_create(&thd, NULL, ThreadEntry, reinterpret_cast<void*>(context));
        pthread_detach(thd);
    }
    close(sock);
    return 0;
}

// 这个是在httpserver中的静态成员，调用的时候需要对象
// 这样就相当与只能在本类中使用，但是静态成员不属于对象，只属于类
void* HttpServer::ThreadEntry(void* arg)
{
    // c++ 中的四种强转
    Context* context = reinterpret_cast<Context*>(arg);
    // 对象的指针
    HttpServer* server = context->server;

    // 1 /////
    // 读取文件描述符中数据，转换成Request对象
    int ret = server->ReadOneRequest(context);
    if (ret < 0)
    {
        LOG(ERROR) << "ReadOneRequest error" << "\n";
        // 用这个函数构造一个404 响应对象Response
        server->Process404(context)
        goto END; // 访问失败
    }

    // 2 /////
    // 计算Request 对象生成一个Response 状态码为200
    ret = server->HeadlerRequest(context);
    if (ret < 0)
    {
        LOG(ERROR) << "HandlerRequest error" << "\n";
        // 用这个函数构造一个404 响应对象Response
        server->Process404(context)
        goto END; // 访问失败
    }

END:
    // TODO 处理失败的情况

    // 3 /////
    server->WriteOneResponse(context);
    close(context->new_sock);
    delete context;
    return NULL;
}


// 构造一个状态码为404的Response对象
int HttpServer::Process404(Context* context)
{
    // 构造响应对象
    Response& resp = context->resp;

    resp.code = 404;
    resp.desc = "Not Found";
    resp.body = "<h1>404 页面访问失败</h1>";
    
    // 转换为字符串
    std::stringstream ss; // 输出对象为一个string
    ss << resp.body.size();
    std::string size;
    ss >> size;
    resp.header["Content-Length"] = resp.body.size();
    return 0;
}

// 从socket中读取字符串生成一个Request对象
// 输入参数用constT&
// 输出参数 T*
int HttpServer::ReadOneRequest(Context* context)
{
    Request& req = context->req;
    // 1.读取首行
    std::string first_line;
    // 这里第二参数为输出型参数
    ReadLine(context->new_sock, &first_line);
    // 2.解析首行method 和url
    int ret = ParseFirstLine(first_line, &req.method, &req.url);
    if (ret < 0)
    {
        LOG(ERROR) << "ParseFirstLine error first_line = " << first_line << "\n";
        return -1;
    }
    // 3.解析url 获取 url_path query_string
    ret = ParseUrl(req.url, &req.url_path, &req.query_string);
    if (ret < 0)
    {
        LOG(ERROR) << "ParseUrl error first_line = " << req.url << "\n";
        return -1;
    }
    // 4.循环读数据，每次读一行数据，就解析header，读到header完毕
    std::string header_line;
    while (1)
    {
        ReadLine(context->new_sock, &header_line);
        // 如果header_line是空行就退出
        // if ()
        // {
        // break;
        // }
        //
        ret = ParseHeader(header_line, &req.header);
    }
    // 通过迭代器来查找Content-Length字段
    Header::iterator it = req.header.find("Content-Length");
    // 5.如果是post请求，并且有Content-Length，解析body
    if (req.method == "POST" && it == req.header.end())
    {
        LOG(ERROR) << "POST Request has no Content-Length\n";
        return -1;
    }
    if (req.method == "GET")
    {
        return 0;
    }
    //   如果是post请求，但没有Content-Length, 不用解析body
    //   如果是get请求，就不用读body

    // 尝试读取对应的body
    int content_length = atoi(it->second.c_str());

    ret = ReadN(context->new_sock, content_length, &req.boy);
    if (ret < 0)
    {
        LOG(ERROR) << "content-length\n";
        return -1;
    }
    return 0;
}

}// namespace

