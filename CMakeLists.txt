cmake_minimum_required(VERSION 3.11)
project(OsuApi CXX)

set(CMAKE_CXX_STANDARD 20)

add_library(NtDll STATIC
    NtDll/include/ntdll.h
    NtDll/include/domain.h
    #
    NtDll/src/ntdll.cpp
    NtDll/src/domain.cpp
)

add_library(ProcessManager STATIC
    include/ProcessManager/ProcessProvider.h
    include/ProcessManager/ProcessInfo.h
    include/ProcessManager/ProcessOpener.h
    include/ProcessManager/HandleRaii.h
    # src
    src/ProcessManager/ProcessProvider.cpp
    src/ProcessManager/ProcessOpener.cpp
    src/ProcessManager/HandleRaii.cpp
)

add_library(MemoryScanning STATIC
    include/MemoryScanning/PatternMatcher.h
    include/MemoryScanning/ProcessMemoryScanner.h
    # src
    src/MemoryScanning/ProcessMemoryScanner.cpp
    src/MemoryScanning/PatternMatcher.cpp
)

add_library(Memory STATIC
    include/Memory/Memory.h
    include/Memory/DotNetString.h
    # src
    src/Memory/Memory.cpp
    src/Memory/DotNetString.cpp
)

add_library(ProvideOsuInfo STATIC
    include/ProvideOsuInfo/ProviderOsuInfo.h
    include/ProvideOsuInfo/OsuHandler.h
    include/ProvideOsuInfo/OsuSignatureInfo.h
    # src
    src/ProvideOsuInfo/ProviderOsuInfo.cpp
    src/ProvideOsuInfo/OsuHandler.cpp
)

add_library(Logger STATIC
    include/Logger/Logger.h
    #
    src/Logger/Logger.cpp
)

target_include_directories(ProcessManager PUBLIC 
    include/ProcessManager
)
target_include_directories( MemoryScanning PUBLIC 
    include/MemoryScanning)
target_include_directories(Memory 
    PUBLIC include/Memory
)
target_include_directories(Logger PUBLIC 
    include/Logger
)
target_include_directories(ProvideOsuInfo PUBLIC 
    include/ProvideOsuInfo
    include/ProvideOsuInfo/OsuOffsets
)
target_include_directories(NtDll PUBLIC
     NtDll/include
)

foreach(target ProcessManager MemoryScanning Memory ProvideOsuInfo NtDll)
    target_link_libraries(${target} PUBLIC Logger)
endforeach()

add_executable(OsuApi
    main.cpp
)

target_link_libraries(OsuApi PRIVATE
    ProvideOsuInfo
)
target_link_libraries(MemoryScanning PUBLIC 
    ProcessManager
    Memory
)
target_link_libraries(ProvideOsuInfo PUBLIC
    Memory 
    MemoryScanning
)
target_link_libraries(Memory PUBLIC
    NtDll
)