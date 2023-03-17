#include "matrix.hpp"

#include <omp.h>

Matrix::Matrix(uint32_t count_str, uint32_t count_col, int32_t init_val)
    : count_str_(count_str), count_col_(count_col)
{
    matrix_ = new int32_t*[count_str_];

    for(uint32_t str = 0; str < count_str_; ++str)
    {
        matrix_[str] = new int32_t[count_col_];
        for(uint32_t col = 0; col < count_col_; ++col)
            matrix_[str][col] = init_val;
    }
}

Matrix::Matrix(Matrix&& new_matr)
{
    count_str_ = new_matr.count_str_; 
    count_col_ = new_matr.count_col_;
    matrix_ = new_matr.matrix_;

    new_matr.count_str_ = 0;
    new_matr.count_col_ = 0;
    new_matr.matrix_ = nullptr;
}

Matrix& Matrix::operator=(Matrix&& new_matr)
{
    std::swap(new_matr.matrix_, matrix_);
    std::swap(new_matr.count_str_, count_str_);
    std::swap(new_matr.count_col_, count_col_);

    return *this;
}

Matrix::~Matrix()
{
    for(uint32_t i = 0; i < count_str_; ++i)
        delete[] matrix_[i];

    delete[] matrix_;
}

void Matrix::foreachElem(std::function<int32_t(uint32_t, uint32_t)> get_elem_func)
{
    for(uint32_t str = 0; str < count_str_; ++str)
        for(uint32_t col = 0; col < count_col_; ++col)
            matrix_[str][col] = get_elem_func(str, col); 
}

int32_t& Matrix::elem(uint32_t str, uint32_t col)
{
    if (str >= count_str_)
        throw std::logic_error("Invalid string index");

    if (col >= count_col_)
        throw std::logic_error("Invalid column index");

    return matrix_[str][col];
}

void Matrix::transpose()
{
    Matrix new_matr(count_col_, count_str_);

    for(uint32_t str = 0; str < count_str_; ++str)
        for(uint32_t col = 0; col < count_col_; ++col)
            new_matr.matrix_[col][str] = matrix_[str][col];

    this->operator=(std::move(new_matr));
}


Matrix Matrix::multiStrToCol(const Matrix& another_matr)
{
    if (count_str_ != another_matr.count_col_ || count_col_ != another_matr.count_str_)
        std::logic_error("Incorrect matrix dimensions");

    Matrix res_matr(count_str_, another_matr.count_col_);
 
    uint32_t count_ = count_col_;
    int32_t** matrix = matrix_;
    int32_t** a_matrix = another_matr.matrix_;
 
    auto calc_elem_func = [ matrix, a_matrix, count_](uint32_t str, uint32_t col)
    {
        int32_t res = 0;
        for(uint32_t i = 0; i < count_; ++i)
            res += matrix[str][i] * a_matrix[i][col]; 

        return res;
    };

    res_matr.foreachElem(calc_elem_func);

    return res_matr;
}


Matrix Matrix::multiColToStr(const Matrix& another_matr)
{
    //if (count_str_ != another_matr.count_col_ || count_col_ != another_matr.count_str_)
        //std::logic_error("Incorrect matrix dimensions");

    Matrix res_matr(count_col_, another_matr.count_str_);
 
    uint32_t count_ = count_str_;
    int32_t** matrix = matrix_;
    int32_t** a_matrix = another_matr.matrix_;
 
    auto calc_elem_func = [ matrix, a_matrix, count_](uint32_t str, uint32_t col)
    {
        int32_t res = 0;
        for(uint32_t i = 0; i < count_; ++i)
            res += matrix[i][str] * a_matrix[col][i]; 

        return res;
    };

    res_matr.foreachElem(calc_elem_func);

    return res_matr;
}

Matrix Matrix::multiStrToStr(const Matrix& another_matr)
{
    //if (count_str_ != another_matr.count_col_ || count_col_ != another_matr.count_str_)
        //std::logic_error("Incorrect matrix dimensions");

    Matrix res_matr(count_str_, another_matr.count_str_);
 
    uint32_t count_ = count_col_;
    int32_t** matrix = matrix_;
    int32_t** a_matrix = another_matr.matrix_;
 
    auto calc_elem_func = [ matrix, a_matrix, count_](uint32_t str, uint32_t col)
    {
        int32_t res = 0;
        for(uint32_t i = 0; i < count_; ++i)
            res += matrix[str][i] * a_matrix[col][i]; 

        return res;
    };

    res_matr.foreachElem(calc_elem_func);

    return res_matr;
}

Matrix Matrix::multiColToCol(const Matrix& another_matr)
{
    //if (count_str_ != another_matr.count_col_ || count_col_ != another_matr.count_str_)
        //std::logic_error("Incorrect matrix dimensions");

    Matrix res_matr(count_col_, another_matr.count_col_);
 
    uint32_t count_ = count_str_;
    int32_t** matrix = matrix_;
    int32_t** a_matrix = another_matr.matrix_;
 
    auto calc_elem_func = [ matrix, a_matrix, count_](uint32_t str, uint32_t col)
    {
        int32_t res = 0;
        for(uint32_t i = 0; i < count_; ++i)
            res += matrix[i][str] * a_matrix[i][col]; 

        return res;
    };

    res_matr.foreachElem(calc_elem_func);

    return res_matr;
}
