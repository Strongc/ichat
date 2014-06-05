#include "stdafx.h"
#include "url_client.h"
#include <sstream>

static size_t OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
	std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);
	if (NULL == str || NULL == buffer)
	{
		return -1;
	}

	char* pData = (char*)buffer;
	str->append(pData, size * nmemb);
	return nmemb;
}

url_client::url_client()
: m_curl(NULL)
{
	m_curl = curl_easy_init();
	if (NULL == m_curl)
	{
		throw std::string("curl_easy_init fail");
	}
}


url_client::~url_client()
{
	if (NULL != m_curl)
	{
		curl_easy_cleanup(m_curl);
		m_curl = NULL;
	}
}

int url_client::get_(const std::string& url, std::string& response, const struct curl_slist* chunk)
{
	if (chunk != NULL)
	{
		curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, chunk);
	}
	curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(m_curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, OnWriteData);
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, (void *)&response);
	/**
	* 当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作。
	* 如果不设置这个选项，libcurl将会发信号打断这个wait从而导致程序退出。
	*/
	curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, 3);
	curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 3);
	CURLcode res = curl_easy_perform(m_curl);
	return res;
}

int url_client::delete_(const std::string& url, std::string& response, const struct curl_slist* chunk)
{
	if (chunk != NULL)
	{
		curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, chunk);
	}
	curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(m_curl, CURLOPT_CUSTOMREQUEST, "DELETE");

	curl_easy_setopt(m_curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, OnWriteData);
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, (void *)&response);
	/**
	* 当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作。
	* 如果不设置这个选项，libcurl将会发信号打断这个wait从而导致程序退出。
	*/
	curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, 3);
	curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 3);
	CURLcode res = curl_easy_perform(m_curl);
	return res;
}

int url_client::post_(const std::string& url, const std::string& ext, std::string& response, const struct curl_slist* chunk)
{
	if (chunk != NULL)
	{
		curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, chunk);
	}
	curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(m_curl, CURLOPT_POST, 1);
	curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, ext.c_str());
	curl_easy_setopt(m_curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, OnWriteData);
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, (void *)&response);
	curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, 3);
	curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 3);
	CURLcode res = curl_easy_perform(m_curl);
	return res;
}

int url_client::put_(const std::string& url, const std::string& ext, std::string& response, const struct curl_slist* chunk)
{
	if (chunk != NULL)
	{
		curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, chunk);
	}
	curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(m_curl, CURLOPT_PUT, 1);
	curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, ext.c_str());
	curl_easy_setopt(m_curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, OnWriteData);
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, (void *)&response);
	curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, 3);
	curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 3);
	CURLcode res = curl_easy_perform(m_curl);
	return res;
}

struct curl_slist* url_client::make_chunk_(const std::vector<_chunk_string>& chunk_string)
{
	struct curl_slist* chunk = new struct curl_slist;
	std::ostringstream s;
	for each (_chunk_string var in chunk_string)
	{
		s << var.name << ": " << var.value;
		chunk = curl_slist_append(chunk, s.str().c_str());
		s.clear();
	}

	return chunk;
}

void url_client::destory_chunk(struct curl_slist* chunk)
{
	curl_slist_free_all(chunk);
	chunk = NULL;
}