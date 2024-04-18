#include <tuple>
#include <string>
#include <format>
#include <variant>
#include <functional>
#include "parser.hpp"

Parser<char> pchar(char characterToMatch) {
	auto inner = [characterToMatch](string str) -> ParserResult<Success<char>> {
		if (str.empty()) {
			return string("Got Empty String");
		}

		if (characterToMatch == str[0]) {
			return std::make_tuple(str[0], str.substr(1)); // 'A', "ABC" -> ('A', "BC")
		}

		return std::format("Excpeted {}. Got {}", characterToMatch, str[0]);
		};
	return Parser<char> {
		.inner = inner
	};
}
