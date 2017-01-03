#include "http_master.h"
#include "http_worker.h"
#include "base/log_wrap.h"
class ThreadWrapper
{
public:
	ThreadWrapper(HttpWorker* worker) : worker_(worker) {}
	int operator()()
	{
		return worker_->Svc();
	}
private:
	HttpWorker* worker_;
};

HttpMaster::HttpMaster()
	: mutex_(),
	  cond_(),
	  start_count_down_(kWorkerNum),
	  stop_count_down_(kWorkerNum)
{
	for (int i = 0; i < kWorkerNum; i++)
	{
		workers_[i] = NULL;
	}
}

HttpMaster::~HttpMaster()
{
	
}

bool HttpMaster::Create()
{
	for (int i = 0; i < kWorkerNum; i++)
	{
		workers_[i] = new HttpWorker(this);
		if (!workers_[i]->Create())
		{
			return false;
		}

		ThreadWrapper wrapper(workers_[i]);
		thrg_.create_thread(wrapper);
	}

	// wait until all thread worker init success.
	start_count_down_.Wait();

	return true;
}

void HttpMaster::StartCountDown()
{
	start_count_down_.CountDown();
}

void HttpMaster::StopCountDown()
{
	stop_count_down_.CountDown();
}

void HttpMaster::PushFinishJob(const JobNodePtr& job_node)
{
	boost::mutex::scoped_lock lock(mutex_);
	job_list_.push_back(job_node);

	// note:
	// we don't need to notify master because master fetch jobs
	// in main loop and master will never call wait function.
}

void HttpMaster::Close()
{
    for(int i = 0; i < kWorkerNum; i++)
  	{
  		workers_[i]->WakeStop();
    }

	// wait all worker thread stop.
	stop_count_down_.Wait();
  	thrg_.join_all();

}

void HttpMaster::RequestUrl(const std::string& url, const std::string& content, SCallData scalldata, Callback call_back)
{	
	HttpWorker* worker = workers_[scalldata.i32[2] % kWorkerNum];
	JobNodePtr node_ptr = JobNodePtr(new JobNode(url, content, scalldata, call_back));
	worker->PushJob(node_ptr);
}

void HttpMaster::ProcessFinishJobs()
{
	std::list<JobNodePtr> job_list;
	
	// use Multiple buffering to narrow critical area
	{
		boost::mutex::scoped_lock lock(mutex_);
		std::swap(job_list, job_list_);		
	}

	// deal jobs
	std::list<JobNodePtr>::iterator it = job_list.begin();
	for (; it != job_list.end(); ++it)
	{
		JobNodePtr& job = *it;
		job->call_back(job->scalldata, job->json_value);
	}
}
