//Sort.h – заголовочный файл
template<class T>
class Vector
{
public:
	Vector() : ptr(0), _size(0), _capacity(0){}	//констркутор   
	Vector(const Vector<T> &src)						//конструктор копирования
	{
		array = new T[src.size];
		size = src.size; 
		index = src.index;
		for (int i = 0; i < size; i++)
		{
			array[i] = src.array[i];
		}
	}
	Vector<T> operator=(const Vector<T> &a)		//переопределение
	{
		delete[] array;
		array = new T[a.size];
		size = a.size;
		index = a.index;
		for (int i = 0; i < size; i++)
		{
			array[i] = a.array[i];
		}
		return *this;
	}

	void push_back(T const& rhs)			//вставка в конец
	{
		insert(rhs, _size);
	}
	void push_front(T const& rhs)			//вставка в начало
	{
		insert(rhs, 0);
	}

	T& operator[](size_t const index)
	{
		if (index >= _size)
			throw std::runtime_error("Vector::operator[] exception: index >= _size");
		return ptr[index];
	}

	size_t size()const					//размер
	{
		return _size;
	}

	size_t capacity()const				//вместимость
	{
		return _capacity;
	}

	~Vector()						//деструктор
	{
		delete[] ptr;
		ptr = 0;
		_size = 0;
		_capacity = 0;
	}

	class Iterator				//итератор
	{
	public:
		//	Iterator(Node* node) : m_node(node) { }

		explicit Iterator(T* prhs) : ptr(prhs){}
		
		Iterator operator--()		//--
		{
			if (ptr)
				--ptr;
			return *this;
		}
		
		Iterator operator--(int) 
		{
			T* pret = ptr;
			if (ptr)
				--ptr;
			return Iterator(pret);
			
			/*Iterator temp = *this;
			--(*this);
			return temp;*/
		}
		
		Iterator operator++()		//--
		{
			if (ptr)
				++ptr;
			return *this;
		}

		Iterator operator++(int)		//++
		{
			T* pret = ptr;
			if (ptr)
				++ptr;
			return Iterator(pret);
		}

		bool operator==(const Iterator& other) const {	//==
			if (this == &other) {
				return true;
			}
			return ptr == other.ptr;
		}

		bool operator!=(const Iterator& other) const {  //!=
			return !operator==(other);
		}

		T& operator*() const
		{
			return *ptr;
		}

		T* operator->() const
		{
			return ptr;
		}

	private:
		T* ptr;
	};

	Iterator begin()
	{
		return Iterator(ptr);
	}

	Iterator end()
	{
		return Iterator(ptr + _size);
	}

	typedef struct slist_ {
		long val;
		struct slist_ *next;
	} slist;

	private:
	T* ptr;
	T* array;
	size_t _size, _capacity;
	
	void insert(T const& rhs, size_t const index)
	{
		if (index > _size)
			throw std::runtime_error("Vector::insert exception: index > _size");
		if (!ptr)
		{
			ptr = new T[2];
			_capacity = 2;
			ptr[0] = rhs;
			_size = 1;
		}
		else if (_size == _capacity)
		{
			T* ptmp = new T[_capacity * 2];
			size_t i = 0;
			for (i = 0; i < index; ++i)
				ptmp[i] = ptr[i];
			ptmp[index] = rhs;
			for (i = index; i < _size; ++i)
				ptmp[i + 1] = ptr[i];
			delete[] ptr;
			ptr = ptmp;
			ptmp = 0;
			_capacity *= 2;
			++_size;
		}
		else if (index == 0)
		{
			int i = 0;
			for (i = int(_size - 1); i >= 0; --i)
				ptr[i + 1] = ptr[i];
			ptr[index] = rhs;
			++_size;
		}
		else
		{
			T* ptmp = new T[_capacity * 2];
			size_t i = 0;
			for (i = 0; i < index; ++i)
				ptmp[i] = ptr[i];
			ptmp[index] = rhs;
			delete[] ptr;
			ptr = ptmp;
			ptmp = 0;
			++_size;
		}
	}
};
