#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>

class Barrier 
{
    private:
        const uint32_t COUNT_ALL_THREAD;
        uint32_t era_number_{0};
        uint32_t count_waiting_thread_{0};
        
        std::condition_variable wait_other_thread_;
        std::mutex mutex;
    public:
        explicit Barrier(uint32_t count_all_thread)
            : COUNT_ALL_THREAD(count_all_thread) {}

        //no-copy
        Barrier(const Barrier&) = delete;
        Barrier& operator=(const Barrier&) = delete;

        void waitOther()
        {
            std::unique_lock<std::mutex> lock(mutex);

            uint32_t my_era = era_number_;
            ++count_waiting_thread_;
            
            if (count_waiting_thread_ < COUNT_ALL_THREAD)
                while (my_era == era_number_ && count_waiting_thread_ < COUNT_ALL_THREAD)
                    wait_other_thread_.wait(lock);
            else
            {
                ++era_number_;
                count_waiting_thread_ = 0;
                wait_other_thread_.notify_all();
            }
        }
};
 
