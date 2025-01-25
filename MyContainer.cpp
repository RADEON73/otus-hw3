#include <iostream>
#include <stdexcept>
#include <iterator>
#include "MyAllocator.cpp"

#include <iostream>
#include <memory>
#include <stdexcept>

template <typename T, typename Alloc = std::allocator<T>>
class MyContainer
{
public:
    using value_type = T;
    using allocator_type = Alloc;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;

    MyContainer(const Alloc& alloc = Alloc()) : alloc_(alloc), data_(nullptr), size_(0), capacity_(0) {}

    ~MyContainer()
    {
        clear();
    }

    void add(const T& value)
    {
        if (size_ == capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        new (data_ + size_) T(value);
        ++size_;
    }

    void clear()
    {
        for (size_type i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        alloc_.deallocate(data_, capacity_);
        size_ = 0;
        capacity_ = 0;
        data_ = nullptr;
    }

    size_type size() const
    {
        return size_;
    }

    bool empty() const
    {
        return size_ == 0;
    }

    reference operator[](size_type index)
    {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }

    const_reference operator[](size_type index) const
    {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }

    void reserve(size_type new_capacity)
    {
        if (new_capacity <= capacity_) {
            return;
        }

        pointer new_data = alloc_.allocate(new_capacity);
        for (size_type i = 0; i < size_; ++i) {
            new (new_data + i) T(std::move(data_[i]));
            data_[i].~T();  // ”ничтожаем старые элементы
        }
        alloc_.deallocate(data_, capacity_);
        data_ = new_data;
        capacity_ = new_capacity;
    }

    class iterator
    {
    public:
        iterator(pointer ptr) : ptr_(ptr) {}

        T& operator*() { return *ptr_; }
        T* operator->() { return ptr_; }
        iterator& operator++()
        {
            ++ptr_;
            return *this;
        }
        bool operator!=(const iterator& other) const
        {
            return ptr_ != other.ptr_;
        }

    private:
        pointer ptr_;
    };

    iterator begin()
    {
        return iterator(data_);
    }

    iterator end()
    {
        return iterator(data_ + size_);
    }

private:
    allocator_type alloc_;
    pointer data_;
    size_type size_;
    size_type capacity_;
};