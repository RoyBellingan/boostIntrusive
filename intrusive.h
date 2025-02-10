#ifndef BOOST_JSON_INTRUSIVE_H
#define BOOST_JSON_INTRUSIVE_H

#include </home/roy/Scaricati/boost_1_87_0/boost/json/fwd.hpp>
#include <boost/system/error_code.hpp>
#include <boost/version.hpp>
#include <format>
#include <string>
#include <vector>

struct BOOST_JSON_INTRUSIVE {
	struct Key {
		const char* key = nullptr;
		std::size_t pos = -1;
		Key(const char* k_);
		Key(std::size_t p_);
	};
	using PathVec = std::vector<Key>;

	BOOST_JSON_INTRUSIVE();
	static void               push(const Key key);
	static void               inc();
	static void               pop();
	static std::string        composePath();
	//static std::string        composeMessage(boost::json::value* original_, boost::json::value target);
	static inline std::string message;
	static inline PathVec     path;

	static inline boost::system::error_code error;
	static inline boost::json::value*       original = nullptr;
};

#ifndef BOOST_JSON_INTRUSIVE_INDEX_INC
#define BOOST_JSON_INTRUSIVE_INDEX_INC (BOOST_JSON_INTRUSIVE::inc());
#endif

#ifndef BOOST_JSON_INTRUSIVE_PATH_PUSH
#define BOOST_JSON_INTRUSIVE_PATH_PUSH(x) (BOOST_JSON_INTRUSIVE::push(x));
#endif

#ifndef BOOST_JSON_INTRUSIVE_PATH_POP
#define BOOST_JSON_INTRUSIVE_PATH_POP (BOOST_JSON_INTRUSIVE::pop());
#endif

#ifndef BOOST_JSON_INTRUSIVE_MESSAGE
#define BOOST_JSON_INTRUSIVE_MESSAGE(x) (BOOST_JSON_INTRUSIVE::message = (x));
#endif

boost::json::object subtractJson(const boost::json::object& first, const boost::json::object& second, std::string path = {});

#include </home/roy/Scaricati/boost_1_87_0/boost/json/detail/value_to.hpp>
#endif // BOOST_JSON_INTRUSIVE_H
