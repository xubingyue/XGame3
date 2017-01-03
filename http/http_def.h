#ifndef HTTP_DEF_H_
#define HTTP_DEF_H_

#include <boost/function.hpp>
#include <stdint.h>
#include <string>
#include <boost/shared_ptr.hpp>
#include "jsoncpp/include/json/json.h"
union SCallData
{
	int 		i32[4];
	char		c[16];
	SCallData()
	{
	}

	explicit SCallData(int _i0, int _i1 = 0,  int _i2 = 0, int _i3 = 0)
	{
		i32[0] = _i0;
		i32[1] = _i1;
		i32[2] = _i2;
		i32[3] = _i3;
	}

};

typedef boost::function<void(SCallData, const Json::Value& )> Callback;
struct JobNode
{
    JobNode(const std::string& _url,  const std::string& _content, SCallData _scalldata, Callback _call_back)
        : url(_url), content(_content), scalldata(_scalldata), call_back(_call_back)
    {}
	JobNode()
    {
    }
	std::string url;
	std::string content;
	SCallData scalldata;
	Callback call_back;
    Json::Value json_value;
};

typedef boost::shared_ptr<JobNode> JobNodePtr;

#endif // HTTP_DEF_H_
