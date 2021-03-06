cmake_minimum_required(VERSION 3.8)
project(src/cigmar)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-pthread")
include_directories(${PROJECT_SOURCE_DIR}/src)
set(SOURCE_FILES
        src/cigmar/base64.hpp
        src/cigmar/classes/AbstractString.hpp
        src/cigmar/classes/array_t.hpp
        src/cigmar/classes/ArrayList.hpp
        src/cigmar/classes/Char.hpp
        src/cigmar/exception/Exception.hpp
        src/cigmar/classes/HashMap.hpp
        src/cigmar/classes/HashSet.hpp
        src/cigmar/classes/Queue.hpp
        src/cigmar/classes/Stack.hpp
        src/cigmar/classes/StaticCollection.hpp
        src/cigmar/classes/String.hpp
        src/cigmar/classes/StringView.hpp
        src/cigmar/classes/TreeMap.hpp
        src/cigmar/classes/TreeSet.hpp
        src/cigmar/classes/UnicodeString.hpp
        src/cigmar/file/Lines.hpp
        src/cigmar/file/text/read.hpp
        src/cigmar/file/writer/Binary.hpp
        src/cigmar/file/writer/Text.hpp
        src/cigmar/filesystem.hpp
        src/cigmar/format/Tsv.hpp
        src/cigmar/gui/backend.hpp
        src/cigmar/gui/gui.cpp
        src/cigmar/gui/gui.hpp
        src/cigmar/gui/primitive.hpp
        src/cigmar/gui/utils.hpp
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
        src/cigmar/symbols.hpp
        src/cigmar/time.hpp
        src/cigmar/tree.hpp
        src/cigmar/types/pos_t.hpp
        src/cigmar/unicode.hpp
        src/cigmar/unittests.hpp
        src/cigmar/utils/Hasher.hpp
        src/cigmar/utils/Initializable.hpp
        src/cigmar/video/ffmpeg.cpp
        src/cigmar/video/ffmpeg.hpp
        src/cigmar/video/manager/Database.cpp
        src/cigmar/video/manager/Database.hpp
        src/cigmar/video/manager/DbVideo.cpp
        src/cigmar/video/manager/DbVideo.hpp
        src/cigmar/video/manager/Folder.cpp
        src/cigmar/video/manager/Folder.hpp
        src/cigmar/video/manager/Library.cpp
        src/cigmar/video/manager/Library.hpp
        src/cigmar/video/manager/manager.hpp
        src/cigmar/video/manager/MultipleProperties.hpp
        src/cigmar/video/manager/MultipleProperty.hpp
        src/cigmar/video/manager/PropertyType.hpp
        src/cigmar/video/manager/PropertyTypes.hpp
        src/cigmar/video/manager/Report.hpp
        src/cigmar/video/manager/RowMapping.hpp
        src/cigmar/video/manager/TableRow.hpp
        src/cigmar/video/manager/UniqueProperties.hpp
        src/cigmar/video/manager/UniqueProperty.hpp
        src/cigmar/video/Video.cpp
        src/cigmar/video/Video.hpp
        src/cigmar/whirlpool.hpp
        src/cpp/cigmar.cpp
        src/cpp/filesystem_unix.cpp
        src/cpp/filesystem_windows.cpp
        src/cpp/main.cpp
        src/cpp/tests/test_arrays.cpp
        src/cpp/tests/test_cigmar.cpp
        src/cpp/tests/test_encodings.cpp
        src/cpp/tests/test_filesystem.cpp
        src/cpp/tests/test_HashMap.cpp
        src/cpp/tests/test_math.cpp
        src/cpp/tests/test_Queue.cpp
        src/cpp/tests/test_Stack.cpp
        src/cpp/tests/test_time.cpp
        src/cpp/tests/test_TreeMap.cpp
        src/cpp/tests/test_video.cpp
        src/libraries/base64/base64.hpp
        src/libraries/json/json.hpp
        src/libraries/sqlite/sqlite3.c
        src/libraries/sqlite/sqlite3.h
        src/libraries/utf/utf.hpp
        src/libraries/whirlpool/nessie.h
        src/libraries/whirlpool/Whirlpool.cpp
        src/cigmar/exception/IndexException.hpp src/cpp/tests/testString.cpp src/cpp/tests/testUnicodeString.cpp)

add_executable(cigmar ${SOURCE_FILES})

# It's unfortunate we have to add this line (because of sqlite)
# as this makes the compilation less trivial.
target_link_libraries(cigmar ${CMAKE_DL_LIBS})
