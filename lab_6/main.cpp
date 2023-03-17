#include "matrix.hpp"
#include "timer.hpp"

#include <omp.h>
#include <vector>

#include <iostream>
#include <stdexcept>
#include <cstdint>

#include <time.h>
#include <stdlib.h>
#include <limits>

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

void printMatr(Matrix& matr)
{
    for (uint32_t str = 0; str < matr.countStr(); ++str)
    {
        for (uint32_t col = 0; col < matr.countCol(); ++col)
            std::cout << matr.elem(str, col) << " ";

        std::cout << std::endl;
    }
}

void randomizeMatr(Matrix& matr, int32_t max_val = 10)
{
    matr.foreachElem([max_val](uint32_t str, uint32_t col)
            { return static_cast<int32_t>(rand() % max_val); });
}

int main()
{
    try
    {
        srand(time(NULL));
        
        const int32_t first_count_str = askCount("Enter count string: ");
        const int32_t first_count_col = askCount("Enter count column: ");

        Matrix first(first_count_str, first_count_col);
        randomizeMatr(first);
        Matrix second(first_count_col, first_count_str);
        randomizeMatr(second);

        bool need_print_matr = askYesNo("Do you want print matrix(y/n)?  ");
        if (need_print_matr)
        {
            std::cout << "Matrix #1:" << std::endl;
            printMatr(first);
            std::cout << "Matrix #2" << std::endl;
            printMatr(second);
        }

        Matrix res(1, 1);

        {
            std::cout << std::endl << "Start multiply str to col(classic)" << std::endl;
            Timer timer;
            res = first.multiStrToCol(second);
        }
        if (need_print_matr)
        {
            std::cout << "Result:" << std::endl;
            printMatr(res);
        }


        {
            std::cout << std::endl << "Start multiply str to str" << std::endl;
            Timer timer;
            second.transpose();
            res = first.multiStrToStr(second);
        }
        if (need_print_matr)
        {
            std::cout << "Result:" << std::endl;
            printMatr(res);
        }


        {
            std::cout << std::endl << "Start multiply col to str" << std::endl;
            Timer timer;
            first.transpose();
            res = first.multiColToStr(second);
        }
        if (need_print_matr)
        {
            std::cout << "Result:" << std::endl;
            printMatr(res);
        }


        {
            std::cout << std::endl << "Start multiply col to col" << std::endl;
            Timer timer;
            second.transpose();
            res = first.multiColToCol(second);
        }
        if (need_print_matr)
        {
            std::cout << "Result:" << std::endl;
            printMatr(res);
        }

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
