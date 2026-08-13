cmake_minimum_required(VERSION 3.20)

project(limit_order_book LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(order_book
    src/main.cpp
    src/order_book.cpp
    src/trades.cpp
    src/command_parser.cpp
)

target_include_directories(order_book PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(order_book PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

add_executable(order_book_tests
    tests/order_book_tests.cpp
    src/order_book.cpp
    src/trades.cpp
)

target_include_directories(order_book_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(order_book_tests PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
