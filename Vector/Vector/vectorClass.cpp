#include<iostream>
using namespace std;
#include<assert.h>
#include<string>

//vector的底层结构为可存放任意类型的动态顺序表
//由于可存放任意类型，所以用模板来进行模拟实现
template <class T> 
class Vector
{
public:
	//Vector的迭代器实际就是一个原生态指针
	typedef T* iterator;
	typedef const T* const_iterator;

public:
	////////////////////////////////////////////////////////////
	//构造与析构函数
	Vector()  //无参构造函数
		:_start(nullptr)
		, _finish(nullptr)
		, _endOfStorage(nullptr)
	{}

	Vector(int n, const T& val = T())  //带参构造函数：用n个val填充
		:_start(new T[n])
	{
		for (int i = 0; i < n; ++i)
		{
			_start[i] = val;
		}
		_finish = _start + n;
		_endOfStorage = _finish;
	}

	Vector(const Vector<T>& v)  //拷贝构造
		:_start(nullptr)
		, _finish(nullptr)
		, _endOfStorage(nullptr)
	{
		//开辟空间，防止浅拷贝
		Reserve(v.Capacity());

		//拷贝元素
		iterator it = begin();
		const_iterator const_it = v.const_begin();
		while (const_it != v.const_end())
		{
			*it++ = *const_it++;
		}

		//更新指针的指向
		_finish = _start + v.Size();
		_endOfStorage = _start + v.Capacity();
	}

	//如果使用iterator做迭代器，会导致初始化的迭代器区间[first, last)只能是vector的迭代器
	//重新声明迭代器，迭代器区间[first, last)可以是任意容器的迭代器
	template<class InputIterator>
	Vector(InputIterator first, InputIterator last)  //区间构造
	{
		Reserve(last - first);
		while (first != last)
		{
			Push_Back(*first);
			++first;
		}
	}

	~Vector()  //析构函数
	{
		if (_start)
		{
			delete[] _start;
			_start = nullptr;
			_finish = nullptr;
			_endOfStorage = nullptr;
		}
	}

	////////////////////////////////////////////////////////////
	//迭代器
	iterator begin()
	{
		return _start;
	}
	iterator end()
	{
		return _finish;
	}

	//反向迭代器
	const_iterator const_begin()const
	{
		return _start;
	}
	const_iterator const_end()const
	{
		return _finish;
	}

	////////////////////////////////////////////////////////////
	//空间增长操作
	size_t Size()const
	{
		return _finish - _start;
	}

	size_t Capacity()const
	{
		return _endOfStorage - _start;
	}

	bool Empty()
	{
		if (_start == _finish)
		{
			return true;  //空
		}
		return false;  
	}

	void Resize(size_t newSize, const T& val = T())  //重新规定空间中元素的个数，若规定后存在未装载元素的空间，用val填充
	{
		//若新大小 小于 旧的大小，直接更新_finish，返回； 否则要对未装载元素的空间进行赋值
		if (newSize < size())
		{
			_finish = _start + newSize;
			return;
		}
		//若新大小大于总容量，扩容
		if (newSize > Capacity())
		{
			Reserve(newSize);
		}
		//对未装载元素的空间赋值val
		iterator it = _finish;  //先用旧的尾作为新的遍历头
		iterator _finish = _start + newSize;  //再更新 新的尾，并且作为遍历的终点
		while (it != _finish)
		{
			*it++ = val;
		}
	}

	void Reserve(size_t newCapacity)  //扩容
	{
		if (newCapacity > Capacity())
		{
			//-----1、开辟新空间
			T* temp = new T[newCapacity];
			//-----2、资源拷贝
			size_t n = Size();
			if (_start) //如果_start指向的空间存在时
			{
				for (size_t i = 0; i < n; ++i)
				{
					temp[i] = _start[i];
				}
				//-----3、释放旧空间
				delete[] _start;
			}
			//更新指针的指向
			_start = temp;
			_finish = _start + n;
			_endOfStorage = _start + newCapacity;
		}
		else
		{
			_finish = _start + newCapacity;
		}
	}

	////////////////////////////////////////////////////////////
	//增删查改
	Vector<T>& operator=(Vector<T> v)
	{
		Swap(v);
		return *this;
	}

	void Push_Back(const T& c)  //尾插一个元素c
	{
		Insert(end(), c);
	}

	void Pop_Back()  //删掉最后一个元素
	{
		--_finish;
	}

	iterator Insert(iterator pos, const T& c)  //插入一个元素，并返回其插入位置的地址
	{
		//断言，看插入的位置是否合法
		assert(pos >= begin() && pos <= end());

		//判断是否需要扩容
		if (_finish == _endOfStorage)
		{
			size_t size = Size();
			size_t newCapacity = (0 == Capacity()) ? 1 : Capacity() * 2;
			Reserve(newCapacity);
			//由于扩容引起的空间地址发生变化，导致需要重置pos
			pos = _start + size;
		}
		//将pos位及以后的元素全部向后移动一位
		iterator it = end();
		while (it > pos)
		{
			*it = *(it - 1);
			--it;
		}
		//pos位插入c
		*pos = c;
		//更新_finish
		++_finish;

		return pos;
	}

	iterator Earse(iterator pos)  //删除pos位置的元素，返回删除位置
	{
		//判断删除位置的合法性
		assert(pos >= begin() && pos < end());

		iterator it = pos;
		while (it != end() - 1)
		{
			*it = *(it + 1);
			++it;
		}
		--_finish;

		return pos;
	}

	void Swap(Vector<T>& v)
	{
		swap(_start, v._start);
		swap(_finish, v._finish);
		swap(_endOfStorage, v._endOfStorage);
	}

	T& operator[](size_t pos)
	{
		assert(pos < Size());
		return _start[pos];
	}
	const T& operator[](size_t pos)const
	{
		assert(pos < Size());
		return _start[pos];
	}
	////////////////////////////////////////////////////////////
	//
private:
	iterator _start;  //指向顺序表内第一个元素
	iterator _finish;  //指向顺序表内最后一个元素的末尾(下一个位置)
	iterator _endOfStorage;  //指向该空间中的末尾(最后一个位置)
};

void TestVector1()
{
	Vector<int> v1;
	Vector<int> v2(5, 100);
	Vector<int> v3(v2.begin() + 1, v2.end());
	Vector<int> v4(v3);
}
void TestVector2()
{
	Vector<char> v;
	cout << v.Size() << endl;
	cout << v.Capacity() << endl;

	v.Push_Back('a');
	v.Push_Back('b');
	v.Push_Back('c');
	v.Push_Back('d');
	cout << v.Size() << endl;
	cout << v.Capacity() << endl;

	v.Pop_Back();
	v.Pop_Back();
	cout << v.Size() << endl;
	cout << v.Capacity() << endl;
}
void  TestVector3()
{
	Vector<string> v;
	v.Push_Back("1111");
	v.Push_Back("22222");
	v.Push_Back("333333");
	v.Push_Back("000");

	for (size_t i = 0; i < v.Size(); i++)
	{
		cout << v[i] << endl;
	}
}

int main()
{
	// TestVector1();
	// TestVector2();
	TestVector3();
	return 0;
}