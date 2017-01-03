#include "curl/curl.h"
#include <map>
#include <string>
#include "jsoncpp/include/json/value.h"
#include "response_blob.h"

class ResponseBlob;

class HttpRequest
{
public:
	bool Create();
	bool GetResponse(const std::string& uri, const std::string& content, Json::Value& json_value);
private:
	static size_t HeaderFunction(char *data, size_t size, size_t nmemb, void *userdata);
	static size_t WriteFunction(char *data, size_t size, size_t nmemb, void *userdata);

public:
	// 私有构造函数，只有friend class 才可以构造
	HttpRequest();

private: 
	///The first basic rule is that you mustneversimultaneously share a 
	///libcurl handle (be it easy or multi or whatever) betweenmultiple 
	///threads. Only use one handle in one thread at any time. You can 
	///passthe handles around among threads, but you must never use a 
	///single handle frommore than one thread at any given time.
	///curl 非线程安全，不要再多线程中使用同一个curl handle
	CURL *curl_;
	ResponseBlob blob_;
	size_t blobSize_;
};
