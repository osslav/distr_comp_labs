#pragma once

#include <vector>
#include <iostream>
#include <cstdint>

#include <stdlib.h>
#include <limits>

#include <mpi.h>

using BoolArr = std::vector<bool>;
using BoolMatr = std::vector<BoolArr>;


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

void randomizeMatr(BoolMatr& matr, const uint32_t* size_matr, uint32_t count_one_per_zero)
{
    matr.clear();
    matr.reserve(size_matr[0]);
    for (uint32_t i = 0; i < size_matr[0]; ++i)
    {
        matr.push_back(BoolArr(size_matr[1]));
        for (uint32_t j = 0; j < size_matr[1]; ++j)
            matr[i][j] = static_cast<bool>(rand() % (1 + count_one_per_zero));
    }
}

void printMatr(const BoolMatr& matr)
{
    for (auto& str_matr: matr)
    {
        for (auto elem: str_matr)
            if (elem)
                std::cout << "+ ";
            else
                std::cout << "- ";

        std::cout << std::endl;
    }
}

//index for upper left corner of square 2x2
bool checkSquare(const BoolMatr& matr, uint32_t ind_str, uint32_t ind_col)
{
    if (ind_str < 0 || (ind_str + 1) >= matr.size())
        throw std::logic_error("Invalid string index");

    if (ind_col < 0 || (ind_col + 1) >= matr[0].size())
        throw std::logic_error("Invalid string index");

    if (matr[ind_str][ind_col] && matr[ind_str][ind_col + 1] && matr[ind_str + 1][ind_col] && matr[ind_str + 1][ind_col + 1])
        return true;
    else
        if (!(matr[ind_str][ind_col] || matr[ind_str][ind_col + 1] || matr[ind_str + 1][ind_col] || matr[ind_str + 1][ind_col + 1]))
            return true;

    return false;
}

bool haveSquare(BoolMatr& matr)
{
    for (uint32_t i = 0; i < (matr.size() - 1); ++i)
        for (uint32_t j = 0; j < (matr[0].size() - 1); ++j)
            if (checkSquare(matr, i, j))
                    return true;

    return false;
}

void clearSquare(BoolMatr& matr)
{
    if (matr.size() == 0)
        return;

    uint32_t count_str = matr.size();
    uint32_t count_col = matr[0].size();

    for (uint32_t ind_str = 0; ind_str < (count_str - 1); ++ind_str)
        for (uint32_t ind_col = 0; ind_col < (count_col - 1); ++ind_col)
            if (checkSquare(matr, ind_str, ind_col))
                matr[ind_str + 1][ind_col + 1] = !matr[ind_str + 1][ind_col + 1];
}


void clearJointFromSquare(BoolMatr& matr, uint32_t ind_str_joints)
{
    if (matr.size() == 0)
        return;

    uint32_t count_str = matr.size();
    uint32_t count_col = matr[0].size();

    bool joints_cleared = false;
    for (uint32_t ind_str = ind_str_joints; !joints_cleared && ind_str < (count_str - 1); ++ind_str)
    {
        joints_cleared = true;
        for (uint32_t ind_col = 0; ind_col < (count_col - 1); ++ind_col)
            if (checkSquare(matr, ind_str, ind_col))
            {
                joints_cleared = false;
                matr[ind_str + 1][ind_col + 1] = !matr[ind_str + 1][ind_col + 1];
            }
    }
}

void storeToBuf(uint8_t* buffer, uint32_t size_buf, const BoolArr& bool_arr)
{
    uint32_t ind_curr_elem_buff = 0;
    uint8_t count_bit_in_curr_buf_elem = 0;
    buffer[ind_curr_elem_buff] = 0;

    for (uint32_t ind_elem = 0; ind_elem < bool_arr.size(); ++ind_elem)
    {
        buffer[ind_curr_elem_buff] <<= 1;
        
        if (bool_arr[ind_elem])
            ++buffer[ind_curr_elem_buff];

        ++count_bit_in_curr_buf_elem;
        if (count_bit_in_curr_buf_elem >= 8)
        {
            count_bit_in_curr_buf_elem = 0;
            ++ind_curr_elem_buff;

            if (ind_curr_elem_buff < size_buf)
                buffer[ind_curr_elem_buff] = 0;
        }
    }

    buffer[ind_curr_elem_buff] <<= (8 - count_bit_in_curr_buf_elem);
}


void storeToMatr(uint8_t* buffer,uint32_t size_buf, BoolArr& bool_arr)
{
    uint32_t ind_curr_elem_buff = 0;
    
    uint8_t mask = 1;
    mask <<= 7;

    for (uint32_t ind_elem = 0; ind_elem < bool_arr.size(); ++ind_elem)
    {
        if (buffer[ind_curr_elem_buff] & mask)
            bool_arr[ind_elem] = true;
        else
            bool_arr[ind_elem] = false;

        mask >>= 1;

        if (mask == 0)
        {
            mask = 1;
            mask <<= 7;

            ++ind_curr_elem_buff;
        }
    }
}


