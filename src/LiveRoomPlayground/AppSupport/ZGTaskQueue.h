//
// ZegoTaskQueue.h
//
// Copyright 2018Äê Zego. All rights reserved.
//

#ifndef ZegoTaskQueue_h__
#define ZegoTaskQueue_h__

#include <memory>
namespace ZEGO
{
    namespace APPSUPPORT
    {

        class TaskRunable
        {
        public:
            TaskRunable() {}
            virtual ~TaskRunable() {}
            virtual bool Run() = 0;
        };

        template<class ClosureType>
        class ClosureTaskRunable : public TaskRunable
        {
        public:
            explicit ClosureTaskRunable(ClosureType && closure)
                :closure_(std::forward<ClosureType>(closure))
            {
            }

        private:
            bool Run() override
            {
                closure_();
                return true;
            }

            typename std::remove_const<typename std::remove_reference<ClosureType>::type>::type closure_;
        };

        class TaskQueue
        {
        public:

            explicit TaskQueue(const char * queue_name = 0);

            ~TaskQueue();

            template<class ClosureType>
            void AsyncRun(ClosureType&& closure)
            {
                std::unique_ptr<TaskRunable> t(new ClosureTaskRunable<ClosureType>((std::forward<ClosureType>(closure))));
                PostTask(std::move(t));
            }

        protected:

            void PostTask(std::unique_ptr<TaskRunable> task);

        private:
            class Impl;
            std::shared_ptr<Impl> impl_;
        };

    }
}


#endif // ZegoTaskQueue_h__

