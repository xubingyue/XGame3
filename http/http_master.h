#ifndef HTTP_MASTER_H_
#define HTTP_MASTER_H_

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>
#include <list>
#include <string>
#include <set>
#include "count_down_latch.h"
#include "http_def.h"

/**
 * \brief : 未考虑线程的正常退出 
 *  	  对于该业务场景不需要 降低编写复杂度
 */

class HttpWorker;
class HttpMaster
{
public:
	HttpMaster();
	~HttpMaster();
	static const int kWorkerNum = 5;
	bool Create();
	void PushFinishJob(const JobNodePtr& job_node);
	void Close();
	void StartCountDown();
	void StopCountDown();
	void RequestUrl(const std::string& url, const std::string& content, SCallData scalldata, Callback call_back);
	void ProcessFinishJobs();

private:
	HttpWorker* workers_[kWorkerNum];
	std::list<JobNodePtr> job_list_;
	boost::mutex mutex_;
	boost::condition cond_;
	boost::thread_group thrg_;
	CountDownLatch start_count_down_;
	CountDownLatch stop_count_down_;
};

#endif // HTTP_MASTER_H_