void sendByteArr(uint8_t* buffer, uint32_t size_buf, uint32_t ind_process)
{
    MPI_Send(buffer, size_buf, MPI_BYTE, ind_process, 100, MPI_COMM_WORLD);
}


void recvByteArr(uint8_t* buffer, uint32_t size_buf, uint32_t ind_process)
{
    MPI_Status out_status;
    MPI_Recv(buffer, size_buf, MPI_BYTE, ind_process, 100, MPI_COMM_WORLD, &out_status);
}


void sendUintArr(uint32_t* arr, uint32_t size_arr, uint32_t ind_process)
{
    MPI_Send(arr, size_arr, MPI_UNSIGNED, ind_process, 10, MPI_COMM_WORLD);
}


void recvUintArr(uint32_t* arr, uint32_t size_arr, uint32_t ind_process)
{
    MPI_Status out_status;
    MPI_Recv(arr, size_arr, MPI_UNSIGNED, ind_process, 10, MPI_COMM_WORLD, &out_status);
}


void main_runtime_func(uint32_t count_process)
{
    srand(time(NULL));
       
    uint32_t size_matr[2];
    size_matr[0] = askCount("Enter count string: ");
    size_matr[1] = askCount("Enter count column: ");
    
    uint32_t count_one_per_zero = askCount("Enter count ones per zeros: ");

    BoolMatr matrix;
    randomizeMatr(matrix, size_matr, count_one_per_zero);

    bool need_print_matr = askYesNo("Do you want print matrix(y/n)? ");
    if (need_print_matr)
    {
        std::cout << "Matrix:" << std::endl;
        printMatr(matrix);
    }

    {
        Timer timer;
            
        uint32_t step_by_string = size_matr[0] / (count_process - 1);
            
        //send block
        uint32_t size_buf = (size_matr[1] / 8) + 1;
        uint8_t* buffer = new uint8_t[size_buf];
        uint32_t ind_curr_worker = 0;
        for (uint32_t ind_curr_str = 0; ind_curr_str < size_matr[0]; ++ind_curr_str)
        {
            if ((ind_curr_str == (step_by_string * ind_curr_worker)) && ((ind_curr_worker + 1) < count_process))
            {
                ++ind_curr_worker;
                uint32_t size_curr_fragment[2] = {step_by_string, size_matr[1]};
                if (ind_curr_worker == (count_process - 1))
                    size_curr_fragment[0] += size_matr[0] % (count_process - 1);

                sendUintArr(size_curr_fragment, 2, ind_curr_worker);
            }

            storeToBuf(buffer, size_buf, matrix[ind_curr_str]);
            sendByteArr(buffer, size_buf, ind_curr_worker);
        }

        //recieve block
        ind_curr_worker = 0;
        for (uint32_t ind_curr_str = 0; ind_curr_str < size_matr[0]; ++ind_curr_str)
        {  
            if (ind_curr_str >= step_by_string * ind_curr_worker && (ind_curr_worker + 1) < count_process)
                ++ind_curr_worker;
       
            recvByteArr(buffer, size_buf, ind_curr_worker);
            storeToMatr(buffer, size_buf, matrix[ind_curr_str]);
                
        }
        delete[] buffer;

        //sync data from diferent workers
        for (uint32_t ind_str = step_by_string; ind_str < size_matr[0]; ind_str += step_by_string)
            clearJointFromSquare(matrix, ind_str - 1);

    }

    if (need_print_matr)
    {
        std::cout << "Result:" << std::endl;
        printMatr(matrix);
    }

    if (haveSquare(matrix))
        throw std::runtime_error("Matrix have square after clearing");
    else
        std::cout << "Good! Matrix dont have square" << std::endl;
}


void worker_runtime_func(uint32_t ind_main)
{
    //get size matrix
    uint32_t size_matr[2];
    recvUintArr(size_matr, 2, ind_main);

    //create matrix
    BoolMatr matrix;
    matrix.reserve(size_matr[0]);
    for (uint32_t i = 0; i < size_matr[0]; ++i)
        matrix.push_back(BoolArr(size_matr[1]));

    //get data
    uint32_t size_buffer = (size_matr[1] / 8) + 1;
    uint8_t* buffer = new uint8_t[size_buffer];
    for (uint32_t ind_str = 0; ind_str < size_matr[0]; ++ind_str)
    {
        recvByteArr(buffer, size_buffer, ind_main);
        storeToMatr(buffer, size_buffer, matrix[ind_str]);
    }

    //edit data
    clearSquare(matrix);

    //send result
    for (uint32_t ind_str = 0; ind_str < size_matr[0]; ++ind_str)
    {
        storeToBuf(buffer, size_buffer, matrix[ind_str]);
        sendByteArr(buffer, size_buffer, ind_main);
    }
    delete[] buffer;
}

