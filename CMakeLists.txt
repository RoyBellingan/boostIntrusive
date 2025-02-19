cmake_minimum_required(VERSION 3.16)

project(BoostJsonIntrusive LANGUAGES CXX)


# Set Clang as the C++ compiler
#set(CMAKE_CXX_COMPILER "clang++")

message(STATUS "Using compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Compiler path: ${CMAKE_CXX_COMPILER}")
message(STATUS "Compiler version: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Compiler is Clang: ${CMAKE_CXX_COMPILER_ID} STREQUAL \"Clang\"")
message(STATUS "Compiler is GNU: ${CMAKE_CXX_COMPILER_ID} STREQUAL \"GNU\"")


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories("boostjson/include")

add_executable(BoostJsonIntrusive main.cpp
	src.cpp
	intrusive.h intrusive.cpp
	util.h util.cpp)

# Common warnings for both compilers
set(COMMON_WARNINGS
    -Wall
    -Wextra
    -Wpedantic
    -Werror
    -Wimplicit-fallthrough
    #-Wcast-align
    -Wcast-qual
    #-Wdouble-promotion
    -Wformat=2
    -Wformat-security
    -Wformat-signedness
    -Wnull-dereference
    -Woverloaded-virtual
    -Wunused
    -Wno-unused-parameter
    -Wpointer-arith
    -Wuninitialized
    -Wunknown-pragmas
    -Wstack-protector
    -Wunreachable-code
    -Wvarargs
    -Wvla
    -Wwrite-strings
    -fstack-protector-strong
    -fno-common
    -Wno-sign-compare
)

# GCC-specific warnings
set(GCC_WARNINGS
    -Wvector-operation-performance
    -Wctor-dtor-privacy
    -Wnoexcept
    -Wstrict-null-sentinel
    #-Wsign-promo
    #-Weffc++
)

# Set compiler-specific options
if(CMAKE_CXX_COMPILER MATCHES ".*clang\\+\\+.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*[Cc]lang.*")
    message(STATUS "Configuring for Clang compiler")
    target_compile_options(BoostJsonIntrusive
        PRIVATE
            ${COMMON_WARNINGS}
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Configuring for GCC compiler")
    target_compile_options(BoostJsonIntrusive
        PRIVATE
            ${COMMON_WARNINGS}
            ${GCC_WARNINGS}
    )
endif()

# Enable sanitizers in debug mode
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(BoostJsonIntrusive
            PRIVATE
                -fsanitize=undefined
                -fsanitize=float-divide-by-zero
                -fsanitize=float-cast-overflow
                -fno-sanitize-recover=all
        )
        target_link_options(BoostJsonIntrusive
            PRIVATE
                -fsanitize=undefined
                -fsanitize=float-divide-by-zero
                -fsanitize=float-cast-overflow
        )
    endif()
endif()

include(GNUInstallDirs)
install(TARGETS BoostJsonIntrusive
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(BoostJsonIntrusive
        PUBLIC
            -fsanitize=address
            -fno-omit-frame-pointer
    )
    target_link_options(BoostJsonIntrusive
        PRIVATE
            -fsanitize=address
    )
    target_link_libraries(BoostJsonIntrusive
        PRIVATE
            asan
    )
endif()

target_link_libraries(BoostJsonIntrusive
    PUBLIC
        fmt
)
