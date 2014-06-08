// librest.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "librest.h"
#include <regex>
#include <map>

std::map<std::string, REST_FUN> map_rest_fun;

// 这是导出函数的一个示例。
LIBREST_API int rest_fun_regist(const std::string& api, REST_FUN rest_fun)
{
	std::map<std::string, REST_FUN>::iterator itr = map_rest_fun.find(api);
	if (itr != map_rest_fun.end())
	{
		itr->second = rest_fun;
		return 1;
	}

	map_rest_fun.insert(std::make_pair(api, rest_fun));

    printf("add rest fun:%s\n", api.c_str());

	return 0;
}

LIBREST_API int rest_fun_set(const _request& req, _reply& rep)
{
	// 正则表达式匹配
	std::map<std::string, REST_FUN>::const_iterator itr = map_rest_fun.begin();
	for (; itr != map_rest_fun.end(); ++itr)
	{
		std::tr1::regex pattern(itr->first);
		if (std::tr1::regex_match(req.uri, pattern))
		{
			itr->second(req, rep);
			return 0;
		}
	}
    printf("not find rest fun for %s\n", req.uri.c_str());
	return -1;
}
