#include <memory>
#include <stdexcept>
#include <iostream>

constexpr bool MYCONTAINER_DEBUG_MESSAGE_ON = false;

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

    explicit MyContainer(const Alloc& alloc = Alloc()) : alloc_(alloc) {}

    MyContainer(const MyContainer& other) : size_(other.size_), capacity_(other.capacity_)
    {
        data_ = alloc_.allocate(capacity_);
        for (size_type i = 0; i < size_; ++i) {
            new (data_ + i) T(other.data_[i]);
        }
    }

    MyContainer(MyContainer&& other) noexcept
        : alloc_(std::move(other.alloc_)), data_(other.data_), size_(other.size_), capacity_(other.capacity_)
    {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    ~MyContainer()
    {
        clear();
    }

    MyContainer& operator=(const MyContainer& other)
    {
        if (this != &other) {
            clear();
            size_ = other.size_;
            capacity_ = other.capacity_;
            data_ = alloc_.allocate(capacity_);
            for (size_type i = 0; i < size_; ++i) {
                new (data_ + i) T(other.data_[i]);
            }
        }
        return *this;
    }

    MyContainer& operator=(MyContainer&& other) noexcept
    {
        if (this != &other) {
            clear();
            alloc_ = std::move(other.alloc_);
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    void insert(const T& value)
    {
        if (size_ == capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        new (data_ + size_) T(value);
        ++size_;
        if (MYCONTAINER_DEBUG_MESSAGE_ON) {
            for (auto i = 0; i < capacity_; ++i)
                std::cout << data_[i] << " ";
            std::cout << std::endl;
        }
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

    reference at(size_type index)
    {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }

    const_reference at(size_type index) const
    {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }

    reference front()
    {
        if (empty()) {
            throw std::out_of_range("Container is empty");
        }
        return data_[0];
    }

    const_reference front() const
    {
        if (empty()) {
            throw std::out_of_range("Container is empty");
        }
        return data_[0];
    }

    reference back()
    {
        if (empty()) {
            throw std::out_of_range("Container is empty");
        }
        return data_[size_ - 1];
    }

    const_reference back() const
    {
        if (empty()) {
            throw std::out_of_range("Container is empty");
        }
        return data_[size_ - 1];
    }

    void reserve(size_type new_capacity)
    {
        if (new_capacity <= capacity_) {
            return;
        }
        pointer new_data = alloc_.allocate(new_capacity);
        if (MYCONTAINER_DEBUG_MESSAGE_ON)
            std::cout << "Container try to reallocated new capacity - " << new_capacity << std::endl;
        for (size_type i = 0; i < size_; ++i) {
            new (new_data + i) T(std::move(data_[i]));
            data_[i].~T();
        }
        alloc_.deallocate(data_, capacity_);
        data_ = new_data;
        capacity_ = new_capacity;
    }

    size_type capacity() const
    {
        return capacity_;
    }

    void print() const
    {
        for (auto& value : *this)
            std::cout << value << " ";
        std::cout << std::endl;
    }

    class const_iterator
    {
    public:
        explicit const_iterator(const_pointer ptr) : ptr_(ptr) {}
        const_reference operator*() const { return *ptr_; }
        const_pointer operator->() const { return ptr_; }
        const_iterator& operator++()
        {
            ++ptr_;
            return *this;
        }

        auto operator<=>(const const_iterator&) const = default;

    private:
        const_pointer ptr_;
    };

    const_iterator begin() const { return const_iterator(data_); }
    const_iterator end() const { return const_iterator(data_ + size_); }

    class iterator
    {
    public:
        explicit iterator(pointer ptr) : ptr_(ptr) {}
        T& operator*() { return *ptr_; }
        T* operator->() { return ptr_; }
        iterator& operator++()
        {
            ++ptr_;
            return *this;
        }

        auto operator<=>(const iterator&) const = default;

    private:
        pointer ptr_;
    };

    iterator begin() { return iterator(data_); }
    iterator end() { return iterator(data_ + size_); }

private:
    allocator_type alloc_; //Аллокатор
    pointer data_{ nullptr }; //Буфер с данными
    size_type size_{ 0 }; //Размер
    size_type capacity_{ 0 }; //Емкость
};