#include "gtest/gtest.h"
#include "MyMapWrapper.cpp"
#include "MyContainer.cpp"
#include "MyAllocator.cpp"

constexpr int size = 10; //До скольки заполнять массивы

int factorial(int n)
{
	int result = 1;
	for (int i = 1; i <= n; ++i)
		result *= i;
	return result;
}

// Тестирование MyMapWrapper с дефолтным аллокатором
TEST(MyMapWrapperTest, DefaultAllocator)
{
	MyMapWrapper<int, int> map;
	map.fill(size);

	for (int i = 0; i < size; ++i)
		EXPECT_EQ(map[i], factorial(i));
}

// Тестирование MyMapWrapper с кастомным аллокатором
TEST(MyMapWrapperTest, CustomAllocator)
{
	using CustomAllocator = MyAllocator<std::pair<const int, int>>;
	MyMapWrapper<int, int, std::less<int>, CustomAllocator> map;
	map.fill(size);

	for (int i = 0; i < size; ++i)
		EXPECT_EQ(map[i], factorial(i));
}

// Тестирование MyContainer с дефолтным аллокатором
TEST(MyContainerTest, DefaultAllocator)
{
	MyContainer<int> container;
	for (int i = 0; i < size; ++i)
		container.insert(i);

	EXPECT_EQ(container.size(), size);

	for (int i = 0; i < size; ++i)
		EXPECT_EQ(container[i], i);
}

// Тестирование MyContainer с кастомным аллокатором
TEST(MyContainerTest, CustomAllocator)
{
	MyContainer<int, MyAllocator<int>> container;

	for (int i = 0; i < size; ++i)
		container.insert(i);

	EXPECT_EQ(container.size(), size);

	for (int i = 0; i < size; ++i)
		EXPECT_EQ(container[i], i);
}

// Тестирование контейнера на пустоту
TEST(MyContainerTest, Empty)
{
	MyContainer<int> container;
	EXPECT_TRUE(container.empty());

	container.insert(1);
	EXPECT_FALSE(container.empty());
}

// Тестирование на резервирование памяти
TEST(MyContainerTest, ReserveMemory)
{
	MyContainer<int> container;

	for (int i = 0; i < size; ++i)
		container.insert(i);

	EXPECT_EQ(container.size(), 10);
	EXPECT_GE(container.capacity(), 16);
}

// Тестирование доступа по индексу
TEST(MyContainerTest, AccessByIndex)
{
	MyContainer<int> container;

	for (int i = 0; i < size; ++i)
		container.insert(i);

	// Доступ по индексу
	for (int i = 0; i < size; ++i)
		EXPECT_EQ(container[i], i);

	// Проверка выхода за пределы
	EXPECT_THROW(container[size], std::out_of_range);
}