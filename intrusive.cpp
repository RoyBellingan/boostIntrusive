#include "intrusive.h"
#include <format>

#include <fmt/format.h>
#include <fmt/ranges.h>

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
				// std::string newPath = fmt::format("{}/{}", path, std::string_view(key));
				// result[key]         = subtractJson(value.as_object(), second_value.as_object(), newPath);
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
				result[key] = diff_array;
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
				// throw std::runtime_error(
				//     fmt::format("Types do not match for key: {} in path {}\nFound {} expected {}",
				//                 std::string_view(key), path, asSWString(value.kind()), asSWString(second_value.kind())));
			}
		} else {
			// If the key is not in the second object, keep it in the result
			result[key] = value;
		}
	}

	return result;
}

std::string BOOST_JSON_INTRUSIVE::composePath() {
	return fmt::format("/{}", fmt::join(path, "/"));
}

//TODO add a struct for options, one of those is if to add the original JSON in the error message
std::string BOOST_JSON_INTRUSIVE::composeMessage(bj::value* original_, bj::value target) {
	original  = original_;
	//auto path = composePath();
	// if (error == bj::error::size_mismatch) {
	// 	//Extra element is
	// 	std::error_code ec;
	// 	auto            ptrO = original->find_pointer(path, ec);
	// 	auto            ptrT = target.find_pointer(path, ec);

	// 	if (ptrO && ptrT) {
	// 		auto res = pretty_print(subtractJson(ptrO->as_object(), ptrT->as_object(), path));
	// 		return fmt::format("Found extra element in path {}\n{}", composePath(), res);
	// 	} else {
	// 		throw ExceptionV2(F("Impossible to find the JSON path {}", path));
	// 	}
	// 	return fmt::format("{}: {}", composePath(), message);
	// }
	return message;
}

BOOST_JSON_INTRUSIVE::BOOST_JSON_INTRUSIVE() {
	BOOST_JSON_INTRUSIVE::path.reserve(32);
}

void BOOST_JSON_INTRUSIVE::push(const char* str) {
	BOOST_JSON_INTRUSIVE::path.push_back(str);
}

void BOOST_JSON_INTRUSIVE::pop() {
	BOOST_JSON_INTRUSIVE::path.pop_back();
}
