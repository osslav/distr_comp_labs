#include "barrier.hpp"

#include <iostream>
#include <stdexcept>
#include <thread>
#include <chrono>

namespace tests
{

    void testSingle()
    {
        std::cout << "testSingle(): start test" << std::endl;
        
        Barrier barrier(1);

        barrier.waitOther();
        barrier.waitOther();
        barrier.waitOther();
    
        std::cout << "testSingle(): test completed successfull" << std::endl << std::endl;
    }

    void testTwoThread()
    {
        std::cout << "testTwoThread(): start test" << std::endl;
        
        Barrier barrier(2);
        
        auto thread_func = [&barrier]
        {
            barrier.waitOther();
            barrier.waitOther();
            barrier.waitOther();
        };

        std::thread first(thread_func);
        std::thread second(thread_func);

        first.join();
        second.join();

        std::cout << "testTwoThread(): test completed successfull" << std::endl << std::endl;
    }

 
    void testWrite()
    {
        std::cout << "testWrite(): start test" << std::endl;
        
        Barrier barrier(2);

        std::atomic<uint32_t> one_wait{0};
        std::atomic<uint32_t> two_wait{0};
        std::atomic<uint32_t> three_wait{0};

        std::atomic<bool> error_detected{false};
        
        auto thread_func = [&barrier, &one_wait, &two_wait, &three_wait, &error_detected]
        {
            one_wait.fetch_add(1);

            barrier.waitOther();

            if (one_wait.load() != 2)
                error_detected.store(true);
            two_wait.fetch_add(1);

            barrier.waitOther();

            if (two_wait.load() != 2)
                error_detected.store(true);
            three_wait.fetch_add(1);

            barrier.waitOther();

            if (three_wait.load() != 2)
                error_detected.store(true);
        };
        
        std::thread first(thread_func);
        std::thread second(thread_func);

        first.join();
        second.join();

        if (error_detected.load())
            throw std::runtime_error("testWrite(): test failed");
        
        std::cout << "testWrite(): test completed successfull" << std::endl << std::endl;
    }
        

    void stressOne() 
    {
        std::cout << "stressOne(): start test" << std::endl;

        Barrier barrier(2);

        auto thread_func = [&barrier]
        {
            for (int32_t i = 0; i < 1005000; i++)
                barrier.waitOther();
        };
        
        std::thread first(thread_func);
        std::thread second(thread_func);
        
        first.join();
        second.join();

        std::cout << "stressOne(): test completed successfull" << std::endl << std::endl;
    }

    void stressTwo() 
    {
        std::cout << "stressTwo(): start test" << std::endl;

        Barrier barrier(2);
        uint32_t test_var = 0;

        auto first_func = [&barrier, &test_var]
        {
            for (int32_t i = 0; i < 1005000; i++)
            {
                if (i % 2 == 0)
                    ++test_var;
                barrier.waitOther();
            }
        };
        
        auto second_func = [&barrier, &test_var]
        {
            for (int32_t i = 0; i < 1005000; i++)
            {
                if (i % 2 == 1)
                    ++test_var;
                barrier.waitOther();
            }
        };

        std::thread first(first_func);
        std::thread second(second_func);
        
        first.join();
        second.join();

        if (test_var != 1005000)
            throw std::runtime_error("stressTwo(): test failed");

        std::cout << "stressTwo(): test completed successfull" << std::endl << std::endl;
    }
}


int main()
{
    try
    {
        tests::testSingle();
        tests::testTwoThread();
        tests::testWrite();

        tests::stressOne();
        tests::stressTwo();

        std::cout << "All test complete successfull" << std::endl;
        return EXIT_SUCCESS;
    }
    catch(const std::exception& exc)
    {
        std::cout << "Error: " << exc.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch(...)
    {
        std::cout << "Unkmown error" << std::endl;
        return EXIT_FAILURE;
    }
}
