#include "http_worker.h"
#include "http_master.h"
#include "base/log_wrap.h"

HttpWorker::HttpWorker(HttpMaster* master)
	: mutex_(),
	  cond_(),
	  thread_id_(0),
	  master_(master),
	  stop_(false)
{
}

HttpWorker::~HttpWorker()
{
	//std::cout << "worker exit" << std::endl;
	master_ = NULL;	
}

bool HttpWorker::Create()
{
	if (!http_request_.Create())
	{
		return false;
	}

	return true;
}

int HttpWorker::Svc()
{
	thread_id_ = static_cast<unsigned int>(::syscall(SYS_gettid));
	master_->StartCountDown();

	while (true)
	{
		if (stop_)
		{
			goto stop;
		}
		if (-1 == HangUp())
		{
			break;
		}		
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
			if (http_request_.GetResponse(job->url, job->content, job->json_value))
			{
				
			}
			master_->PushFinishJob(job);
		}

	}
stop:
	master_->StopCountDown();

	return 0;
}

int HttpWorker::HangUp()
{
	boost::mutex::scoped_lock lock(mutex_);
	while(job_list_.empty())
	{
		cond_.wait(mutex_);
	}

	return 0;
}

void HttpWorker::PushJob(const JobNodePtr& job_node_ptr)
{
	boost::mutex::scoped_lock lock(mutex_);
	job_list_.push_back(job_node_ptr);
	cond_.notify_one();
}

void HttpWorker::WakeStop()
{
	boost::mutex::scoped_lock lock(mutex_);
	//job_list_.push_back(JobNodePtr(new JobNode()));
	stop_ = true;
	cond_.notify_one();
}
