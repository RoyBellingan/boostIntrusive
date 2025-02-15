#ifndef UTIL_H
#define UTIL_H

#include "boost/json/fwd.hpp"
void pretty_print(std::ostream& os, boost::json::value const& jv, std::string* indent = nullptr);

#endif // UTIL_H
