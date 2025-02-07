#include "intrusive.h"

#include </home/roy/Scaricati/boost_1_87_0/boost/json.hpp>
#include <boost/describe.hpp>
//#include <boost/describe/enum.hpp>
#include <boost/mp11.hpp>
#include <boost/mp11/algorithm.hpp>

#include <format>
#include <iostream>
#include <vector>

using namespace std;
namespace bj = boost::json;

struct Bau {
	int y;
};
    
struct Miao {
	int              x;
	std::vector<Bau> bau;
};

BOOST_DESCRIBE_STRUCT(Bau, (), (y));
BOOST_DESCRIBE_STRUCT(Miao, (), (x, bau));

int main() {
    
	Miao m;
	m.x = 1;
	m.bau.push_back({2});

	auto json = bj::value_from(m);
    cout << bj::serialize(json) << "\n";
    
	using Md = boost::describe::describe_members<Miao, boost::describe::mod_any_access>;
	//inject something unexpected
	auto& obj   = json.as_object();
	obj["ciao"] = "ciao";
	obj["Miao"] = "";
    
    
    cout << bj::serialize(json) << "\n";

	auto t2 = bj::try_value_to<Miao>(json);
	if (t2.has_error()) {
		auto ref = bj::value_from(m);
		auto msg = BOOST_JSON_INTRUSIVE::composeMessage(&json, ref);
		cout << format("{} in {} ", t2.error().message(), msg);
        exit(1);
	}

	
	return 0;
}

//Todo enum
