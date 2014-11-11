CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(stare)
set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_EXE_LINKER_FLAGS "-pthread")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CSS_FLAGS_RELEASE} -Wall")

add_executable(stare sqlite3.c main.cpp CMDparser.cpp serverv2.cpp Console.cpp DataStructs.h DocumentDatabase.cpp TokenDatabase.cpp EngineStatus.cpp WordPairCountDatabase.cpp HMMengine.cpp HTMLgenerator.cpp jsoncpp.cpp Stopwatch.cpp Tokenizer.cpp)

target_link_libraries(stare dl)

install(TARGETS stare RUNTIME DESTINATION bin)
