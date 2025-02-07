cmake_minimum_required(VERSION 3.16)

project(BoostJsonIntrusive LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(BoostJsonIntrusive main.cpp
	src.cpp
	intrusive.h intrusive.cpp)

include(GNUInstallDirs)
install(TARGETS BoostJsonIntrusive
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

target_link_libraries(BoostJsonIntrusive
	 PUBLIC
		  fmt
)
