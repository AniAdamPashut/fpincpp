#include <iostream>
#include <variant>
#include <tuple>

#include "parser.hpp"

int main() {

	std::string input("ABC");
	Parser<char> parseA = pchar('A');
	Parser<char> parseB = pchar('B');
	auto parseAthenB = andThen(orElse(parseA, parseB), pchar('C'));
	auto res = run(parseAthenB, input);
	std::cout << res.index() << "\n";
 	std::cout << "What a time to be alive!\n";
}