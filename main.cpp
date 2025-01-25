#include <iostream>
#include <map>
#include "MyContainer.cpp"

int factorial(int n)
{
    int result = 1;
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }
    return result;
}

constexpr int size = 10;

int main()
{
    std::map<int, int> defaultMap;
    for (int i = 0; i < size; ++i)
        defaultMap[i] = factorial(i);
    for (auto const& [num, fact] : defaultMap)
        std::cout << num << " " << fact << std::endl;
    
    using Alloc = FixedSizeAllocator<std::pair<const int, int>>;
    std::map<int, int, std::less<int>, Alloc> myMap;
    for (int i = 0; i < size; ++i)
        myMap[i] = factorial(i);
    for (auto const& [num, fact] : myMap)
        std::cout << num << " " << fact << std::endl;


    MyContainer<int, std::allocator<int>> defaultContainer;
    for (int i = 0; i < 10; ++i)
        defaultContainer.add(i);
    std::cout << "Container with std::allocator: ";
    for (const auto& value : defaultContainer) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    MyContainer<int, FixedSizeAllocator<int>> myContainer;
    for (int i = 0; i < 10; ++i) {
        myContainer.add(i);
    }
    std::cout << "Container with myAllocator: ";
    for (const auto& value : myContainer) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
}