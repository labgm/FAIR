#include <chrono>
#include <unistd.h>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <future>
#include <vector>
#include <thread>
#include <queue>

class ThreadPool
{
public:
    using Task = std::function<void()>;
    
    explicit ThreadPool(std::size_t numThreads)
    {
        start(numThreads);
    }

    ~ThreadPool()
    {
        stop();
    }

    void teste(bool onlyRemove)
    {

    }

    template <class T>
    auto enqueue(T task) -> std::future<decltype(task())>
    {
        auto wrapper = std::make_shared<std::packaged_task<decltype(task())()>>(std::move(task));

        {
            std::unique_lock<std::mutex> lock{mEventMutex};
            mTasks.emplace([=] {
                (*wrapper)();
            });
        }

        mEventVar.notify_one();
        return wrapper->get_future();
        }

private:
    std::vector<std::thread> mThreads;

    std::condition_variable mEventVar;

    std::mutex mEventMutex;
    bool mStopping = false;

    std::queue<Task> mTasks;

    void start(std::size_t numThreads)
    {
        for (auto i = 0u; i < numThreads; ++i)
        {
            mThreads.emplace_back([=] {
                while (true)
                {
                    Task task;

                    {
                        std::unique_lock<std::mutex> lock{mEventMutex};

                        mEventVar.wait(lock, [=] { return mStopping || !mTasks.empty(); });

                        if (mStopping && mTasks.empty())
                            break;

                        task = std::move(mTasks.front());
                        mTasks.pop();
                    }

                    task();
                }
            });
        }
    }

    void stop() noexcept
    {
        {
            std::unique_lock<std::mutex> lock{mEventMutex};
            mStopping = true;
        }

        mEventVar.notify_all();

        for (auto &thread : mThreads)
            thread.join();
    }
};

void execute(int value)
{
    std::cerr << "Value: " << value << std::endl;
}

// int main()
// {
//     {
//         ThreadPool pool{30};

//         // auto f1 = pool.enqueue([] {
//         //     return 1;
//         // });
//         // pool.enqueue([] {
//         //     return 2;
//         // });
//         int dez=10,b=0;
//         for (int i = 0; i < 10; i++)
//         {
//             pool.enqueue([i,dez] {
//                 execute(i);
//                 // usleep(50000);
//                 std::this_thread::sleep_for(std::chrono::milliseconds(5000));
//                 execute(i + dez);
//                 // return i;
//             });
//         }
//     }

//     return 0;
// }