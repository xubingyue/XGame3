#ifndef HTTP_WORKER_H_
#define HTTP_WORKER_H_

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>
#include <list>
#include "http_def.h"
#include "http_request.h"
#include "response_blob.h"

class HttpMaster;
class HttpWorker
{
public:
	explicit HttpWorker(HttpMaster* master);
	~HttpWorker();
	bool Create();
	int Svc();
	int HangUp();
	void PushJob(const JobNodePtr& job_node_ptr);
	void WakeStop();
private:
	boost::mutex mutex_;
	boost::condition cond_;
	unsigned int thread_id_;
	HttpMaster* master_;
	std::list<JobNodePtr> job_list_;
	HttpRequest http_request_;
	bool stop_;
};

#endif // HTTP_WORKER_H_
