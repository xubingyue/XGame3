#ifndef HTTP_SESSION_H_
#define HTTP_SESSION_H_

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include "base/msg.h"

using boost::asio::ip::tcp;

struct IResponseCallBack;
struct HttpMsgMap;

class HttpSession : public boost::enable_shared_from_this<HttpSession>
{
public:
    enum
    {
        kRecvBufMaxSize = 8192,
    };

    typedef boost::shared_ptr<HttpSession> HttpSessionPtr;
    
    static HttpSessionPtr Create(boost::asio::io_service& io_service, 
                                 const std::string& server, const std::string& path,
								 int srcID, int dstID,
                                 HttpMsgMap* call_back);
    ~HttpSession();

    void start();
private:
    HttpSession(boost::asio::io_service& io_service, const std::string& server, 
                const std::string& path, int srcid, int dstid, HttpMsgMap* call_back);

    void handle_resolve(const boost::system::error_code& err, 
                        tcp::resolver::iterator endpoint_iterator);

    void handle_connect(const boost::system::error_code& err);

    void handle_write_request(const boost::system::error_code& err);

    void handle_read_status_line(const boost::system::error_code& err);

    void handle_read_headers(const boost::system::error_code& err);

    void handle_read_content(const boost::system::error_code& err);

private:
	static int http_counter;
	boost::asio::io_service& io_service_;
	tcp::resolver resolver_;
	tcp::socket socket_;
	boost::asio::streambuf request_;
	boost::asio::streambuf response_;
	HttpMsgMap* call_back_;
	char buff_[kRecvBufMaxSize];
	unsigned int buff_len_;
	std::string server_;
	std::string path_;

public:
	int	srcSID;
	int dstSID;

};


struct HttpMsgMap
{
	typedef void (*CltMsgCBFn)(HttpSession* http_client, const char * data, unsigned int len, const char * url);
	CltMsgCBFn fn;		 // 具体的处理回调函数
};


struct IResponseCallBack
{
    virtual void OnResponse(HttpSession* http_client, const char * data, unsigned int len, const char * url) = 0;

	//virtual void OnError(IRequestProxy* requestProxy,DWORD dwError,const char * url) = 0;

    //virtual void OnTimeOut(IRequestProxy* requestProxy,float seconds) = 0;
};

#endif // HTTP_SESSION_H_
