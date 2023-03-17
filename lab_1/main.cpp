#include "atomic_array.hpp"

#include <thread>
#include <cstdint>
#include <iostream>
#include <vector>
#include <time.h>

uint32_t askCount(const char* question_msg)
{
    std::cout << question_msg;
    uint32_t count_msg = 0;
    std::cin >> count_msg;

    if (!std::cin)
    {
	    std::cin.clear();
	    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	    throw std::runtime_error("Invalid input");
    }
    return count_msg;
}

void synсhronizationSizes(uint32_t& size_arr, uint32_t& count_threads)
{
    if (count_threads > size_arr)
        {
            std::cout << "Count threads cant be > size array" << std::endl;
            count_threads = size_arr;
            std::cout << "Count threads = " << size_arr << " now" << std::endl;
        }
}

int main()
{
    const int32_t INIT_VALUE = 0;
    const int32_t WRITE_VALUE = 42;

    try
    {
        uint32_t size_arr = askCount("Enter size array: ");
        uint32_t count_threads = askCount("Enter count threads: ");

        synсhronizationSizes(size_arr, count_threads);

        AtomicArray<int32_t> arr_nums(size_arr, INIT_VALUE);
        std::vector<std::thread> vec_threads;
        
        std::atomic<uint32_t> count_overwrites{0};

        auto start_time = clock();
        std::atomic<uint32_t> next_elem{0};

        for (uint32_t thread_num = 0; thread_num < count_threads; thread_num++)
            vec_threads.push_back(std::move(std::thread(
                            [&count_overwrites, &arr_nums, &next_elem]
                            {
                                uint32_t curr_index = next_elem.fetch_add(1);
                                while(curr_index < arr_nums.size())
                                {
                                    if (arr_nums.exchange(curr_index, WRITE_VALUE) == WRITE_VALUE)
                                        count_overwrites.fetch_add(1);

                                    curr_index = next_elem.fetch_add(1);
                                }
                            })));

        for (uint32_t thread_num = 0; thread_num < count_threads; thread_num++)
            vec_threads[thread_num].join();

        auto end_time = clock();
        std::cout << "Time passed: " << double(end_time - start_time) / CLOCKS_PER_SEC << 's' << std::endl;

        if (count_overwrites.load() != 0)
            throw std::runtime_error("Overwrites detected");

        if (arr_nums.checkInArray(INIT_VALUE))
            throw std::runtime_error("Unwrited elem found");

        return EXIT_SUCCESS;
    }
    catch(const std::exception& exc)
    {
        std::cout << "Error: " << exc.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch(...)
    {
        std::cout << "Unknown error" << std::endl;
        return EXIT_FAILURE;
    }
}
