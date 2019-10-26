#pragma once
#include "CRTP.h"

namespace CS225
{
	class AbstractElement abstract : ObjectCounter<int>
	{
	public:
		static size_t live()
		{
			return ObjectCounter<int>::live();
		}
		static size_t total()
		{
			return ObjectCounter<int>::total();
		}
		
		virtual int Get() const = 0;
		virtual void Set(int new_val) = 0;
		virtual void Print() const = 0;

		virtual ~AbstractElement() = default;
		virtual AbstractElement* Clone() = 0;
	};
}