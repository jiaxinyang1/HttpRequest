#include "HttpService.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/locale.hpp>
#include <cstdlib>
int main(int argc, char* argv[])
{
	HttpService test;
	test.setCookie("sid=m20k6lhz; buvid3=F6E33CB6-53D6-413E-BDEB-C4AEF583F2B41984infoc; fts=1511685463; pgv_pvi=6480661504; rpdid=olwpxoqkowdosooxqspiw; LIVE_BUVID=7be1311519ef2451eb68980c4ce02a1c; LIVE_BUVID__ckMd5=3e2afd777184de5a; CURRENT_QUALITY=112; finger=81df3ec0");
	cout<<test.HttpRequestFor("s.search.bilibili.com/main/suggest?jsoncallback=jqueryCallback_bili_3&func=suggest&suggest_type=accurate&sub_type=tag&main_ver=v1&highlight=&userid=0&bangumi_acc_num=1&special_acc_num=1&topic_acc_num=1&upuser_acc_num=3&tag_num=10&special_num=10&bangumi_num=10&upuser_num=3&term=bilibili&rnd=0.3018351813416065&_=1535015627124");
	system("pause");
	return 0;

}
void HttpService::JSON(string s) const
{
	auto first = s.find('{');
	auto last = s.rfind('}');
	string buf = s.substr(first, last - first + 1);
	std::stringstream str_stream(buf);
	boost::property_tree::ptree root;
	try
	{
		boost::property_tree::read_json(str_stream, root);
	}
	catch (exception e)
	{
		cout << e.what() << endl;
	}
	auto item = root.get_child("result");
	auto item2 = item.get_child("tag");
	for (auto it = item2.begin(); it != item2.end(); ++it)
	{

		cout << UTF8ToGBK((unsigned char *)it->second.get< string >("value").c_str()) << endl;
		cout << UTF8ToGBK((unsigned char *)it->second.get< string >("name").c_str()) << endl;
	}

}