#ifndef HTTP_REQUEST_MGR_H_
#define HTTP_REQUEST_MGR_H_
#include "http_session.h"


// TODO:注意io_service的析构顺序
class HttpRequestMgr
{
public:
    HttpRequestMgr(boost::asio::io_service& io_service) : io_service_(io_service)
    {
    }

    bool HttpRequest(const std::string& server, const std::string& path, int srcID, int dstID, HttpMsgMap* call_back)
    {
        if (call_back == NULL)
        {
            return false;
        }

        HttpSession::HttpSessionPtr new_session = HttpSession::Create(io_service_, server, path, srcID, dstID, call_back);
        new_session->start();
        return true;
    }

private:
    boost::asio::io_service& io_service_;
};

#endif // HTTP_REQUEST_MGR_H_
