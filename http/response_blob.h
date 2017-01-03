#pragma once
#include <stddef.h>
#include <string>

class ResponseBlob
{
	static const int DefaultBuffSize = 256;
public:
	ResponseBlob();
	~ResponseBlob();

public:
	void Reset();
	const void* GetData() const { return buff_; }
	void* GetData() { return buff_; }
	size_t GetLength() const { return use_size_; }
	const std::string& GetContentType() const { return content_type_; }
	void SetContentType(const std::string& content_type) { content_type_ = content_type; }
	bool Realloc(size_t bytes);
	bool Push(void* data, size_t len);
private:
	size_t FreeSize() const; 
private:
	void* buff_;
	size_t buff_size_; // 已分配长度
	size_t use_size_;  // 内容真实长度
	std::string content_type_;
	
};
