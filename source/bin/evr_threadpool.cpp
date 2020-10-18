#include "evr_threadpool.h"

/*
 * Constructor
 *************/
ThreadPool::ThreadPool(size_t numThreads, size_t maxJobsInQueue) : m_workers(numThreads), m_bTerminate(false)
{
    for (std::unique_ptr<std::thread> &worker : m_workers)
    {
        worker.reset(new std::thread(&ThreadPool::loop, this));
    }
}

/*
 * Destructor
 ************/
ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lockList(m_lockJobsList);
        m_bTerminate = true;
        m_notifyJob.notify_all();
    }

    for (std::unique_ptr<std::thread> &worker : m_workers)
    {
        worker->join();
    }
}

/*
 * Schedule a job
 ****************/
void ThreadPool::executeJob(std::function<void()> job, std::function<void()> notificationJob)
{
    std::unique_lock<std::mutex> lockList(m_lockJobsList);
    m_jobs.push_back(std::pair<std::function<void()>, std::function<void()>>(job, notificationJob));
    m_notifyJob.notify_one();
}

/*
 * Retrieve the next job to execute
 **********************************/
std::pair<std::function<void()>, std::function<void()>> ThreadPool::getNextJob()
{
    std::unique_lock<std::mutex> lockList(m_lockJobsList);

    while (!m_bTerminate)
    {
        if (!m_jobs.empty())
        {
            std::pair<std::function<void()>, std::function<void()>> job = m_jobs.front();
            m_jobs.pop_front();
            return job;
        }

        m_notifyJob.wait(lockList);
    }

    throw Terminated("Thread terminated");
}

/*
 * Function executed by each worker
 **********************************/
void ThreadPool::loop()
{
    try
    {
        for (;;)
        {
            std::pair<std::function<void()>, std::function<void()>> job = getNextJob();
            job.first();
            job.second();
        }
    }
    catch (Terminated &e)
    {
    }
}