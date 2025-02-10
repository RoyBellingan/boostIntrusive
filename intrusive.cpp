#include "intrusive.h"
#include </home/roy/Scaricati/boost_1_87_0/boost/json.hpp>

#include <format>

using namespace std;
namespace bj = boost::json;

boost::json::object subtractJson(const boost::json::object& first, const boost::json::object& second, std::string path) {
	boost::json::object result;

	// Iterate through the first object
	for (const auto& item : first) {
		const auto& key   = item.key();
		const auto& value = item.value();

		// Check if the key exists in the second object
		if (second.contains(key)) {
			const auto& second_value = second.at(key);

			// If both values are objects, recursively subtract
			if (value.is_object() && second_value.is_object()) {
				std::string newPath = std::format("{}/{}", path, std::string_view(key));
				auto diff          = subtractJson(value.as_object(), second_value.as_object(), newPath);
				if(!diff.empty()){
					result[key] = diff;
				}
			}
			// If both values are arrays, subtract elements
			else if (value.is_array() && second_value.is_array()) {
				boost::json::array diff_array;
				const auto&        first_array  = value.as_array();
				const auto&        second_array = second_value.as_array();

				for (const auto& elem : first_array) {
					if (std::find(second_array.begin(), second_array.end(), elem) == second_array.end()) {
						diff_array.push_back(elem);
					}
				}
				if (!diff_array.empty()) {
					result[key] = diff_array;
				}

			}
			// For other types, remove if TYPE matches
			else if (value.kind() == second_value.kind()) {

			} else {
				// For integer types, check if they are convertible, because if the value is 0 in the config, is considered an int64, even if the original type was uint64

				if (second_value.kind() == boost::json::kind::uint64) {
					std::error_code ec;
					value.to_number<uint64_t>(ec);
					if (!ec) {
						continue;
					}
				}
				auto x = to_string(value.kind());
				auto y = to_string(second_value.kind());
				throw std::runtime_error(
				    std::format("Types do not match for key: {} in path {}\nFound {} expected {}",
				                std::string_view(key), path, "", ""));
			}
		} else {
			// If the key is not in the second object, keep it in the result
			result[key] = value;
		}
	}

	return result;
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

std::string BOOST_JSON_INTRUSIVE::composePath() {
	return join(path);
}

//TODO add a struct for options, one of those is if to add the original JSON in the error message
// std::string BOOST_JSON_INTRUSIVE::composeMessage(bj::value* original_, bj::value target) {
	
// 	auto path = composePath();
// 	if (error == bj::error::size_mismatch) {
// 		//Extra element is
// 		std::error_code ec;
// 		auto            ptrO = original->find_pointer(path, ec);
// 		auto            ptrT = target.find_pointer(path, ec);

// 		if (ptrO && ptrT) {
// 			auto res = bj::serialize(subtractJson(ptrO->as_object(), ptrT->as_object(), path));
// 			return std::format("Found extra element in path {}\n{}", composePath(), res);
// 		} else {
// 			throw std::format("Impossible to find the JSON path {}", path);
// 		}
// 		return std::format("{}: {}", composePath(), message);
// 	} else {
// 		message.append(" in path " + path);
// 	}
// 	return message;
// }

BOOST_JSON_INTRUSIVE::BOOST_JSON_INTRUSIVE() {
	BOOST_JSON_INTRUSIVE::path.reserve(32);
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

BOOST_JSON_INTRUSIVE::Key::Key(std::size_t p_) {
	pos = p_;
}
