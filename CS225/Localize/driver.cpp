#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <windows.h>

#include "LocalizeText.h"
#include "Strings.h"



void test0() {
	LocalizedText localizedText("English.dat");
	std::cout << localizedText.ToString(HI) << std::endl;
}

void test1() {
	LocalizedText localizedText("Korean.dat");

	std::cout << localizedText.ToString(HI) << std::endl;
}

void test2() {
	LocalizedText localizedText("French.dat");

	std::cout << localizedText.ToString(HI) << std::endl;
}

void test3() {
	LocalizedText localizedText("Spanish.dat");

	std::cout << localizedText.ToString(HI) << std::endl;
}

void test4() {
	LocalizedText localizedText("Italian.dat");

	std::cout << localizedText.ToString(HI) << std::endl;
}


void test5() {
	LocalizedText localizedText("English.dat");
	std::cout << localizedText.ToString(FAVORITE) << std::endl;
	std::cout << localizedText.ToString(INTERESTING) << std::endl;
	std::cout << localizedText.ToString(LEARNING) << std::endl;
	std::cout << localizedText.ToString(GAMEPROJECT) << std::endl;
}

void test6() {
	LocalizedText localizedText("Korean.dat");

	std::cout << localizedText.ToString(FAVORITE) << std::endl;
	std::cout << localizedText.ToString(INTERESTING) << std::endl;
	std::cout << localizedText.ToString(LEARNING) << std::endl;
	std::cout << localizedText.ToString(GAMEPROJECT) << std::endl;
}

void test7() {
	LocalizedText localizedText("Spanish.dat");
	
	std::cout << localizedText.ToString(FAVORITE) << std::endl;
	std::cout << localizedText.ToString(INTERESTING) << std::endl;
	std::cout << localizedText.ToString(LEARNING) << std::endl;
	std::cout << localizedText.ToString(GAMEPROJECT) << std::endl;
	std::cout << localizedText.ToString(TRANSLATION) << std::endl;
}

void test8() {
	std::cout << LocalizedText("Korean.dat").ToString(ENGLISH) << std::endl;
	std::cout << LocalizedText("English.dat").ToString(BETTERGRADE) << std::endl;
}

void test9() {
	std::cout << LocalizedText("Korean.dat").ToString(HEX) << std::endl;
}



int main(int argc, char** argv) {
#ifdef _MSC_VER
	// Set console code page to UTF-8 so console known how to interpret string data
	SetConsoleOutputCP(CP_UTF8);
#endif

	int test_num = -1;

	typedef void(*Test)(void);
	Test Tests[] = {
		test0, test1, test2, test3, test4, test5, test6, test7, test8, test9
	};
	int numTests = static_cast<int>(sizeof(Tests) / sizeof(*Tests));
	if (argc > 1) {
		test_num = std::atoi(argv[1]);
	}

	if (test_num >= 0 && test_num < numTests) {
		Tests[test_num]();
	}
	else {
		for (int i = 0; i < numTests; i++) {
			Tests[i]();
		}
	}

#ifdef _MSC_VER
	system("pause");
#endif
	return 0;
}