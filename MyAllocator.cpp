#include <iostream>

template <typename T, size_t N = 100>
class FixedSizeAllocator
{
public:
    using value_type = T;
    using pointer = T*;  // ���������� ����������� ����
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;

    FixedSizeAllocator() = default;
    template <typename U>
    explicit FixedSizeAllocator(const FixedSizeAllocator<U, N>& other) {}

    ~FixedSizeAllocator() = default;

    T* allocate(std::size_t n)
    {
        if (n > N) {
            throw std::bad_alloc();  // ������, ���� ������������� ������, ��� N ���������
        }

        if (!allocated) {
            // �������� ������ ��� N ���������
            std::cout << "Allocating " << sizeof(T) * N << " bytes for " << N << " elements\n";
            data = static_cast<T*>(::operator new(sizeof(T) * N));
            allocated = true;

        }
        // ���������, ��� �� �� ����� �� ������� ���������� ������
        if (currentIndex + n > N) {
            throw std::bad_alloc();  // ��� ������������ �����
        }

        T* ptr = data + currentIndex;
        currentIndex += n;
        return ptr;
    }

    void deallocate(T* p, std::size_t n)
    {
    };

    // ������ ��� ��������������� � ����������� ��������
    void construct(T* p, const T& value)
    {
        new(p) T(value);  // �������� ����������� T(value) ��� �������, �������������� �� ������ p
    }

    void destroy(T* p)
    {
        p->~T();  // �������� ���������� ��� �������
    }

    template <typename U>
    struct rebind
    {
        using other = FixedSizeAllocator<U, N>;
    };

private:
    T* data = nullptr; // ��������� �� ���������� ������
    bool allocated{ false }; // ����, �����������, ��� ������ ���� ��������
    int currentIndex{ -1 }; // ������, �� ������� �� ��������� � ���������� ������
};

// ������������� ��� �������������� ����������
template <typename T1, typename T2, size_t N>
bool operator==(const FixedSizeAllocator<T1, N>&, const FixedSizeAllocator<T2, N>&)
{
    return true;
}

template <typename T1, typename T2, size_t N>
bool operator!=(const FixedSizeAllocator<T1, N>&, const FixedSizeAllocator<T2, N>&)
{
    return false;
}