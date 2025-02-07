#ifndef BOOST_JSON_INTRUSIVE_H
#define BOOST_JSON_INTRUSIVE_H

#include <boost/json/fwd.hpp>
#include <boost/system/error_code.hpp>
#include <boost/version.hpp>
#include <format>
#include <string>
#include <vector>

struct BOOST_JSON_INTRUSIVE {
    BOOST_JSON_INTRUSIVE();
    static void                             push(const char* str);
    static void                             pop();
    static std::string                      composePath();
    static std::string                      composeMessage(boost::json::value* original_, boost::json::value target);
    static inline std::string               message;
    static inline std::vector<const char*>  path;
    static inline std::string_view          key;
    static inline boost::system::error_code error;
    static inline boost::json::value*       original = nullptr;
};

#ifndef BOOST_JSON_INTRUSIVE_PATH_PUSH
#define BOOST_JSON_INTRUSIVE_PATH_PUSH(x) (BOOST_JSON_INTRUSIVE::push(x));
#endif

#ifndef BOOST_JSON_INTRUSIVE_PATH_POP
#define BOOST_JSON_INTRUSIVE_PATH_POP (BJIntrusive::pop());
#endif

#ifndef BOOST_JSON_INTRUSIVE_MESSAGE
#define BOOST_JSON_INTRUSIVE_MESSAGE(x) (BOOST_JSON_INTRUSIVE::message = (x));
#endif

#include </home/roy/Scaricati/boost_1_87_0/boost/json/detail/value_to.hpp>
#endif // BOOST_JSON_INTRUSIVE_H
