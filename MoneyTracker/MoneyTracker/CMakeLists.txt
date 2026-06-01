cmake_minimum_required(VERSION 3.16)

project(MoneyTracker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

qt_add_executable(MoneyTracker
    main.cpp
)

target_link_libraries(MoneyTracker PRIVATE
    Qt6::Widgets
    Qt6::Sql
)