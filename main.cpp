#include "intrusive.h"
#include "util.h"

#include "boost/json.hpp"
//here so is just easier to click and go inside
#include "boost/json/detail/value_to.hpp"

#include <boost/describe.hpp>
#include <boost/mp11.hpp>
#include <boost/mp11/algorithm.hpp>

#include <format>
#include <iostream>
#include <vector>

using namespace std;
namespace bj = boost::json;

struct Image {
	std::string path;
	std::size_t size;
};

struct Like {
	std::string author;
	uint        timestamp = 0;
};

struct Comment {
	std::string       text;
	uint              timestamp = 0;
	std::vector<Like> likes;
};

struct Post {
	Image                image;
	std::vector<Comment> comment;
};

BOOST_DESCRIBE_STRUCT(Image, (), (path, size));
BOOST_DESCRIBE_STRUCT(Like, (), (author, timestamp));
BOOST_DESCRIBE_STRUCT(Comment, (), (text, timestamp, likes));
BOOST_DESCRIBE_STRUCT(Post, (), (image, comment));

void extra1(std::vector<Post> posts) {
	//case 1 very simple
	auto test = bj::parse(R"([{"extra":"extra"}])");

	auto sub = BOOST_JSON_INTRUSIVE::subtractJson(test, bj::value_from(posts));
	if (sub) {
		pretty_print(cout, sub.value());
	}
}
void extra2(std::string raw, std::vector<Post> posts) {
	auto test = bj::parse(raw);
	test.set_at_pointer("/0/comment/0/likes/1/extra", "extra");
	auto sub = BOOST_JSON_INTRUSIVE::subtractJson(test, bj::value_from(posts));
	if (sub) {
		pretty_print(cout, sub.value());
	}
}

void try_convert(bj::value test) {
	//I have to think at something better...
	BOOST_JSON_INTRUSIVE::reset();

	auto l1_try = bj::try_value_to<std::vector<Post>>(test);
	if (l1_try.has_error()) {
		cout << std::format(R"(

------
default msg : {}
expanded msg: {})",
		                    l1_try.error().message(), BOOST_JSON_INTRUSIVE::getMessage());
	}
}

void missing2(std::string raw) {
	auto test = bj::parse(raw);
	//next PR will be for delete_at_pointer
	test.as_array()[0].at("comment").as_array()[0].as_object().erase("timestamp");
	try_convert(test);
}

void missing3(std::string raw) {
	auto test = bj::parse(raw);
	test.as_array()[0].at("comment").as_array()[0].at("likes").as_array()[1].as_object().erase("author");
	try_convert(test);
}

void alter1(std::string raw) {
	auto test = bj::parse(raw);
	test.as_array()[0].at("comment").as_array()[0].as_object().erase("timestamp");
	test.set_at_pointer("/0/comment/0/timestamp", "string");
	try_convert(test);
}

int main() {
	//For the reference one, in the array we need only one sample, here we have 2 just to test
	std::string raw  = R"(
[
    {
        "image" : {
            "path" : "https://i.natgeofe.com/n/4f5aaece-3300-41a4-b2a8-ed2708a0a27c/domestic-dog_thumb_square.jpg?wp=1&w=170&h=170",
            "size" : 9100
        },
        "comment" : [
            {
                "text" : "this is cool",
                "timestamp" : 123456,
                "likes" : [
                    {
                        "author" : "Coco",
                        "timestamp" : 123
                    },
                    {
                        "author" : "Izzy",
                        "timestamp" : 456
                    }
                ]
            }
        ]
    }
]

)";
	auto        json = bj::parse(raw);
	//TODO we should populate in auto, inserting a value for the array
	std::vector<Post> posts = bj::value_to<std::vector<Post>>(json);

	//detect the extra element
	//extra1(posts);
	//extra2(raw, posts);

	//detect missing non optional element
	missing2(raw);
	missing3(raw);

	alter1(raw);

	cout << endl;

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
