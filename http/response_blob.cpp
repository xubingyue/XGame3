#include "response_blob.h"
#include <stdlib.h>
#include <string.h>

ResponseBlob::ResponseBlob()
{
    buff_ = malloc(DefaultBuffSize);
    buff_size_ = DefaultBuffSize;
    use_size_ = 0;
}

ResponseBlob::~ResponseBlob()
{
	free(buff_);
	buff_ = NULL;
	buff_size_ = 0;
    use_size_ = 0;
}

void ResponseBlob::Reset()
{
    use_size_ = 0;
}

bool ResponseBlob::Realloc(size_t bytes)
{
	if(buff_size_ == bytes)
    {
		return false;
    }

	void *new_data = realloc(buff_, bytes);

	if(!new_data)
    {
        return false;
    }

	buff_ = new_data;
	buff_size_= bytes;
    return true;
}

bool ResponseBlob::Push(void* data, size_t len) 
{
    if (FreeSize() < len)
    {
        size_t new_size = buff_size_;
        while (new_size < len + use_size_)
        {
            new_size = new_size >> 1;
        }

        if (!Realloc(new_size))
        {
            return false;
        }
    }

    void* begin = (void*)((char*)buff_+use_size_);
    memcpy(begin, data, len);
    use_size_ = use_size_ + len;
    return true;
}

size_t ResponseBlob::FreeSize() const
{
    return buff_size_ - use_size_;
}

