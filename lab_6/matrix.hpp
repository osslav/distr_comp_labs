#pragma once

#include <iostream>
#include <stdlib.h>
#include <stdexcept>
#include <utility>
#include <functional>

class Matrix
{
private:
    int32_t** matrix_;

    uint32_t count_str_;
    uint32_t count_col_;

public:
    Matrix(uint32_t count_str, uint32_t count_col, int32_t init_val = 0);

    Matrix(const Matrix&) = delete;
    Matrix& operator=(const Matrix&) = delete;

    Matrix(Matrix&& new_matr);
    Matrix& operator=(Matrix&& new_matr);
    
    ~Matrix();

    void foreachElem(std::function<int32_t(uint32_t, uint32_t)> get_elem_func);
    int32_t& elem(uint32_t str, uint32_t col);

    uint32_t countStr() const { return count_str_; }
    uint32_t countCol() const { return count_col_; }

    void transpose();

    Matrix multiStrToCol(const Matrix& another_matr);
    Matrix multiColToStr(const Matrix& another_matr);
    Matrix multiStrToStr(const Matrix& another_matr);
    Matrix multiColToCol(const Matrix& another_matr);
};

