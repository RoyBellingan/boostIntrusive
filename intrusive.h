#ifndef BOOST_JSON_INTRUSIVE_H
#define BOOST_JSON_INTRUSIVE_H

#include <boost/json/fwd.hpp>
#include <string>
#include <vector>
#include <optional>

struct BOOST_JSON_INTRUSIVE {
	struct Key {
		const char* key = nullptr;
		int         pos = -1;
		Key(const char* k_);
		Key(int p_);
	};
	using PathVec = std::vector<Key>;

	static inline std::string message;
	static inline PathVec     path;

	BOOST_JSON_INTRUSIVE();
	static std::string getMessage();
	static void        reset();
	static void        push(const Key key);
	static void        inc();
	static void        pop();
	static std::string composePath();

	static std::optional<boost::json::value> subtractJson(const boost::json::value& first, const boost::json::value& second, PathVec& path);
	static std::optional<boost::json::value> subtractJson(const boost::json::object& first, const boost::json::object& second, PathVec& path);
	static std::optional<boost::json::value> subtractJson(const boost::json::array& first, const boost::json::array& second, PathVec& path);
	static std::optional<boost::json::value> subtractJson(const boost::json::value& first, const boost::json::value& second);
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

#include <boost/json/detail/value_to.hpp>
#endif // BOOST_JSON_INTRUSIVE_H
