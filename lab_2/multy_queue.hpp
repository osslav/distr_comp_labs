#pragma once

#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>

template <typename ValueType>
class MultyQueue
{
    private:
        struct Node
        {
            ValueType elem;
            Node* next = nullptr;

            Node() {}
            Node(ValueType new_elem) 
                : elem(new_elem) {}
        };

        Node* head_;
        std::mutex mutex_head_;
        
        Node* tail_;
        std::mutex mutex_tail_;
        
        std::atomic<uint32_t> size_{0};
        std::condition_variable wait_while_empty_;
    
    public:
        MultyQueue();

        //no-copy
        MultyQueue(const MultyQueue&) = delete;
        MultyQueue& operator=(const MultyQueue&) = delete;

        ~MultyQueue();

        uint32_t size() { return size_.load(); }
        bool isEmpty() { return size_.load() == 0; }

        void enqueue(const ValueType& new_elem);

        ValueType dequeue();
};
 

template <typename ValueType>
MultyQueue<ValueType>::MultyQueue()
{
    Node* dummy_node = new Node;
    head_ = dummy_node;
    tail_ = dummy_node;
}


template <typename ValueType>
MultyQueue<ValueType>::~MultyQueue()
{
    while(head_)
    {
        Node* old_head = head_;
        head_ = old_head->next;
        delete old_head;
    }
}


template <typename ValueType>
void MultyQueue<ValueType>::enqueue(const ValueType& new_elem)
{
    Node* new_node = new Node(new_elem);
    std::lock_guard<std::mutex> lock(mutex_tail_);

    tail_->next = new_node;
    tail_ = new_node;

    if (size_.fetch_add(1) == 0)
        wait_while_empty_.notify_one();
}

template <typename ValueType>
ValueType MultyQueue<ValueType>::dequeue()
{
    std::unique_lock<std::mutex> lock(mutex_head_);

    while(isEmpty())
        wait_while_empty_.wait(lock);
            
    Node* old_head = head_;
    head_ = old_head->next;
    size_.fetch_sub(1);

    delete old_head;
    return head_->elem;
}

