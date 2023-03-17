#include "multy_queue.hpp"

#include <iostream>
#include <stdexcept>
#include <thread>
#include <chrono>

namespace tests
{

    void testWriteRead()
    {
        std::cout << "testWriteRead(): start test" << std::endl;
        
        MultyQueue<int32_t> test_queue;

        const int32_t TEST_VAL = 1;

        test_queue.enqueue(TEST_VAL);

        if (test_queue.dequeue() != TEST_VAL)
            throw std::runtime_error("testWriteRead(): invalid elem");

        if (!test_queue.isEmpty())
            throw std::runtime_error("testWriteRead(): queue not empty");
    
        std::cout << "testWriteRead(): test completed successfull" << std::endl << std::endl;
    }
 
    void testWait()
    {
        std::cout << "testWait(): start test" << std::endl;
        
        MultyQueue<int32_t> test_queue;

        std::thread reader([&test_queue]
                { 
                    for (int i = 0; i < 3; i++)
                        test_queue.dequeue();
                });

        std::thread writer([&test_queue]
                { 
                    for (int i = 0; i < 3; i++)
                    {
                        std::this_thread::yield();
                        std::this_thread::sleep_for(std::chrono::milliseconds(500));    
                        test_queue.enqueue(1);
                    }
                });

        writer.join();
        reader.join();

        if (!test_queue.isEmpty())
            throw std::runtime_error("testWait(): queue not empty");
        
        std::cout << "testWait(): test completed successfull" << std::endl << std::endl;
    }
        

    void stressOne() 
    {
        std::cout << "stressOne(): start test" << std::endl;

        MultyQueue<int32_t> test_queue;
        
        auto writer_func = [&test_queue]
        {
            for (int32_t i = 0; i < 1005000; i++)
                test_queue.enqueue(i);
        };

        std::atomic<uint32_t> count_invalid_elem{0};
        auto reader_func = [&test_queue, &count_invalid_elem]
        {
            for (int32_t i = 0; i < 1005000; i++)
                if (test_queue.dequeue() != i)
                    count_invalid_elem.fetch_add(1);

        };

        std::thread writer(writer_func);
        std::thread reader(reader_func);
        
        writer.join();
        reader.join();

        if (count_invalid_elem.load() != 0)
            throw std::runtime_error("stressOne(): detected invalid elem in queue");

        if (!test_queue.isEmpty())
            throw std::runtime_error("stressOne(): queue not empty");
        
        std::cout << "stressOne(): test completed successfull" << std::endl << std::endl;
    }

    void stressTwo() 
    {
        std::cout << "stressTwo(): start test" << std::endl;

        MultyQueue<int32_t> test_queue;
        
        auto writer_func = [&test_queue]
        {
            for (int32_t i = 0; i < 100500; i++)
                test_queue.enqueue(0);
        };

        auto reader_func = [&test_queue]
        {
            for (int32_t i = 0; i < 100500; i++)
                test_queue.dequeue();
        };

        std::thread writer_one(writer_func);
        std::thread reader_one(reader_func);
        std::thread writer_two(writer_func);
        std::thread reader_two(reader_func);
        
        writer_one.join();
        writer_two.join();
        reader_one.join();
        reader_two.join();

        if (!test_queue.isEmpty())
            throw std::runtime_error("stressTwo(): test failed");
        
        std::cout << "stressTwo(): test completed successfull" << std::endl << std::endl;
    }
}


int main()
{
    try
    {
        tests::testWriteRead();
        tests::testWait();

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
