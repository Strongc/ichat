// demo_http_client.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "url_client.h"

int _tmain(int argc, _TCHAR* argv[])
{
	curl_global_init(CURL_GLOBAL_ALL);
	url_client uc;
	//struct curl_slist* cc = uc.make_chunk_();
	std::string res;
	int a = uc.get_("192.168.100.107:9699/api/region", res);

	int b = uc.delete_("192.168.100.107:9699/api/region", res);
	getchar();
	curl_global_cleanup();
	return 0;
}

