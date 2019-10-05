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

template<class T>
class Vector
{
public:
	class Iterator
	{
		friend  class Vector;

	public:
		// Pre increment
		Iterator &operator++() noexcept;
		// Post increment.  Note here, this returns a copy of the Iterator from before the value was incremented. The int param is unused and should be unnamed
		Iterator operator++(int) noexcept;
		// Pre decrement 
		Iterator &operator--() noexcept;
		// Post decrement.  Note here, this returns a copy of the Iterator from before the value was incremented. The int param is unused and should unnamed
		Iterator operator--(int) noexcept;

		bool operator !=(const Iterator &rhs) const noexcept;
		bool operator ==(const Iterator &rhs) const noexcept;
		bool operator <=(const Iterator &rhs) const noexcept;
		bool operator >=(const Iterator &rhs) const noexcept;
		bool operator <(const Iterator &rhs) const noexcept;
		bool operator >(const Iterator &rhs) const noexcept;
		T &operator *();
		Iterator operator +(int offset) noexcept;
		Iterator operator -(int offset) noexcept;

	private:
		Iterator() = default;
		explicit Iterator(T*, const Vector<T>*);

		T* location;
		const Vector<T>* belongsTo;
	};

	Vector();
	~Vector() noexcept;
	Vector(const Vector<T> &) = delete;
	Vector<T> operator =(const Vector<T> &) = delete;
	void PushBack(const T& value);
	void Remove(Iterator location);
	void Insert(Iterator location, const T& value);
	T &operator[](unsigned int index);
	int Count() const noexcept;
	int Capacity() const noexcept;
	// Iterator to the start of your data
	Iterator Begin() const;
	// Iterator to the end of your data, this should not be a valid location
	Iterator End() const;
	// If the value is not found return an Iterator to the end of your data
	Iterator Find(const T& value);
	


private:
	unsigned int capacity;
	unsigned int count;
	T* data = nullptr;
};

#include "Vector.hpp"
