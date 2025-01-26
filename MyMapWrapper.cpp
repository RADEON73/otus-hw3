#pragma once
#include <type_traits>
#include <memory>
#include <utility>
#include <map>
#include <iostream>
#include <ostream>

template <typename KeyType, typename ValueType, typename Compare = std::less<KeyType>, typename Alloc = std::allocator<std::pair<const KeyType, ValueType>>>
class MyMapWrapper
{
public:
    using MapType = std::map<KeyType, ValueType, Compare, Alloc>;

    MyMapWrapper() = default;

    ValueType& operator[](const KeyType& key)
    {
        return map_[key];
    }

    const ValueType& operator[](const KeyType& key) const
    {
        return map_.at(key);
    }

    void insert(KeyType key, ValueType value)
    {
        map_[key] = value;
    }

    void print() const
    {
        for (const auto& [key, value] : map_) {
            std::cout << key << " " << value << std::endl;
        }
    }

    int factorial(int n) const
    {
        int result = 1;
        for (int i = 1; i <= n; ++i) {
            result *= i;
        }
        return result;
    }

    void fill(int n)
    {
        static std::map<int, int> memo;

        for (int i = 0; i < n; ++i) {
            if (!memo.contains(i)) {
                memo[i] = factorial(i);
            }
            map_[i] = memo[i];
        }
    }


private:
    MapType map_;
};