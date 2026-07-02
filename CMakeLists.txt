cmake_minimum_required(VERSION 3.11)
project(OsuApi CXX)

set(CMAKE_CXX_STANDARD 20)

add_library(SignatureFinder
    include/SignatureFinder/OpenProcess.h
    include/SignatureFinder/Process_info.h
    include/SignatureFinder/ProvideProcesses.h
    include/SignatureFinder/SignatureFinder.h
    include/SignatureFinder/HandlerRaii.h
    src/SignatureFinder/ProvideProcesses.cpp
    src/SignatureFinder/OpenProcess.cpp
    src/SignatureFinder/SignatureFinder.cpp
)

add_library(Memory
    include/Memory/Memory.h
    src/Memory/Memory.cpp
)

target_include_directories(SignatureFinder PUBLIC include/SignatureFinder)
target_include_directories(Memory PUBLIC include/Memory)

add_executable(OsuApi
    main.cpp
)

target_link_libraries(OsuApi PRIVATE SignatureFinder)
target_link_libraries(SignatureFinder PUBLIC Memory)