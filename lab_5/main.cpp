#include <omp.h>
#include <vector>

#include <iostream>
#include <stdexcept>
#include <cstdint>

#include <time.h>
#include <stdlib.h>
#include <limits>

#include "timer.hpp"

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


bool askYesNo(const char* question_msg)
{
    std::cout << question_msg;
    char answer;
    std::cin >> answer;

    switch(answer)
    {
        case 'y':
            return true;
        case 'n':
            return false;
        default:
            throw std::logic_error("Invalid input");
    }
}

template <typename ValType>
void printVector(std::vector<ValType> vector)
{
    std::cout << "Vector: " << std::endl;
    for (auto elem: vector)
        std::cout << elem << " ";

    std::cout << std::endl;
}

int main()
{
    try
    {
        const int32_t MAX_NUM = 10;
        srand(time(NULL));

        uint32_t size_vectors = askCount("Enter size vectors: ");
        
        std::vector<int32_t> first, second;        
        for (uint32_t i = 0; i < size_vectors; i++)
        {
            first.push_back(rand() % (MAX_NUM + 1));
            second.push_back(rand() % (MAX_NUM + 1));
        }

        if (askYesNo("Do you want print vectors(y/n)? "))
        {
            printVector(first);
            printVector(second);
        }


        int64_t scalar = 0;
        {
            Timer timer;
            #pragma omp parallel for shared(first, second, size_vectors) reduction(+:scalar)
            for (uint32_t i = 0; i < size_vectors; ++i)
                scalar += first[i] * second[i];
        
        }
        std::cout << "Scalar product = " << scalar << std::endl;


        uint32_t real_scalar = 0;

        {
            Timer timer;
            for (uint32_t i = 0; i < size_vectors; i++)
                real_scalar += first[i] * second[i];
        }
        std::cout << "Real scalar product = " << real_scalar << std::endl;
        
        return EXIT_SUCCESS;
    }
    catch (const std::exception& exc)
    {
        std::cout << "Error: " << exc.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cout << "Unrnown error" << std::endl;
        return EXIT_FAILURE;
    }
}
