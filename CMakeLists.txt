cmake_minimum_required(VERSION 3.8)
project(src/cigmar)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-pthread")
include_directories(${PROJECT_SOURCE_DIR}/src)
set(SOURCE_FILES
        src/cigmar/base64.hpp
        src/cigmar/classes/array_t.hpp
        src/cigmar/classes/ArrayList.hpp
        src/cigmar/classes/Char.hpp
        src/cigmar/classes/Exception.hpp
        src/cigmar/classes/HashMap.hpp
        src/cigmar/classes/HashSet.hpp
        src/cigmar/classes/Queue.hpp
        src/cigmar/classes/Stack.hpp
        src/cigmar/classes/StaticCollection.hpp
        src/cigmar/classes/String.hpp
        src/cigmar/classes/StringView.hpp
        src/cigmar/classes/Timer.hpp
        src/cigmar/classes/TreeMap.hpp
        src/cigmar/classes/TreeSet.hpp
        src/cigmar/file/Lines.hpp
        src/cigmar/file/text/read.hpp
        src/cigmar/file/writer/Binary.hpp
        src/cigmar/file/writer/Text.hpp
        src/cigmar/filesystem.hpp
        src/cigmar/gui.hpp
        src/cigmar/format/tsv.hpp
        src/cigmar/hex.hpp
        src/cigmar/init.hpp
        src/cigmar/interfaces/Collection.hpp
        src/cigmar/interfaces/Comparable.hpp
        src/cigmar/interfaces/CrossComparable.hpp
        src/cigmar/interfaces/Hashable.hpp
        src/cigmar/interfaces/Streamable.hpp
        src/cigmar/math/Fraction.hpp
        src/cigmar/numbers.hpp
        src/cigmar/print.hpp
        src/cigmar/sqlite.hpp
        src/cigmar/std.hpp
        src/cigmar/symbols.hpp
        src/cigmar/time.hpp
        src/cigmar/types/pos_t.hpp
        src/cigmar/unittests.hpp
        src/cigmar/utils/Hasher.hpp
        src/cigmar/utils/Initializable.hpp
        src/cigmar/whirlpool.hpp
        src/cpp/base64.cpp
        src/cpp/cigmar.cpp
        src/cpp/file/text/read.cpp
        src/cpp/filesystem.cpp
        src/cpp/filesystem_unix.inc
        src/cpp/filesystem_windows.inc
        src/cpp/unittests.cpp
        src/cpp/whirlpool.cpp
        src/libraries/base64/base64.hpp
        src/libraries/json/json.hpp
        src/libraries/sqlite/sqlite3.c
        src/libraries/sqlite/sqlite3.h
        src/libraries/whirlpool/nessie.h
        src/libraries/whirlpool/Whirlpool.cpp
        src/main.cpp
        src/tests/test_arrays.cpp
        src/tests/test_cigmar.cpp
        src/tests/test_encodings.cpp
        src/tests/test_filesystem.cpp
        src/tests/test_HashMap.cpp
        src/tests/test_math.cpp
        src/tests/test_Queue.cpp
        src/tests/test_Stack.cpp
        src/tests/test_String.cpp
        src/tests/test_time.cpp
        src/tests/test_TreeMap.cpp
        src/tests/test_video.cpp
        src/video/ffmpeg.cpp
        src/video/ffmpeg.hpp
        src/video/manager/Database.cpp
        src/video/manager/Database.hpp
        src/video/manager/DbVideo.cpp
        src/video/manager/DbVideo.hpp
        src/video/manager/Folder.cpp
        src/video/manager/Folder.hpp
        src/video/manager/Library.cpp
        src/video/manager/Library.hpp
        src/video/manager/MultipleProperties.hpp
        src/video/manager/MultipleProperty.hpp
        src/video/manager/PropertyType.hpp
        src/video/manager/PropertyTypes.hpp
        src/video/manager/Report.hpp
        src/video/manager/RowMapping.hpp
        src/video/manager/TableRow.hpp
        src/video/manager/UniqueProperties.hpp
        src/video/manager/UniqueProperty.hpp
        src/video/Video.hpp
        src/video/manager/manager.hpp)

add_executable(cigmar ${SOURCE_FILES})

# It's unfortunate we have to add this line (because of sqlite)
# as this makes the compilation less trivial.
target_link_libraries(cigmar ${CMAKE_DL_LIBS})
