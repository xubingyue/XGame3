#ifndef COUNT_DOWN_LATCH_H_
#define COUNT_DOWN_LATCH_H_

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>

//用于多线程之间同步 
//比如主线程等待其他线程初始化完成后才能继续
class CountDownLatch
{
public:
	explicit CountDownLatch(int _count);
	void Wait();
	void CountDown();
	int GetCount() const;
private:
	mutable boost::mutex mutex_;
	boost::condition cond_;
	int count_;
};

#endif // COUNT_DOWN_LATCH_H_
