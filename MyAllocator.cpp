#pragma once
#include <iostream>
#include <exception>
#include <utility>

constexpr bool MYALLOCATOR_DEBUG_MESSAGE_ON = false;

template <typename T>
class MyAllocator
{
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;

    template <typename U>
    struct rebind
    {
        using other = MyAllocator<U>;
    };

    template <typename U>
    explicit MyAllocator(const MyAllocator<U>& other) {}

    MyAllocator() = default;
    ~MyAllocator() = default;

    int findFreeMemorySpace(size_t n) const
    {
        for (int i = 0; i < (clusterSize - n); ++i) {
            bool busy = true;
            for (int j = 0; j < n; j++) {
                if (memoryChecksum[i + j]) {
                    busy = false;
                    break;
                }
            }
            if (busy)
                return i;
        }
        return -1;
    }

    T* allocate(std::size_t n)
    {
        int freeMemorySpacePointer = 0;
        try {
            if (n == 0)
                return nullptr;
            if (n > clusterSize)
                throw std::bad_alloc();

            if (!allocated) {
                if (MYALLOCATOR_DEBUG_MESSAGE_ON)
                    std::cout << "Allocating " << sizeof(T) * clusterSize << " bytes for " << clusterSize << " elements\n";
                memoryPool = static_cast<T*>(::operator new(sizeof(T) * clusterSize));
                memoryChecksum = static_cast<bool*>(::operator new(sizeof(bool) * clusterSize));
                allocated = true;
                for (int i = 0; i < clusterSize; ++i)
                    memoryChecksum[i] = false;

            }
            freeMemorySpacePointer = findFreeMemorySpace(n);
            if (freeMemorySpacePointer == -1)
                throw std::bad_alloc();
            for (int i = freeMemorySpacePointer; i < freeMemorySpacePointer + n; i++)
                memoryChecksum[i] = true;

        }
        catch (const std::bad_alloc& e) {
            clusterSize = std::max(static_cast<int>(n), clusterSize * 2);
            if (MYALLOCATOR_DEBUG_MESSAGE_ON)
                std::cerr << "Error: " << e.what() << ". Buffer filled, resize to new buffer : " << clusterSize << std::endl;
            allocated = false;
            allocate(n);
        }
        return &memoryPool[freeMemorySpacePointer];
    }

    void deallocate(T* p, std::size_t n)
    {
        if (p == nullptr || n == 0)
            return;
        int idx = p - &memoryPool[0];
        if (idx < 0 || idx >= clusterSize)
            return;
        for (int i = idx; i < idx + n; ++i)
            memoryChecksum[i] = false;
    };

    void clear()
    {
        if (allocated) {
            for (int i = 0; i < clusterSize; ++i) {
                if (memoryChecksum[i]) {
                    memoryPool[i].~T();
                }
            }
            ::operator delete(memoryPool);
            ::operator delete(memoryChecksum);
        }
        allocated = false;
    }

private:
    T* memoryPool{ nullptr }; // Указатель на выделенную память
    bool* memoryChecksum{ nullptr }; // Указатель на массив с индикацией состояния памяти (true бит - занятый уасток памяти равный Т)
    bool allocated{ false }; // Флаг, указывающий, что память была выделена
    int clusterSize = 15; //Рамер буфера элементов по сколько релоцируется память (не самого количества элементов, а памяти под reserve)
};