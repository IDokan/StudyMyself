#include "array.h"

CS225::Array::Array(int * array, 
		unsigned int _size, 
		const ElementFactory* _pElementFactory
		) 
	: data(new AbstractElement*[_size]),size(_size),
		pElementFactory(_pElementFactory), count (new int(1))
{
	for (unsigned int i=0;i<size;++i) {
		//create Element with id = 1 (that is Element1 )
		data[i] = pElementFactory->MakeElement( 1, array[i] );
	}
}

CS225::Array::Array(const Array& array)
	:data(array.data), size(array.size), pElementFactory(array.pElementFactory), count(array.count)
{
	++(*count);
}

CS225::Array& CS225::Array::operator=(const Array& array)
{
	// Assign when only this and array are different.
	if (this->data != array.data)
	{
		// Deallocate all data
		for (size_t i = 0; i < size; ++i)
		{
			delete data[i];
		}

		data = array.data;
		size = array.size;
		pElementFactory = array.pElementFactory;
	}
	return *this;
}

int CS225::Array::Get(unsigned int pos) const { return data[pos]->Get(); }

void CS225::Array::Set(int id, int pos, int value) { 
	//setter will delete the old and create new 
	//slightly inefficient if new and old have the same type
	delete data[pos];
	data[pos] = pElementFactory->MakeElement(id,value); 
}

void CS225::Array::Print() const {
	for (unsigned int i=0;i<size;++i) data[i]->Print(); 
	std::cout << std::endl;
}
