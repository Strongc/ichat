#pragma once

#include <string>
#include <vector>
#include "curl\include\curl.h"
#include "curl\include\easy.h"

class url_client
{
public:
	url_client();
	~url_client();

private:
	CURL* m_curl;

public:
	int get_(const std::string& url, std::string& response, const struct curl_slist* chunk = NULL);
	int delete_(const std::string& url, std::string& response, const struct curl_slist* chunk = NULL);
	int post_(const std::string& url, const std::string& ext, std::string& response, const struct curl_slist* chunk = NULL);
	int put_(const std::string& url, const std::string& ext, std::string& response, const struct curl_slist* chunk = NULL);

	struct _chunk_string
	{
		std::string name;
		std::string value;
	};
	struct curl_slist* make_chunk_(const std::vector<_chunk_string>& chunk_string);
	void destory_chunk(struct curl_slist* chunk);
};

