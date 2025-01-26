#include <iostream>
#include <utility>
#include <memory>
#include <type_traits>
#include "MyMapWrapper.cpp"
#include "MyContainer.cpp"
#include "MyAllocator.cpp"
#include <string>
#include <vector>

int factorial(int n)
{
    int result = 1;
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }
    return result;
}

constexpr int size = 10; //До скольки заполнять массивы

int main()
{
    std::cout << "Map with default std::allocator: " << std::endl;
    MyMapWrapper<int, int> defaultMap;
    for (int i = 0; i < size; ++i)
        defaultMap[i] = factorial(i);
    defaultMap.print();

    std::cout << "Map with myAllocator: " << std::endl;
    using CustomAllocator = MyAllocator<std::pair<const int, int>>;
    MyMapWrapper<int, int, std::less<int>, CustomAllocator> myMap;
    for (int i = 0; i < size; ++i)
        myMap[i] = factorial(i);
    myMap.print();

    std::cout << "Container with default allocator: " << std::endl;
    MyContainer<int, std::allocator<int>> defaultContainer;
    for (int i = 0; i < size; ++i) {
        defaultContainer.insert(i);
    }
    defaultContainer.print();

    std::cout << "Container with myAllocator: " << std::endl;
    MyContainer<int, MyAllocator<int>> myContainer;
    for (int i = 0; i < size; ++i) {
        myContainer.insert(i);
    }
    myContainer.print();
}