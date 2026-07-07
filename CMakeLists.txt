cmake_minimum_required(VERSION 3.11)
project(OsuApi CXX)

set(CMAKE_CXX_STANDARD 20)

add_library(ProcessManager
    include/ProcessManager/ProvideProcesses.h
    include/ProcessManager/Process_info.h
    include/ProcessManager/OpenProcess.h
    include/ProcessManager/HandlerRaii.h
    # src
    src/ProcessManager/ProvideProcesses.cpp
    src/ProcessManager/OpenProcess.cpp
    src/ProcessManager/HandlerRaii.cpp
)

add_library(MemoryScanning 
    include/MemoryScanning/PatternMatcher.h
    include/MemoryScanning/ProcessMemoryScanner.h
    # src
    src/MemoryScanning/ProcessMemoryScanner.cpp
    src/MemoryScanning/PatternMatcher.cpp
)

add_library(Memory
    include/Memory/Memory.h
    include/Memory/DotNetString.h
    # src
    src/Memory/Memory.cpp
    src/Memory/DotNetString.cpp
)

add_library(ProvideOsuInfo
    include/ProvideOsuInfo/ProviderOsuInfo.h
    include/ProvideOsuInfo/OsuHandler.h
    # src
    src/ProvideOsuInfo/ProviderOsuInfo.cpp
    src/ProvideOsuInfo/OsuHandler.cpp
)


target_include_directories(ProcessManager PUBLIC include/ProcessManager)
target_include_directories(MemoryScanning PUBLIC include/MemoryScanning)
target_include_directories(Memory PUBLIC include/Memory)
target_include_directories(ProvideOsuInfo PUBLIC 
    include/ProvideOsuInfo
    include/ProvideOsuInfo/OsuOffsets
)

add_executable(OsuApi
    main.cpp
)

target_link_libraries(OsuApi PRIVATE ProvideOsuInfo)
target_link_libraries(MemoryScanning PUBLIC ProcessManager)
target_link_libraries(ProvideOsuInfo PUBLIC Memory ProcessManager MemoryScanning)