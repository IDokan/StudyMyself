/* Start Header --------------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Vector.hpp
Purpose: source file for bit vector assignment
Project: sinil.gang / CS225 / Vector
Author: Kang Shinil
Creation date: 10/05/2019
- End Header ----------------------------------------------------------------
*/
#pragma once

template <class T>
typename Vector<T>::Iterator Vector<T>::Begin() const
{
	return Iterator(data, this);
}

template <class T>
typename Vector<T>::Iterator Vector<T>::End() const
{
	return Iterator(data + count, this);
}

template <class T>
typename Vector<T>::Iterator Vector<T>::Find(const T& value)
{
	for (Iterator it = Begin(); it != End(); ++it)
	{
		if (*it == value)
		{
			return it;
		}
	}
	throw("Not Founded!");
}

template <class T>
Vector<T>::Iterator::Iterator(T* pointer, const Vector<T>* ptr)
	:location(pointer), belongsTo(ptr)
{
}

template <class T>
typename Vector<T>::Iterator& Vector<T>::Iterator::operator++() noexcept
{
	++location;
	return *this;
}

template <class T>
typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int) noexcept
{
	T* oldLocation = location;
	++location;
	return Iterator(oldLocation);
}

template <class T>
typename Vector<T>::Iterator& Vector<T>::Iterator::operator--() noexcept
{
	--location;
	return *this;
}

template <class T>
typename Vector<T>::Iterator Vector<T>::Iterator::operator--(int) noexcept
{
	T* oldLocation = location;
	--location;
	return Iterator(oldLocation);
}

template <class T>
bool Vector<T>::Iterator::operator!=(const Iterator& rhs) const noexcept
{
	return (location != rhs.location);
}

template <class T>
bool Vector<T>::Iterator::operator==(const Iterator& rhs) const noexcept
{
	return (location == rhs.location);
}

template <class T>
bool Vector<T>::Iterator::operator<=(const Iterator& rhs) const noexcept
{
	return (location <= rhs.location);
}

template <class T>
bool Vector<T>::Iterator::operator>=(const Iterator& rhs) const noexcept
{
	return (location >= rhs.location);
}

template <class T>
bool Vector<T>::Iterator::operator<(const Iterator& rhs) const noexcept
{
	return (location < rhs.location);
}

template <class T>
bool Vector<T>::Iterator::operator>(const Iterator& rhs) const noexcept
{
	return (location > rhs.location);
}

template <class T>
T& Vector<T>::Iterator::operator*()
{
	if (*this < belongsTo->Begin() || *this >= belongsTo->End())
	{
		throw("Iterator try to dereference with pointing invalid location");
	}
	return *location;
}

template <class T>
typename Vector<T>::Iterator Vector<T>::Iterator::operator+(int offset) noexcept
{
	return Iterator(location + offset, belongsTo);
}

template <class T>
typename Vector<T>::Iterator Vector<T>::Iterator::operator-(int offset) noexcept
{
	return Iterator(location - offset, belongsTo);
}

template <class T>
Vector<T>::Vector()
	:capacity(8), count(0)
{
	data = (T *)malloc(capacity * sizeof(T));
	if (data == nullptr)
	{
		throw("malloc couldn't be worked as we expected!");
	}
}

template <class T>
Vector<T>::~Vector() noexcept
{
	free(data);
	data = nullptr;
}

template <class T>
void Vector<T>::PushBack(const T& value)
{
	++count;
	if (count > capacity)
	{
		capacity *= 2;
		data = (T *)realloc(data, sizeof(T) * capacity);

		if (data == nullptr)
		{
			throw("realloc couldn't be worked as we expected!");
		}
	}

	data[count - 1] = value;
}

template <class T>
void Vector<T>::Remove(Iterator location)
{
	if (location < Begin() || location >= End())
	{
		throw("Remove try to remove at not exist value");
	}
	for (Iterator it = location + 1; it != End(); ++it)
	{
		*(it - 1) = *it;
	}
	--count;
}

template <class T>
void Vector<T>::Insert(Iterator location, const T& value)
{
	if (location < Begin() || location >= End())
	{
		throw("Insert try to insert at not exist value");
	}

	PushBack(value);

	for (Iterator it = End() - 1; it != location; --it)
	{
		*it = *(it - 1);
	}

	*(location + 1) = value;
}

template <class T>
T& Vector<T>::operator[](unsigned int index)
{
	if (index >= count)
	{
		throw("Operator[] is called with out of range!");
	}
	return *(data + index);
}

template <class T>
int Vector<T>::Count() const noexcept
{
	return count;
}

template <class T>
int Vector<T>::Capacity() const noexcept
{
	return capacity;
}

template<class T>
std::ostream& operator<<(std::ostream& out, const Vector<T>& rhs)
{
	for (typename Vector<T>::Iterator it = rhs.Begin(); it != rhs.End(); ++it)
	{
		out << *it << "  ";
	}
	return out;
}