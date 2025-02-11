#include "intrusive.h"
#include "util.h"
#include "json/include/boost/json.hpp"

#include <format>
#include <iostream>

using namespace std;
namespace bj = boost::json;

bool empty(const boost::json::value& val) {
	if (val.is_array() && val.as_array().empty()) {
		return true;
	}
	if (val.is_object() && val.as_object().empty()) {
		return true;
	}
	return false;
}

std::string join(const BOOST_JSON_INTRUSIVE::PathVec& path) {
	std::string res;
	res.reserve(path.size() * 16);

	for (const auto& p : path) {
		res += "/";
		if (p.key != nullptr) {
			res += p.key;
		} else {
			//if an array convert to string
			res += std::to_string(p.pos);
		}
	}

	return res;
}

std::optional<boost::json::value> BOOST_JSON_INTRUSIVE::subtractJson(const boost::json::object& first, const boost::json::object& second, PathVec& path) {
	boost::json::object result;

	for (const auto& item : first) {
		const auto& key   = item.key();
		const auto& value = item.value();

		// Check if the key exists in the second object
		if (second.contains(key)) {
			auto k = std::string_view(key);
			path.push_back(k.data());

			const auto& second_value = second.at(key);

			// If both values are objects, recursively subtract
			if (value.is_object() && second_value.is_object()) {

				auto diff = subtractJson(value, second_value, path);
				if (diff) {
					result[key] = diff.value();
				}
			}
			// If both values are arrays, subtract elements
			else if (value.is_array() && second_value.is_array()) {
				auto diff = subtractJson(value.as_array(), second_value.as_array(), path);
				if (diff) {
					result[key] = diff.value();
				}
			} else {
				auto diff = subtractJson(value, second_value, path);
				if (diff) {
					result[key] = diff.value();
				}
			}

			path.pop_back();
		} else {
			// If the key is not in the second object, keep it in the result
			result[key] = value;
		}
	}
	if (result.empty()) {
		return {};
	}
	return result;
}

std::optional<boost::json::value> BOOST_JSON_INTRUSIVE::subtractJson(const boost::json::array& first, const boost::json::array& second, PathVec& path) {
	boost::json::array temp;

	path.push_back(-1);
	for (auto i = 0; i < first.size(); i++) {
		auto f_elem = first[i];
		//WE always check again the first item, please do not have different types in an array!
		auto s_elem = second[0];
		path.back() = i;
		auto diff   = subtractJson(f_elem, s_elem, path);
		if (diff) {
			temp.push_back(diff.value());
		} else {
			//we insert in any case to keep the position
			temp.push_back({});
		}
	}
	path.pop_back();
	//only if all element are empty we do not propagate
	{
		//TODO remove null element from the end, we need only until one is needed
		bool allEmpty = true;
		for (auto& el : temp) {
			if (!el.is_null()) {
				allEmpty = false;
			}
		}
		if (allEmpty) {
			return {};
		} else {
			return temp;
		}
	}
}

std::optional<boost::json::value> BOOST_JSON_INTRUSIVE::subtractJson(const boost::json::value& first, const boost::json::value& second) {
	PathVec path;
	return subtractJson(first, second, path);
}

std::optional<boost::json::value> BOOST_JSON_INTRUSIVE::subtractJson(const boost::json::value& first, const boost::json::value& second, PathVec& path) {
	if (first.is_object()) {
		if (!second.is_object()) {
			//type mismatch return all
			return first;
		}
		return subtractJson(first.as_object(), second.as_object(), path);
	} else if (first.is_array()) {
		if (!second.is_array()) {
			//type mismatch return all
			return first;
		}

		return subtractJson(first.as_array(), second.as_array(), path);
	} // For other types, remove if TYPE matches
	else if (first.kind() == second.kind()) {
		//just do nothing
		return {};
	}
	// For integer types, check if they are convertible, because if the value is 0 in the config, is considered an int64, even if the original type was uint64
	auto k1 = first.kind();
	auto k2 = second.kind();

	if (k1 == boost::json::kind::uint64 && k2 == boost::json::kind::int64) {
		return {};
	}
	if (k1 == boost::json::kind::int64 && k2 == boost::json::kind::uint64) {
		return {};
	}

	auto x = to_string(k1);
	auto y = to_string(k2);
	throw std::runtime_error(
	    std::format("Types do not match in path {}\nFound {} expected {}",
	                join(path), std::string_view(x), std::string_view(y)));
}

std::string BOOST_JSON_INTRUSIVE::composePath() {
	return join(path);
}

BOOST_JSON_INTRUSIVE::BOOST_JSON_INTRUSIVE() {
	BOOST_JSON_INTRUSIVE::path.reserve(32);
}

string BOOST_JSON_INTRUSIVE::getMessage() {
	if (message.empty()) {
		return "Last json pointer was " + join(path);
	}
	return message;
}

void BOOST_JSON_INTRUSIVE::reset() {
	path.clear();
	message.clear();
}

void BOOST_JSON_INTRUSIVE::push(const Key key) {
	BOOST_JSON_INTRUSIVE::path.push_back(std::move(key));
}

void BOOST_JSON_INTRUSIVE::inc() {
	if (!path.empty()) { //is this check even needed ?
		path.back().pos++;
	}
}

void BOOST_JSON_INTRUSIVE::pop() {
	BOOST_JSON_INTRUSIVE::path.pop_back();
}

BOOST_JSON_INTRUSIVE::Key::Key(const char* k_) {
	key = k_;
}

BOOST_JSON_INTRUSIVE::Key::Key(int p_) {
	pos = p_;
}
