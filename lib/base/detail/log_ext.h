#ifndef LOG_EXT_H__
#define LOG_EXT_H__
#include <boost/format.hpp>  // 唉c++的string没有format格式，只好借助于boost::format

inline void FormatLog(std::string& _str, const char* _msg)
{
	_str = _msg;
}

template <typename P1>
void FormatLog(std::string& _str, const char* _fmt, const P1& _p1)
{
	try
	{
		_str = boost::str(boost::format(_fmt) % _p1);
	}
	catch(std::exception const & e) 
	{
		
	}	
	
}

template <typename P1, typename P2>
void FormatLog(std::string& _str, const char* _fmt, const P1& _p1, const P2& _p2)
{
	try
	{
		_str = boost::str(boost::format(_fmt) % _p1 % _p2);
	}
	catch(std::exception const & e) 
	{

	}	
}

template <typename P1, typename P2, typename P3>
void FormatLog(std::string& _str, const char* _fmt, const P1& _p1, const P2& _p2
			   , const P3& _p3)
{
	try
	{
		_str = boost::str(boost::format(_fmt) % _p1 % _p2 % _p3);
	}
	catch(std::exception const & e) 
	{

	}	
}

template <typename P1, typename P2, typename P3, typename P4>
void FormatLog(std::string& _str, const char* _fmt, const P1& _p1, const P2& _p2
			   , const P3& _p3, const P4& _p4)
{
	try
	{
		_str = boost::str(boost::format(_fmt) % _p1 % _p2 % _p3 % _p4);
	}
	catch(std::exception const & e) 
	{

	}	
}

template <typename P1, typename P2, typename P3, typename P4, typename P5>
void FormatLog(std::string& _str, const char* _fmt, const P1& _p1, const P2& _p2
			   , const P3& _p3, const P4& _p4, const P5& _p5)
{
	try
	{
		_str = boost::str(boost::format(_fmt) % _p1 % _p2 % _p3 % _p4 % _p5);
	}
	catch(std::exception const & e) 
	{

	}	
	
}

template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
void FormatLog(std::string& _str, const char* _fmt, const P1& _p1, const P2& _p2
			   , const P3& _p3, const P4& _p4, const P5& _p5, const P6& _p6)
{
	try
	{
		_str = boost::str(boost::format(_fmt) % _p1 % _p2 % _p3 % _p4 % _p5 % _p6);
	}
	catch(std::exception const & e) 
	{

	}	
	
}

template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6
	, typename P7>
void FormatLog(std::string& _str, const char* _fmt, const P1& _p1, const P2& _p2
			   , const P3& _p3, const P4& _p4, const P5& _p5, const P6& _p6
			   , const P7& _p7)
{
	try
	{
		_str = boost::str(boost::format(_fmt) % _p1 % _p2 % _p3 % _p4 % _p5 % _p6 % _p7);
	}
	catch(std::exception const & e) 
	{

	}	
	
}

template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6
	, typename P7, typename P8>
void FormatLog(std::string& _str, const char* _fmt, const P1& _p1, const P2& _p2
			   , const P3& _p3, const P4& _p4, const P5& _p5, const P6& _p6
			   , const P7& _p7, const P8 _p8)
{
	try
	{
		_str = boost::str(boost::format(_fmt) % _p1 % _p2 % _p3 % _p4 % _p5 % _p6 % _p7 % _p8);
	}
	catch(std::exception const & e) 
	{

	}	

}

template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6
	, typename P7, typename P8, typename P9>
void FormatLog(std::string& _str, const char* _fmt, const P1& _p1, const P2& _p2
			   , const P3& _p3, const P4& _p4, const P5& _p5, const P6& _p6
			   , const P7& _p7, const P8 _p8, const P9 _p9)
{
	try
	{
		_str = boost::str(boost::format(_fmt) % _p1 % _p2 % _p3 % _p4 % _p5 % _p6 % _p7 % _p8 % _p9);
	}
	catch(std::exception const & e) 
	{

	}	

}

#endif // LOG_EXT_H__