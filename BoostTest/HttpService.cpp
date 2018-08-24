#include "HttpService.h"
#include <boost/locale.hpp>
HttpService::HttpService()
{
}

HttpService::~HttpService()
{
			boost::system::error_code ec;
			socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
}

string HttpService::HttpRequestFor(string url, string method)
{
	connect(url,method);
	write(socket, req);

	return readResponse();
}

void HttpService::connect(string url , const string method)
{
	addressResolve(url);
	auto const results = resolver.resolve(host, port);//解析地址
	boost::asio::connect(socket, results.begin(), results.end());//连接
	req.target(target);
	req.version(version);
	method == "post" ? req.method(boost::beast::http::verb::post) : req.method(boost::beast::http::verb::get);
	req.set(boost::beast::http::field::host, host);
	req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);
}

void HttpService::addressResolve(string url)
{
	int i;
	for(i=0;i<url.length();++i)
	{
		if (url[i]=='/')
			break;
	}
	host = url.substr(0, i);
	target = url.substr(i);
}

string HttpService::readResponse()
{
	boost::beast::flat_buffer buffer;
	boost::beast::http::response<boost::beast::http::string_body> res;

	boost::beast::http::read(socket, buffer, res);

	std::string result = UTF8ToGBK((unsigned char*)res.body().c_str());

	return result;
}

string HttpService::UTF8ToGBK(unsigned char * lpUTF8Str)
{
	wchar_t * lpUnicodeStr = nullptr;
	int nRetLen = 0;

	if (!lpUTF8Str)  //如果UTF8字符串为NULL则出错退出
		return nullptr;

	nRetLen = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<char *>(lpUTF8Str), -1, nullptr, 0);  //获取转换到Unicode编码后所需要的字符空间长度
	lpUnicodeStr = new WCHAR[nRetLen + 1];  //为Unicode字符串空间
	nRetLen = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<char *>(lpUTF8Str), -1, lpUnicodeStr, nRetLen);  //转换到Unicode编码
	if (!nRetLen)  //转换失败则出错退出
		return nullptr;
	nRetLen = WideCharToMultiByte(CP_ACP, 0, lpUnicodeStr, -1, nullptr, 0, nullptr, nullptr);  //获取转换到GBK编码后所需要的字符空间长度
	const auto GBKStr = new char[nRetLen + 1];

	nRetLen = WideCharToMultiByte(CP_OEMCP, 0, lpUnicodeStr, -1, GBKStr, nRetLen, nullptr, nullptr);  //转换到GBK编码
	GBKStr[nRetLen] = '\0';
	std::string buffer(GBKStr);
	
	delete[]GBKStr;
	delete[]lpUnicodeStr;
	return  buffer;
}

