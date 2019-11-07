#include <iostream>
#include <string>
#include <vector>

class UndoString : public std::string
{
private:
	struct UndoData;

public:
	UndoString(const std::string& s) : std::string(s) {}

	// This operator replaces std::string's non-const operator [].
	UndoData operator [] (const size_t index) noexcept
	{
		return UndoData  { *this, index };
	}

	void undo()
	{
		if (undoStack.empty())
		{
			throw std::exception{ "Nothing to undo!" };
		}
		UndoData history = undoStack.back();
		this->std::string::operator[](history.pos) = history.oldValue;
		undoStack.pop_back();
	}

private:
	struct UndoData
	{
		UndoString& str;
		size_t pos;
		char oldValue;

		UndoData(UndoString& sourceString, size_t position) : str(sourceString), pos(position), oldValue(0) {}

		// Invoked when proxy is used to modify the value.
		void operator = (const char& rhs)
		{
			char& character = str.std::string::operator[](pos);
			oldValue = character;
			character = rhs;
			str.undoStack.push_back(*this);
		}

		// Invoked when proxy is used to read the value.
		// This converts UndoData to a const char &
		operator const char& () const
		{
			return (str.std::string::operstor[](pos));
		}
	};

	std::vector<UndoData> undoStack;
};

int main(void)
{
	try {
		UndoString ustr("C++ undo string using proxy");
		std::cout << ustr << std::endl;		// C++ undo string using proxy
		ustr[0] = 'c';
		std::cout << ustr[0] << std::endl;		// c
		std::cout << ustr << std::endl;		// c++ undo string using proxy
		ustr.undo();
		std::cout << ustr[0] << std::endl;		// C
		std::cout << ustr << std::endl;		// C++ undo string using proxy

		ustr[0] = 'c';
		ustr[8] = '_';
		ustr[4] = 'U';
		ustr[9] = 'S';

		std::cout << ustr << std::endl;		// c++ Undo_String using proxy
		ustr.undo();
		ustr.undo();
		std::cout << ustr << std::endl;		// c++ undo_string using proxy
		ustr.undo();
		ustr.undo();
		std::cout << ustr << std::endl;		// C++ undo string using proxy

		ustr.undo();						// this should cause an exception
	}
	catch (std::exception& exception) {
		std::cout << exception.what();		// Nothing to undo!
	}
}