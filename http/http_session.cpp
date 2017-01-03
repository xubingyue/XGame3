#include "http_session.h"
#include "base/log_wrap.h"

int HttpSession::http_counter = 0;
HttpSession::HttpSessionPtr HttpSession::Create(boost::asio::io_service& io_service, 
                                 const std::string& server, const std::string& path,
								 int srcID, int dstID, HttpMsgMap* call_back)
{
	HttpSessionPtr new_session(new HttpSession(io_service, server, path, srcID, dstID, call_back));
	return new_session;
}

HttpSession::HttpSession(boost::asio::io_service& io_service, const std::string& server, 
                         const std::string& path, int srcid, int dstid, HttpMsgMap* call_back)
      : io_service_(io_service), resolver_(io_service), socket_(io_service), 
	   call_back_(call_back), buff_len_(0), server_(server), path_(path), 
		srcSID(srcid), dstSID(dstid)
{
    memset(buff_, 0, sizeof(buff_)); 
    http_counter ++; 
    LOG_DEBUG("HttpSession, http_counter:%d", http_counter);
}

void HttpSession::start()
{
    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    std::ostream request_stream(&request_);
    request_stream << "GET " << path_ << " HTTP/1.0\r\n";
    request_stream << "Host: " << server_ << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    // Start an asynchronous resolve to translate the server and service names
    // into a list of endpoints.
    
	//tcp::resolver::query query(server_, "http");
	tcp::resolver::query query(server_, "18082");

    resolver_.async_resolve(query,
        boost::bind(&HttpSession::handle_resolve, shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::iterator)); 
}

HttpSession::~HttpSession()
{
   http_counter--;
   LOG_DEBUG("~HttpSession, http_counter:%d", http_counter);
}

void HttpSession::handle_resolve(const boost::system::error_code& err, 
                    tcp::resolver::iterator endpoint_iterator)
{
	if (!err)
    {
      // Attempt a connection to each endpoint in the list until we
      // successfully establish a connection.
		boost::asio::async_connect(socket_, endpoint_iterator,
          boost::bind(&HttpSession::handle_connect, shared_from_this(),
            boost::asio::placeholders::error));
    }
    else
    {
		std::cout << "Error: " << err.message() << "\n";
    }
}

void HttpSession::handle_connect(const boost::system::error_code& err)
{
    if (!err)
    {
      // TODO:发现一个问题，如果先打印request内容，会导致发送失败？
      // 可能的原因是streamBuf在std::cout时被修改了
      // 
      // The connection was successful. Send the request.
      boost::asio::async_write(socket_, request_,
          boost::bind(&HttpSession::handle_write_request, shared_from_this(),
            boost::asio::placeholders::error));
    }
    else
    {
      std::cout << "Error: " << err.message() << "\n";
    }
}

void HttpSession::handle_write_request(const boost::system::error_code& err)
{
    if (!err)
    {
      // Read the response status line. The response_ streambuf will
      // automatically grow to accommodate the entire line. The growth may be
      // limited by passing a maximum size to the streambuf constructor.
      boost::asio::async_read_until(socket_, response_, "\r\n",
          boost::bind(&HttpSession::handle_read_status_line, shared_from_this(),
            boost::asio::placeholders::error));
    }
    else
    {
      std::cout << "Error: " << err.message() << "\n";
    }
}

void HttpSession::handle_read_status_line(const boost::system::error_code& err)
{
    if (!err)
    {
      // Check that response is OK.
      std::istream response_stream(&response_);
      std::string http_version;
      response_stream >> http_version;
      unsigned int status_code;
      response_stream >> status_code;
      std::string status_message;
      std::getline(response_stream, status_message);
      if (!response_stream || http_version.substr(0, 5) != "HTTP/")
      {
        std::cout << "Invalid response\n";
        return;
      }
      if (status_code != 200)
      {
        std::cout << "Response returned with status code ";
        std::cout << status_code << "\n";
        return;
      }

      // Read the response headers, which are terminated by a blank line.
      boost::asio::async_read_until(socket_, response_, "\r\n\r\n",
          boost::bind(&HttpSession::handle_read_headers, shared_from_this(),
            boost::asio::placeholders::error));
    }
    else
    {
      std::cout << "Error: " << err << "\n";
    }
}

void HttpSession::handle_read_headers(const boost::system::error_code& err)
{
    if (!err)
    {
      // Process the response headers.
      std::istream response_stream(&response_);
      std::string header;
      while (std::getline(response_stream, header) && header != "\r")
        std::cout << header << "\n";
      std::cout << "\n";

      // Start reading remaining data until EOF. 
      boost::asio::async_read(socket_, response_,
          boost::asio::transfer_at_least(1),
          boost::bind(&HttpSession::handle_read_content, shared_from_this(),
            boost::asio::placeholders::error));

    }
    else
    {
      std::cout << "Error: " << err << "\n";
    }
}

void HttpSession::handle_read_content(const boost::system::error_code& err)
{
    if (!err)
    {
      // Write all of the data that has been read so far.
      //std::cout << &response_;

      // Continue reading remaining data until EOF.
      boost::asio::async_read(socket_, response_,
          boost::asio::transfer_at_least(1),
          boost::bind(&HttpSession::handle_read_content, shared_from_this(),
            boost::asio::placeholders::error));
    }
    else if (err != boost::asio::error::eof)
    {
		std::cout << "Error: " << err << "\n";
    }
    else
    {
      // 读到文件尾，此时获得整个消息内容
      //std::cout << "all content:" << &response_ << std::endl;
		if (response_.size() >= sizeof(buff_))
		{
			LOG_WARN("response content length error, len:%d", response_.size());
			return;
		}

		buff_len_ = response_.size();
		response_.sgetn(buff_, sizeof(buff_));
		
		if (call_back_ != NULL)
		{
			call_back_->fn(this, buff_, buff_len_, "");
		}
		/*
		try
		{
			if (call_back_ != NULL)
			{
				call_back_->OnResponse(this, buff_, buff_len_, "");
			}
		} 
		catch (...)
		{
			LOG_WARN("catch error");
		}
		*/
    }
}

