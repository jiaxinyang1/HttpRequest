#pragma once
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <string>
#include <iostream>
using namespace std;
class HttpService
{
public:
	HttpService();
	~HttpService();
	string HttpRequestFor(string url,string method="get");
	void setCookie( const string& cookie) { this->req.set(boost::beast::http::field::cookie, cookie); }
private:
	string  host;//主域名
	string  port="80";//端口号 http 默认80
	string target;//资源地址
	int version = 11;//http版本 默认1.1

	boost::asio::io_context ioc;

	boost::asio::ip::tcp::resolver resolver{ ioc };//为域名解析
	boost::asio::ip::tcp::socket socket{ ioc };//套接字

	boost::beast::http::request<boost::beast::http::string_body> req;//请求头
private:
	void connect(string url, string method="get");
	void addressResolve(string url);
	string readResponse();
	static string UTF8ToGBK(unsigned char * lpUTF8Str);
	void JSON(string s) const;
	

};

