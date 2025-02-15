cmake_minimum_required(VERSION 3.16)

project(BoostJsonIntrusive LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("boostjson/include")

add_executable(BoostJsonIntrusive main.cpp
	src.cpp
	intrusive.h intrusive.cpp
	util.h util.cpp)

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
