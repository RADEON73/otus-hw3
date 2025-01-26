#include "gtest/gtest.h"
#include "MyMapWrapper.cpp"
#include "MyContainer.cpp"
#include "MyAllocator.cpp"

constexpr int size = 10; //�� ������� ��������� �������

int factorial(int n)
{
	int result = 1;
	for (int i = 1; i <= n; ++i)
		result *= i;
	return result;
}

// ������������ MyMapWrapper � ��������� �����������
TEST(MyMapWrapperTest, DefaultAllocator)
{
	MyMapWrapper<int, int> map;
	map.fill(size);

	for (int i = 0; i < size; ++i)
		EXPECT_EQ(map[i], factorial(i));
}

// ������������ MyMapWrapper � ��������� �����������
TEST(MyMapWrapperTest, CustomAllocator)
{
	using CustomAllocator = MyAllocator<std::pair<const int, int>>;
	MyMapWrapper<int, int, std::less<int>, CustomAllocator> map;
	map.fill(size);

	for (int i = 0; i < size; ++i)
		EXPECT_EQ(map[i], factorial(i));
}

// ������������ MyContainer � ��������� �����������
TEST(MyContainerTest, DefaultAllocator)
{
	MyContainer<int> container;
	for (int i = 0; i < size; ++i)
		container.insert(i);

	EXPECT_EQ(container.size(), size);

	for (int i = 0; i < size; ++i)
		EXPECT_EQ(container[i], i);
}

// ������������ MyContainer � ��������� �����������
TEST(MyContainerTest, CustomAllocator)
{
	MyContainer<int, MyAllocator<int>> container;

	for (int i = 0; i < size; ++i)
		container.insert(i);

	EXPECT_EQ(container.size(), size);

	for (int i = 0; i < size; ++i)
		EXPECT_EQ(container[i], i);
}

// ������������ ���������� �� �������
TEST(MyContainerTest, Empty)
{
	MyContainer<int> container;
	EXPECT_TRUE(container.empty());

	container.insert(1);
	EXPECT_FALSE(container.empty());
}

// ������������ �� �������������� ������
TEST(MyContainerTest, ReserveMemory)
{
	MyContainer<int> container;

	for (int i = 0; i < size; ++i)
		container.insert(i);

	EXPECT_EQ(container.size(), 10);
	EXPECT_GE(container.capacity(), 16);
}

// ������������ ������� �� �������
TEST(MyContainerTest, AccessByIndex)
{
	MyContainer<int> container;

	for (int i = 0; i < size; ++i)
		container.insert(i);

	// ������ �� �������
	for (int i = 0; i < size; ++i)
		EXPECT_EQ(container[i], i);

	// �������� ������ �� �������
	EXPECT_THROW(container[size], std::out_of_range);
}