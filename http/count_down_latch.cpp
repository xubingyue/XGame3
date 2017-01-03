#include "count_down_latch.h"

CountDownLatch::CountDownLatch(int _count)
	: mutex_(),
	  cond_(),
	  count_(_count)
{}

void CountDownLatch::Wait()
{
	boost::mutex::scoped_lock lock(mutex_);
	while(count_ > 0)
	{
		cond_.wait(mutex_);
	}
}

void CountDownLatch::CountDown()
{
	boost::mutex::scoped_lock lock(mutex_);
	--count_;
	if (count_ == 0)
	{
		cond_.notify_all();
	}
}

int CountDownLatch::GetCount() const
{
	boost::mutex::scoped_lock lock(mutex_);
	return count_;
}
