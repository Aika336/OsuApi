cmake_minimum_required(VERSION 3.11)
project(OsuApi CXX)

set(CMAKE_CXX_STANDARD 20)

add_library(Base
	src/Reader.cpp
	src/ProcessUtils.cpp
	src/Memory.cpp
	include/game_state.h
	include/Reader.h
	include/ProcessUtils.h
	include/Offsets.h
	include/Memory.h
	include/Includes.h
)

target_include_directories(Base PUBLIC
	include
)

add_executable(OsuApi
	main.cpp
)

target_link_libraries(OsuApi
	PRIVATE Base
)