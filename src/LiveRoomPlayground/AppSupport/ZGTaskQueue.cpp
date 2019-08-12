#include "stdafx.h"

#include "ZGTaskQueue.h"

#include <queue>
#include <mutex>
#include <atomic>
#include <thread>
#include <condition_variable>

namespace ZEGO
{
    namespace APPSUPPORT
    {
        class TaskQueue::Impl
        {
            friend TaskQueue;

        public:

            explicit Impl(const char * name)
            {        
                task_stoped_.store(false);
                have_task_in_queue_.store(false);
                thread_ = std::thread(std::bind(&Impl::MessageLoopThread, this));
                thread_.detach();
        
            }

            ~Impl()
            {       
                task_stoped_.store(true);
                PostMessage();

                // 等待任务执行完毕
                //thread_.join();

                //ZGLog("this = %ld", this);
        
            }

            void PostMessage()
            {
                have_task_in_queue_.store(true);
                task_condition_var_.notify_one();
            }

        protected:

            void MessageLoopThread()
            {       
                while (true)
                {
                    {
                        std::unique_lock<std::mutex> lock(task_condition_var_mutex_);
                        task_condition_var_.wait(lock, [&] {return have_task_in_queue_.load(); });
                    }

                    ProcessQueuedMessages();

                    //ZGLog("task_released_ = %d", task_have_released_.load());

                    if (task_stoped_.load())
                    {
                        break;
                    }
                }

        
            }

            void ProcessQueuedMessages()
            {       
                while (GetQueueSize() > 0)
                {
                    std::unique_ptr<TaskRunable> task_runable;
                    {
                        std::lock_guard<std::mutex> lock(queue_mutex_);
                        task_runable = std::move(task_queue_.front());
                        task_queue_.pop_front();
                    }

                    task_runable->Run();

        //             if (task_have_released_.load())
        //             {
        //                 break;
        //             }
                }

                have_task_in_queue_.store(false);        
            }

        private:

            int GetQueueSize()
            {
                std::lock_guard<std::mutex> lock(queue_mutex_);
                return task_queue_.size();
            }

            typedef std::deque<std::unique_ptr<TaskRunable> > TaskQueueType;
            TaskQueueType task_queue_;
            std::mutex queue_mutex_;
            std::condition_variable task_condition_var_;
            std::mutex task_condition_var_mutex_;

            std::thread thread_;
            std::atomic<bool> task_stoped_;
            std::atomic<bool> have_task_in_queue_;
        };

        TaskQueue::TaskQueue(const char * queue_name)
            : impl_(new Impl(queue_name))
        {
        }

        TaskQueue::~TaskQueue()
        {

        }

        void TaskQueue::PostTask(std::unique_ptr<TaskRunable> task)
        {
            {
                std::lock_guard<std::mutex> lock(impl_->queue_mutex_);
                impl_->task_queue_.push_back(std::move(task));
            }

            impl_->PostMessage();

        }
    }
}
