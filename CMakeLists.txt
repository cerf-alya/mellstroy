cmake_minimum_required(VERSION 3.10)
project(GraphoDro4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mconsole")
endif()

include_directories(include)

# Основное CLI приложение
add_executable(grapho main.cpp)

# Тесты на граф
add_executable(test_graph_core tests/test_graph.cpp)

# Тесты на парсеры
add_executable(test_parsers tests/test_parsers.cpp)