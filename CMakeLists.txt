cmake_minimum_required(VERSION 3.11)
project(OsuApi CXX)

set(CMAKE_CXX_STANDARD 20)

add_library(SignatureFinder
    include/SignatureFinder/OpenProcess.h
    include/SignatureFinder/Process_info.h
    include/SignatureFinder/ProvideProcesses.h
    include/SignatureFinder/SignatureFinder.h
    src/SignatureFinder/ProvideProcesses.cpp
    src/SignatureFinder/OpenProcess.cpp
    src/SignatureFinder/SignatureFinder.cpp
)

target_include_directories(SignatureFinder PUBLIC include/SignatureFinder)

add_executable(OsuApi
    main.cpp
)


target_link_libraries(OsuApi PRIVATE SignatureFinder)