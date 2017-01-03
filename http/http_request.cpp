#include "http_request.h"
#include "response_blob.h"
#include <string.h>
#include "jsoncpp/include/json/reader.h"
#include "base/log_wrap.h"

size_t HttpRequest::HeaderFunction(char *data, size_t size, size_t nmemb, void *userdata)
{
	HttpRequest *request = static_cast<HttpRequest*>(userdata);

	// This would typically be done by a regex in _any_ language, however... for pure efficiency reasons, do this parsing by hand here
	// Note: In order to make out lives _a little_ easier, we do an initial string copy. This really does help us a lot in
	// preventing us from re-writing half of C's string library

	std::string header(data, size * nmemb);
	std::string::size_type found = header.find(':');

	if(std::string::npos != found)
	{
		std::string headerKey = header.substr(0, found);

		if(strcasecmp(headerKey.c_str(), "Content-Type") == 0)
		{
			std::string::size_type startPos = header.find_first_not_of(" ", found + 1);
			std::string::size_type endPos = header.find_last_not_of(" \r\n");
			std::string headerData = header.substr(startPos, endPos - startPos - 1);

			request->blob_.SetContentType(headerData);
			//LOG_DEBUG("Got content-type of %s", headerData);
		}
		/*
		else if(strcasecmp(headerKey.c_str(), "Content-Length") == 0)
		{
			std::string::size_type startPos = header.find_first_not_of(" ", found + 1);
			std::string::size_type endPos = header.find_last_not_of(" \r\n");
			std::string headerData = header.substr(startPos, endPos - startPos + 1);

			request->blob_->Realloc(Util::StrToInt(headerData.c_str()));
			LOG_DEBUG("Got content-length of %s", headerData);
		}
		*/
	}

	return size * nmemb;
}

size_t HttpRequest::WriteFunction(char *data, size_t size, size_t nmemb, void *userdata)
{
	HttpRequest *request = static_cast<HttpRequest*>(userdata);

	request->blob_.Push(data, size * nmemb);
	
	return size * nmemb;
}

bool HttpRequest::GetResponse(const std::string& uri, const std::string& content, Json::Value& json_value)
{
	blob_.Reset();

	CURLcode result;

	result = curl_easy_setopt(curl_, CURLOPT_URL, uri.c_str());
	if(CURLE_OK != result)
	{
		LOG_FATAL("Failed to set the URL on CURL");
		return false;
	}

	if (content.size() > 0)
	{
		curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, content.c_str());            //post请求
		curl_easy_setopt(curl_, CURLOPT_POSTFIELDSIZE, content.size());
	}

	result = curl_easy_perform(curl_);

	if(CURLE_OK != result)
	{
		LOG_FATAL("Failed to perform the CURL operation");
		return false;
	}

	//std::cout << (char*)(blob_.GetData()) << std::endl;

	Json::Reader reader;
	if (!reader.parse((char*)blob_.GetData(), (char*)blob_.GetData() + blob_.GetLength(), json_value))
	{
		LOG_FATAL("reader.parse error, uri:%s", uri);
		return false;
	}

	if (!json_value.isObject())
	{
		LOG_FATAL("value is not a object");
		return false;
	}

	// TODO:要根据具体的定制化消息解析
	if (json_value.isMember("error"))
	{
		LOG_FATAL("%s", json_value.toStyledString());
		return false;
	}

	return true;
}

HttpRequest::HttpRequest() : curl_(NULL)
{
}

bool HttpRequest::Create()
{
	curl_ = curl_easy_init();

	if(!curl_)
	{
		LOG_FATAL("Unable to create a CURL handle");
		return false;
	}
 
#define HTTPREQUEST_CURL_SET(opt, arg) \
	if(CURLE_OK != curl_easy_setopt(curl_, (opt), (arg))) \
	{ \
		LOG_FATAL("Unable to set CURL argument"); \
		return false; \
	}

#ifdef DEBUG
	HTTPREQUEST_CURL_SET(CURLOPT_VERBOSE, 1);
	HTTPREQUEST_CURL_SET(CURLOPT_DEBUGFUNCTION, &HttpRequest::DebugFunction);
	HTTPREQUEST_CURL_SET(CURLOPT_DEBUGDATA, this);
#endif // DEBUG

	//HTTPREQUEST_CURL_SET(CURLOPT_POST, 1);
	HTTPREQUEST_CURL_SET(CURLOPT_WRITEFUNCTION, &HttpRequest::WriteFunction);
	HTTPREQUEST_CURL_SET(CURLOPT_WRITEDATA, this);
	HTTPREQUEST_CURL_SET(CURLOPT_HEADERFUNCTION, &HttpRequest::HeaderFunction);
	HTTPREQUEST_CURL_SET(CURLOPT_HEADERDATA, this);
	HTTPREQUEST_CURL_SET(CURLOPT_FOLLOWLOCATION, 1);
	HTTPREQUEST_CURL_SET(CURLOPT_NOSIGNAL, 1);

#undef HTTPREQUEST_CURL_SET

	curl_version_info_data *vInfo = curl_version_info(CURLVERSION_NOW);
	if (vInfo->features & CURL_VERSION_SSL)
	{
		LOG_DEBUG("libcurl support SSL!");
	}
	else
	{
		LOG_DEBUG("libcurl does not support SSL!");
	}

	return true;
}

