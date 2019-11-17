#pragma once

#include<iostream>

template <class T>
class DynamicArr
{
public:
	DynamicArr() : arr(NULL), size(0), capacity(0) {}

	DynamicArr(const T& other)
	{
		copy(other);
	}

	DynamicArr& operator=(const T& other)
	{
		if (this != &other)
		{
			delete[] arr;
			copy(other);
		}

		return *this;
	}

	~DynamicArr()
	{
		delete[]  arr;
	}

	const T& operator[](size_t index)const
	{
		if (checkIndex(index))
		{
			return arr[index];
		}
		throw "Invalid index";
	}

	T& operator[](size_t index)
	{
		if (checkIndex(index))
		{
			return arr[index];
		}
		throw "Invalid index";
	}

	void add(const T& what)
	{
		checkSize();
		arr[size++] = what;
	}
	
	void insert(size_t where, const T& what)
	{
		checkSize();
		for (int i = size; i >= where; i--)
		{
			arr[i] = arr[i - 1];
		}
		arr[where] = what;
		size++;
	}

	void remove(size_t index)
	{
		if (checkIndex(index))
		{
			for (int i = index; i < size; i++)
			{
				arr[i] = arr[i + 1];
			}
			size--;
		}
	}

	int find(const T& what)
	{
		for (int i = 0; i < size; i++)
		{
			if (arr[i] == what)
			{
				return i;
			}
		}
		return -1;
	}

	size_t getSize()const
	{
		return size;
	}

	
	/*template<class T>
	friend std::ostream& operator<<(std::ostream& os, const DynamicArr<T>& arr)
	{
		int n = arr.getSize();
		for (int i = 0; i < n; i++)
		{
			os << arr[i] << " ";
		}
		os << std::endl;
		return os;
	}*/

private:
	void copy(const T& other)
	{
		arr = new T[other.size];
		for (int i = 0; i < other.size; i++)
		{
			arr[i] = other.arr[i];
		}
		size = other.size;
		capacity = other.capacity;
	}

	void resize(size_t newCapacity)
	{
		T* buff = new T[newCapacity];
		for (int i = 0; i < size; i++)
		{
			buff[i] = arr[i];
		}
		delete[] arr;
		arr = buff;
		capacity = newCapacity;
	}
	
	void checkSize()
	{
		if (size == capacity)
		{
			resize(capacity ? capacity * 2 : 2);
		}
	}

	bool checkIndex(size_t index)const
	{
		return(index >= 0 && index < size);
	}

private:
	T* arr;
	size_t size;
	size_t capacity;
};