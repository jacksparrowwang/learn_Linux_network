#pragma once
#include <string>
#include <unordered_map>

namespace http_server // 命名域
{
typedef std::unordered_map<std::string, std::string> Header;

struct Request
{
    std::string method; // 亲求的方法，GET/POST
    std::string url;    // 一个完整的网址
    std::string url_path; // /index.html
    std::string query_string; // kwd="cpp" 的键值对
    // std::string version; // 版本号不考虑
    Header header; // 一组字符串键值对
    std::string boy; // http 的请求body
};

struct Response
{
    //std::string version;  //版本号
    int code;   // 状态码
    std::string desc;   // 状态码
    Header header;   // 响应报文中的header的数据
    std::string body;  // 响应报文中body的数据
};

// 指针或者引用可以用前置声明
class HttpServer;

// 这个是从请求来，到响应是所有的数据的存放
struct Context
{
    Request req;
    Response resp;
    int new_sock;
    HttpServer* server;
};

// 流程
class HttpServer
{
    // 函数返回0 表示成功，返回-1为失败
public:
    // 字符串的反序列化
    int Start(const std::string& ip, short port);

    // 从socket中读取一字符串，输入到Request对象中
    int ReadOneRequest(Context* context);

    // 根据Response对象，拼接成一个字符串，写回到客户端
    int WriteOneResponse(Context* context);

    // 根据Request对象，构造Respose对象
    int HeadlerRequest(Context* context); // 这是计算的步骤,也是核心的计算生成Response

    // 构造出一个404响应
    int Process404(Context* context);

private:
    // 在c++中static成员函数，只能在声明中添加static，不能在定义用
    static void* ThreadEntry(void* arg);
    int ParseFirstLine(const std::string& first_line, std::string* method, std::string* url);
    int ParseUrl(const std::string& url, std::string* url_path, std::string* query_string);
};

}// end http_server
