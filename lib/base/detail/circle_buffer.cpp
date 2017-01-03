#include <string.h>
#include "base/utils.h"
#include "base/log_wrap.h"
#include "circle_buffer.h"
#undef min
namespace CircleBuffer
{
const unsigned int checkData = 0xCDEFFEDC;
void RealWrite(tagCircleBuffer* _cb, int& _writePos, const void* _data, int _length)
{
    int first = std::min(_cb->maxLength_ - _writePos, _length);
    if (first > 0)
    {
        ::memcpy(_cb->data_ + _writePos, _data, first);
    }

    if (_length - first > 0)
    {
        _writePos = _length - first;
        ::memcpy(_cb->data_, ((char*)_data) + first, _writePos);
    }
    else
    {
         _writePos += _length;
    }
}

void RealRead(tagCircleBuffer* _cb, int&_readPos, void* _data, int _length)
{
    int first = std::min(_cb->maxLength_ - _readPos, _length);
    if (first > 0)
    {
        ::memcpy(_data, _cb->data_ + _readPos, first);
    }

    if (_length - first > 0)
    {
        _readPos = _length - first;
        ::memcpy(((char*)_data) + first, _cb->data_, _readPos);
    }
    else
    {
         _readPos += _length;
    }
}

int Clear(tagCircleBuffer* _cb)
{
    _cb->readPos_ = _cb->writePos_ = 0;
    _cb->msgRead_ = 0;
    _cb->msgWrite_ = 0;

    return 0;
}

int Write(tagCircleBuffer* _cb, const void* _data, size_t _length)
{
    int readPos = _cb->readPos_;
    int writePos = _cb->writePos_;
	// 写的时候不能写到读点
    int maxCapacity = writePos < readPos ?
        readPos - writePos - 1: readPos + _cb->maxLength_ - writePos - 1;

    int realSize = int(_length + sizeof(int) + sizeof(checkData));
    if (maxCapacity < realSize)
    {
        return -1;
    }

    RealWrite(_cb, writePos, &_length, int(sizeof(realSize)));
    RealWrite(_cb, writePos, _data, _length);
    RealWrite(_cb, writePos, &checkData, int(sizeof(checkData)));
    _cb->writePos_ = writePos;
    _cb->msgWrite_++;
    return _length;
}

int WriteV(tagCircleBuffer* _cb, const iovec* _iov, int _iovcnt)
{
    int readPos = _cb->readPos_;
    int writePos = _cb->writePos_;
	// 写的时候不能写到读点
    int maxCapacity = writePos < readPos ?
        readPos - writePos - 1: _cb->maxLength_ - writePos + readPos - 1;

	size_t length = 0;
	for (int i = 0; i < _iovcnt; ++i)
	{
		length += _iov[i].iov_len;
	}
    int realSize = int(length + sizeof(int) + sizeof(checkData));
    if (maxCapacity < realSize)
    {
        return -1;
    }

    RealWrite(_cb, writePos, &length, int(sizeof(realSize)));
	for (int i = 0; i < _iovcnt; ++i)
	{
		RealWrite(_cb, writePos, _iov[i].iov_base, _iov[i].iov_len);
	}
    RealWrite(_cb, writePos, &checkData, int(sizeof(checkData)));
    _cb->writePos_ = writePos;
	_cb->msgWrite_++;
    return int(length);
}

bool CheckSum(unsigned int _check)
{
	XXZ_ASSERT(checkData == _check);
	if (checkData != _check)
	{
		LOG_WARN("Msg data error, invalid check data! Reset Data");
		//TODO: Maybe should log some msg channel data
		return false;
	}
	return true;
}

int Read(tagCircleBuffer* _cb, void* _data, size_t& _maxLength)
{
    int writePos = _cb->writePos_;
    int readPos = _cb->readPos_;
    int maxSize = readPos <= writePos ?
        writePos - readPos : writePos + _cb->maxLength_ - readPos;

    if (maxSize < int(sizeof(int) + sizeof(checkData)))
    {
        _maxLength = 0;
        return -1;
    }

    int msgSize = 0;
    RealRead(_cb, readPos, &msgSize, int(sizeof(msgSize)));

    if (msgSize > int(_maxLength))
    {
         LOG_WARN("Read msg space too small, need %d, supply %d", msgSize, int(_maxLength));
         return -2;
    }

    XXZ_ASSERT(msgSize >= 0
           && int(msgSize + sizeof(checkData) + sizeof(int)) <= maxSize);
    _maxLength = msgSize;
    RealRead(_cb, readPos, _data, _maxLength);
    unsigned int check = 0;
    RealRead(_cb, readPos, &check, int(sizeof(check)));
	if (!CheckSum(check))
	{
		Clear(_cb);
		return -3;
	}

    _cb->readPos_ = readPos;
	_cb->msgRead_++;
    return msgSize;
}
}

//#define CIRCLE_BUFFER_TEST
#ifdef CIRCLE_BUFFER_TEST
#include <stdlib.h>
#include <thread>
#include <unistd.h>
void PushMsg(tagCircleBuffer* _cb, int _loopNum)
{
    char data[128] = "firstData";
    for (int i = 0; i < _loopNum; )
    {
        if (CircleBuffer::Write(_cb, data, strlen(data)) > 0)
        {
            ++i;
            sprintf(data, "Num: %d, ha", i);
        }
        else
        {
            usleep(1);
        }
    }
}

void PopMsg(tagCircleBuffer* _cb, int _loopNum)
{
    char data[128];
    size_t maxLength = sizeof(data);

    for (int i = 0; i < _loopNum; )
    {
        maxLength = sizeof(data);
        if (CircleBuffer::Read(_cb, data, maxLength) >= 0)
        {
            if (0 == i % 2345)
            {
                data[maxLength] = 0;
                printf("Read %d: %s\r\n", i, data);
            }
            ++i;
        }
        else
        {
            usleep(1);
        }
    }
}

int test()
{
    const size_t maxLength = 1025;

    tagCircleBuffer* cb = (tagCircleBuffer*)::malloc(sizeof(tagCircleBuffer) + maxLength);
    cb->maxLength_ = maxLength;
    CircleBuffer::Clear(cb);

    const int loopNum = 100000;

    std::thread t1(PushMsg, cb, loopNum);
    std::thread t2(PopMsg, cb, loopNum);
    //std::thread t1(testThread);
    //std::thread t2(testThread);
    t1.join();
    t2.join();
    free(cb);

    return 0;
}
#endif /* CIRCLE_BUFFER_TEST */

