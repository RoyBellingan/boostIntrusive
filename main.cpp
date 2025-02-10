#include "intrusive.h"
#include "util.h"

#include </home/roy/Scaricati/boost_1_87_0/boost/json.hpp>
#include </home/roy/Scaricati/boost_1_87_0/boost/json/detail/value_to.hpp>

#include <boost/describe.hpp>
//#include <boost/describe/enum.hpp>
#include <boost/mp11.hpp>
#include <boost/mp11/algorithm.hpp>

#include <format>
#include <iostream>
#include <vector>

using namespace std;
namespace bj = boost::json;

class Ip_address {
      public:
	Ip_address() = default;

	Ip_address(
	    unsigned char oct1,
	    unsigned char oct2,
	    unsigned char oct3,
	    unsigned char oct4)
	    : octets_{oct1, oct2, oct3, oct4} {
	}

	const unsigned char*
	begin() const {
		return octets_.data();
	}

	const unsigned char*
	end() const {
		return octets_.data() + octets_.size();
	}

      private:
	std::array<unsigned char, 4> octets_ = {0, 0, 0, 0};
};

void tag_invoke(const bj::value_from_tag&, bj::value& jv, Ip_address const& addr) {
	// Store the IP address as a 4-element array of octets
	const unsigned char* b = addr.begin();
	jv                     = {b[0], b[1], b[2], b[3]};
}

Ip_address
tag_invoke(const bj::value_to_tag<Ip_address>&, bj::value const& jv) {
	const auto& arr = jv.as_array();
	// if (arr.size() != 4) {
	//        //
	// }
	return Ip_address(
	    arr.at(0).to_number<unsigned char>(),
	    arr.at(1).to_number<unsigned char>(),
	    arr.at(2).to_number<unsigned char>(),
	    arr.at(3).to_number<unsigned char>());
}

struct Image {
	std::string path;
	std::size_t size;
};

struct Comment {
	std::string author;
	std::string text;
};

struct Post {
	Image                    image;
	std::string              title;
	std::vector<std::string> likes;
	std::vector<Comment>     comment;
	int                      shares = 0;
};

BOOST_DESCRIBE_STRUCT(Post, (), (title, likes, comment, shares));
BOOST_DESCRIBE_STRUCT(Comment, (), (author, text));
BOOST_DESCRIBE_STRUCT(Image, (), (path, size));

int main() {

	std::vector<Post> post;

	Comment c;
	c.author = "Nancy";
	c.text   = "this is cool";

	Post p;
	p.comment.push_back(c);
	p.image.path = "https://i.natgeofe.com/n/4f5aaece-3300-41a4-b2a8-ed2708a0a27c/domestic-dog_thumb_square.jpg?wp=1&w=170&h=170";
	p.image.size = 9100;

	p.likes  = {"Nancy", "Roy", "Coco"};
	p.shares = 2;

	p.title = "a random dog";

	post.push_back(p);

	auto json = bj::value_from(post);

	pretty_print(cout, json);

	// { //This will detect the extra element
	// 	auto check = json;
	// 	check.set_at_pointer("/l2/extra", "extra");
	// 	auto sub = subtractJson(json.as_object(), bj::value_from(post).as_object());
	// 	if (!sub.empty()) {
	// 		pretty_print(cout, sub);
	// 	}
	// }

	{
		//Remove a value and get the error position

		auto check = json;
		//next PR will be for delete_at_pointer
		check.as_array()[0].at("comment").as_array()[0].as_object().erase("text");

		pretty_print(cout, check);

		auto l1_try = bj::try_value_to<std::vector<Post>>(check);
		if (l1_try.has_error()) {
			cout << format("{}\n{} ", l1_try.error().message(), BOOST_JSON_INTRUSIVE::message);
			exit(1);
		}
	}

	//
	//    auto original = bj::value_from(m);

	// // //using Md = boost::describe::describe_members<Miao, boost::describe::mod_any_access>;
	// //inject something unexpected
	// auto& obj   = json.as_object();
	// obj["ciao"] = "ciao";
	// obj["bau"].as_array().push_back(1);

	//

	// cout << bj::serialize(obj) << "\n";

	// //cout << t2.value().x;

	// cout << bj::serialize(res);
	// ip_address addr = {127, 0, 0, 12};
	// auto jv   = bj::value_from(addr);
	// assert(serialize(jv) == R"([127,0,0,12])");

	// Convert back to IP address
	// auto addr2 = bj::try_value_to<ip_address>(json);
	// assert(std::equal(
	//     addr.begin(), addr.end(), addr2.begin()));

	return 0;
}

#include <dlfcn.h>
using cxa_throw_type = void(void*, std::type_info*, void (*)(void*));
//now take the address of the REAL __cxa_throw
static cxa_throw_type* original_cxa_throw = reinterpret_cast<cxa_throw_type*>(dlsym(RTLD_NEXT, "__cxa_throw"));

extern "C" {
//And NOW override it

#if defined(__clang__)
void __attribute__((__noreturn__)) __cxa_throw(
    void*           thrown_exception,
    std::type_info* pvtinfo,
    void (*dest)(void*)) {
#else
void __attribute__((__noreturn__)) __cxa_throw(
    void* thrown_exception,
    void* pvtinfo,
    void (*dest)(void*)) {
#endif

	//this will pass tru the exception to the original handler so the program will not catch fire after an exception is thrown
	original_cxa_throw(thrown_exception, (std::type_info*)pvtinfo, dest);

	//we should never reach this point, but the compiler do not recognize the original_cxa_throw above so we put another one here
	throw std::runtime_error("This should never happen!");
}
}

//this is to intercept easily excpetion

//Todo enum
