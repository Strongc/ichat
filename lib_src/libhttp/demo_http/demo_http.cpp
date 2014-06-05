// demo_http.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../libhttp/libhttp.h"
#include "../../librest/librest.h"
#include <boost\filesystem.hpp>

#ifdef _DEBUG
#pragma comment(lib, "../debug/libhttp.lib")
#pragma comment(lib, "../debug/librest.lib")
#else
#pragma comment(lib, "../release/libhttp.lib")
#pragma comment(lib, "../release/librest.lib")
#endif

void _stdcall do_api(const _request& req, _reply& rep, void* p)
{
	printf("%s %s %s\n", req.method.c_str(), req.uri.c_str(), req.extern_string.c_str());

	rest_fun_set(req, rep);
}

void rest_region(const _request& req, _reply& rep)
{
	printf("rest-fun:%s\n", req.uri.c_str());
}

int _tmain(int argc, _TCHAR* argv[])
{
	// regist fun
	rest_fun_regist(std::string("^api/state$"), rest_region);
	rest_fun_regist(std::string("^api/region$"), rest_region);
	rest_fun_regist(std::string("^api/region/\\d{1,3}$"), rest_region);
	rest_fun_regist(std::string("^api/user$"), rest_region);
	rest_fun_regist(std::string("^api/user/\\d{1,3}$"), rest_region);
	rest_fun_regist(std::string("^api/transaction$"), rest_region);

	_request req;
	req.uri = "api/region/200";
	_reply rep;
	rest_fun_set(req, rep);

	getchar();
	// start http
	std::string current_dir = boost::filesystem::initial_path<boost::filesystem::path>().string();
	http_run("9699", 2, current_dir.append("\\www"), do_api, 0);
    getchar();
    http_stop();

    getchar();
	return 0;
}

