CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(astar)
set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_EXE_LINKER_FLAGS "-pthread")

add_executable(astar sqlite3.c main.cpp CMDparser.cpp Console.cpp DataStructs.h Database.cpp HMMengine.cpp HTMLgenerator.cpp Stopwatch.cpp Tokenizer.cpp)
target_link_libraries(astar dl)

install(TARGETS astar RUNTIME DESTINATION bin)
