#include <iostream>
#include <variant>
#include <tuple>

#include "parser.hpp"

int main() {

	std::string input("073ABC");
	Parser<char> p = parseDigit();
	auto res = run(p, input);
	std::cout << res.index() << "\n";
 	std::cout << "What a time to be alive!\n";
}