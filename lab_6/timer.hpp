#pragma once

#include <iostream>
#include <chrono>

class Timer
{
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
    bool is_stopped_ = false;
public:
    Timer() 
    { 
        start_time_ = std::chrono::high_resolution_clock::now();
        std::cout << "Start timer" << std::endl; 
    }

    ~Timer() { stop(); }

    void stop()
    {
        if (!is_stopped_)
        {
            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> res = end_time - start_time_;
            std::cout << "Time passed: " << res.count() << 's' << std::endl;
        
            is_stopped_ = true;
        }
    }
};

