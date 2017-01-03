#ifndef CIRCLE_BUFFER_H_
#define CIRCLE_BUFFER_H_

#ifndef WIN32
#include <sys/uio.h>
#else
struct iovec
{
	void*	iov_base;
	size_t	iov_len;
};
#endif // WIN32

struct tagCircleBuffer
{
    int maxLength_; // 最大长度
    int readPos_;   // 读位置
    int writePos_;  // 写位置
	int msgWrite_;	// 写入的消息数量
	int msgRead_;	// 读取的消息数量
    char data_[1];  // 实际存放的数据
};
struct iovec;
namespace CircleBuffer
{
// 重置循环队列，需要加锁
int Clear(tagCircleBuffer* _cb);

// 写入消息，需要对写位置做加锁
int Write(tagCircleBuffer* _cb, const void* _data, size_t _length);
int WriteV(tagCircleBuffer* _cb, const iovec* _iov, int _iovcnt);

// 读取消息，需要对读取位置加锁，如果返回-2，需要重置
int Read(tagCircleBuffer* _cb, void* _data, size_t& _maxLength);

inline bool Empty(tagCircleBuffer* _cb)
{
	return _cb->readPos_ == _cb->writePos_;
}

// 内部的实现
void RealWrite(tagCircleBuffer* _cb, int& _writePos, const void* _data, int _length);
void RealRead(tagCircleBuffer* _cb, int&_readPos, void* _data, int _length);
bool CheckSum(unsigned int _check);
}

#endif /* CIRCLE_BUFFER_H_ */
