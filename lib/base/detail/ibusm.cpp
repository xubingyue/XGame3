#include <stdio.h>
#ifndef WIN32
#include <sys/shm.h>
#endif  // WIN32
#include "base/sysv_shm.h"
#include "base/detail/circle_buffer.h"
#include "base/log_wrap.h"
#include "ibusm.h"

namespace detail
{
char* CreateShmVerify(key_t _key)
{
    static char verfiy[128];
    ::sprintf(verfiy, "IBus shm magic info %u by Bingo", int(_key));
    return verfiy;
}

IBUSHead* AttachIBus(key_t _key)
{
	CShmWrapper shm;
	if (0 != shm.CreateShm(_key, 0, CreateShmVerify(_key), true))
	{
		return 0;
	}
	return (IBUSHead*)shm.GetShmData();
}

// 删除对bus的引用
int DetachBus(IBUSHead* _head)
{
#ifdef WIN32
	return 0;
#else
	return ::shmdt((char*)_head - CShmWrapper::VerifyNameLength);
#endif
}

IBUSChannel* GetChannel(IBUSHead* _head, int _srcID, int _dstID)
{
	if (0 == _head)
	{
		return 0;
	}

	for (int i = 0; i < _head->count; ++i)
    {
        IBUSChannel& channel = _head->channeles[i];
		if (_srcID == channel.srcID && _dstID == channel.dstID)
		{
			return &channel;
		}
    }
    return 0;
}

int ClearChannel(IBUSHead* _head, int _srcID, int _dstID)
{
	IBUSChannel* channel = GetChannel(_head, _srcID, _dstID);
	if (0 != channel)
	{
		tagCircleBuffer* cb = reinterpret_cast<tagCircleBuffer*>(
                reinterpret_cast<char*>(_head) + channel->start);
		return ::CircleBuffer::Clear(cb);
	}
	return -1;
}

}
