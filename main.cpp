#include "bst.h"

#include<iostream>
#include<string>
#include<algorithm>

/**
function compares last character of strings
@param s1 the first string
@param s2 the second string
@return if s1 last char < s2 last char
*/
bool end_str(std::string s1, std::string s2) {
	return s1.back() < s2.back();
}

int main() {

	// bst using default comparison function (std::less<T>)
	binarysearch::bst<int> bst_1;

	std::cout << "randomly generated integers inserted into bst_1:" << '\n';

	int max = 100; // upper bound to generate random numbers
	for (int i = 0; i < 5; ++i) { // generate 5 random numbers

		int temp = rand() % max; // generate random number
		std::cout << temp << '\n'; // print random number

		bst_1.insert(temp); // insert random number
	}

	std::cout << '\n';

	// printing inserted randomly generated numbers
	std::cout << "elements of bst_1:" << '\n';
	for (const auto& i : bst_1) {
		std::cout << i << '\n';
	}

	std::cout << '\n';

	std::cout << "randomly generated strings inserted into bst_2:" << '\n';

	// bst using specified comparison function (end_str)
	binarysearch::bst<std::string, bool (*)(std::string, std::string)> bst_2(end_str);

	for (int i = 0; i < 5; ++i) { // generate 5 random strings

		int length = rand() % 5 + 1; // generate string length randomly
		std::string temp = ""; // string container

		for (int j = 0; j < length; ++j) { // randomly generate string

			int max = 26; // upper bound to generate random characters
			
			int r = rand() % max; // generate a random number
			char c = 'a' + r; // convert to a character from a-z
			temp.push_back(c); // add character to end of string
		}

		std::cout << temp << '\n'; // print random string
		bst_2.insert(temp); // insert random string
	}

	std::cout << '\n';

	std::cout << "copying bst_2 to bst_3." << '\n' << '\n';

	auto bst_3 = bst_2; // copy of bst created

	std::cout << "elements of bst_3:" << '\n';
	for (const auto& s : bst_3) { // print bst_3, copy of bst_2
		std::cout << s << '\n';
	}

	std::cout << '\n';

	std::cout << "moving bst_3 to bst_4." << '\n' << '\n';

	// move constructor called to create bst_4 from bst_3
	auto bst_4 = std::move(bst_3);

	std::cout << "size of bst_3 " << bst_3.size() << '\n'; // print size of bst_3
	std::cout << "size of bst_4 " << bst_4.size() << '\n'; // print size of bst_4

	// iterator to first element of bst_4
	binarysearch::bst<std::string, decltype(&end_str)>::iterator it = bst_4.begin();

	std::cout << '\n';

	std::cout << "first element of bst_4:" << *it << '\n'; // retrieve value of first element
	std::cout << "size of first element of bst_4: " << it->size() << '\n' << '\n'; // size of first element

	std::cout << "removing first element of bst_4." << '\n' << '\n';

	bst_4.erase(it); // remove first bst_4 element

	std::cout << "elements of bst_4:" << '\n';
	for (const auto& s : bst_4) { // print bst_4 after removal
		std::cout << s << '\n';
	}

	return 0;
}
