#pragma once

#include <atomic>
#include <cstdint>
#include <stdexcept>

template <typename ValueType>
class AtomicArray
{
private:
    std::atomic<ValueType>* arr_;
    uint32_t size_;

    void checkIndex_(uint32_t index)
    {
        if (index >= size_)
            throw std::runtime_error("Invalid index");
    }

public:
    AtomicArray(uint32_t size, ValueType val)
        : size_(size)
    {
        arr_ = new std::atomic<ValueType>[size];
        for (uint32_t i = 0; i < size; i++)
            arr_[i] = val;
    }

    AtomicArray(const AtomicArray&) = delete;
    AtomicArray& operator=(const AtomicArray&) = delete;

    ~AtomicArray() { delete arr_; }

    uint32_t size() { return size_; }
    void set(uint32_t index_elem, ValueType new_elem)
    {
        checkIndex_(index_elem);
        arr_[index_elem].store(new_elem, std::memory_order_relaxed);
    }

    ValueType get(uint32_t index_elem)
    {
        checkIndex_(index_elem);
        return arr_[index_elem].load(std::memory_order_relaxed);
    }

    ValueType exchange(uint32_t index_elem, ValueType new_elem)
    {
        checkIndex_(index_elem);
        return arr_[index_elem].exchange(new_elem, std::memory_order_relaxed);
    }

    bool checkInArray(ValueType val)
    {
        for (uint32_t index_elem = 0; index_elem < size_; index_elem++)
            if (arr_[index_elem].load(std::memory_order_relaxed) == val)
                return true;

        return false;
    }
};
