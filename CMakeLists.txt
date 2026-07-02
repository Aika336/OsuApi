cmake_minimum_required(VERSION 3.11)
project(OsuApi CXX)

set(CMAKE_CXX_STANDARD 20)

add_library(ProcessManager
    include/ProcessManager/ProvideProcesses.h
    include/ProcessManager/Process_info.h
    include/ProcessManager/OpenProcess.h
    include/ProcessManager/HandlerRaii.h

    src/ProcessManager/ProvideProcesses.cpp
    src/ProcessManager/OpenProcess.cpp
)

add_library(MemoryScanning 
    include/MemoryScanning/PatternMatcher.h
    include/MemoryScanning/ProcessMemoryScanner.h
    src/MemoryScanning/ProcessMemoryScanner.cpp
)

add_library(Memory
    include/Memory/Memory.h
    src/Memory/Memory.cpp
)

target_include_directories(ProcessManager PUBLIC include/ProcessManager)
target_include_directories(MemoryScanning PUBLIC include/MemoryScanning)
target_include_directories(Memory PUBLIC include/Memory)

add_executable(OsuApi
    main.cpp
)

target_link_libraries(OsuApi PRIVATE MemoryScanning)
target_link_libraries(MemoryScanning PRIVATE ProcessManager)
target_link_libraries(MemoryScanning PRIVATE Memory)