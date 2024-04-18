#pragma once

#include <tuple>
#include <string>
#include <format>
#include <variant>
#include <numeric>
#include <functional>

using std::string;
using std::tuple;

template<typename T>
using Success = std::tuple<T, std::string>;

template <typename T>
using ParserResult = std::variant<T, std::string>;

template <typename T>
struct Parser {
	std::function<ParserResult<std::tuple<T, std::string>>(std::string&)> inner;
};


Parser<char> pchar(char characterToMatch);


template <typename T>
ParserResult<tuple<T, string>> run(Parser<T> p, string& input) {
	std::function<ParserResult<std::tuple<T, std::string>>(std::string&)> inner = p.inner;
	return inner(input);
}

template <typename T, typename V>
Parser<tuple<T, V>> andThen(Parser<T> p1, Parser<V> p2) {
	auto inner = [p1, p2](string input) -> ParserResult<std::tuple<std::tuple<T, V>, std::string>> {
		auto res = run(p1, input);
		if (res.index() == 1) {
			return get<1>(res);
		}

		Success<T> scc = get<Success<T>>(res);
		T value1 = get<0>(scc);
		string rest = get<1>(scc);
		auto res2 = run(p2, rest);
		if (res2.index() == 1) {
			return get<1>(res2);
		}
		Success<V> scc2 = get<Success<V>>(res2);
		V value2 = get<0>(scc2);
		string remains = get<1>(scc2);
		std::tuple<T, V> new_value = std::make_tuple(value1, value2);
		return std::make_tuple(new_value, remains);
		};

	return Parser<tuple<T, V>>{
		.inner = inner
	};
}

template <typename T>
Parser<T> orElse(Parser<T> p1, Parser<T> p2) {
	std::function<ParserResult<std::tuple<T, std::string>>(std::string&)> inner = [p1, p2](std::string& input) -> ParserResult<std::tuple<T, std::string>> {
		auto res = run(p1, input);
		if (res.index() == 0) {
			return res;
		}

		return run(p2, input);
		};

	return Parser<T> {
		.inner = inner
	};
}


Parser<char> anyOf(string input);
Parser<char> parseDigit();

template<typename T, typename V>
Parser<V> map(std::function<V(T)> f, Parser<T> p) {
	auto inner = [f, p](string input) {
		auto res = run(p, input);
		if (res.index() == 1) {
			return get<1>(res);
		}

		Success<T> scc = get<0>(res);
		T inner = get<0>(scc);
		V new_value = f(inner);
		return std::make_tuple(new_value, get<1>(res));
		};

	return Parser<V> {
		.inner = inner
	};
}
