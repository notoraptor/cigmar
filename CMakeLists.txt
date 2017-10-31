cmake_minimum_required(VERSION 3.8)
project(cigmar)

set(CMAKE_CXX_STANDARD 11)
include_directories(${PROJECT_SOURCE_DIR})
set(SOURCE_FILES
        libraries/base64/base64.hpp
        libraries/json/json.hpp
        cigmar/classes/array_t.hpp
        cigmar/classes/ArrayList.hpp
        cigmar/classes/Char.hpp
        cigmar/classes/Exception.hpp
        cigmar/classes/HashMap.hpp
        cigmar/classes/HashSet.hpp
        cigmar/classes/Queue.hpp
        cigmar/classes/Stack.hpp
        cigmar/classes/String.hpp
        cigmar/classes/Timer.hpp
        cigmar/classes/TreeMap.hpp
        cigmar/classes/TreeSet.hpp
        cigmar/interfaces/Comparable.hpp
        cigmar/interfaces/Hashable.hpp
        cigmar/interfaces/Streamable.hpp
        cigmar/types/pos_t.hpp
        cigmar/filesystem.hpp
        cigmar/init.hpp
        cigmar/numbers.hpp
        cigmar/print.hpp
        cigmar/std.hpp
        cigmar/symbols.hpp
        cigmar/time.hpp
        cigmar/unittests.hpp
        cigmar/utils.hpp
        cpp/cigmar.cpp
        cpp/filesystem.cpp
        cpp/filesystem_unix.inc
        cpp/filesystem_windows.inc
        cpp/unittests.cpp
        tests/test_cigmar.cpp
        tests/test_filesystem.cpp
        tests/test_HashMap.cpp
        tests/test_Queue.cpp
        tests/test_Stack.cpp
        tests/test_time.cpp
        tests/test_TreeMap.cpp
        main.cpp cigmar/base64.hpp cpp/base64.cpp)

add_executable(cigmar ${SOURCE_FILES})
