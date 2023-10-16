cmake_minimum_required(VERSION 3.26)
project(Proyecto C)
set(CMAKE_C_STANDARD 11)
file(GLOB SOURCES "*.c")

include_directories("C:/libquickmail/include")
link_directories("C:/libquickmail/lib")
find_library(myquickmail quickmail)

if (${myquickmail} STREQUAL myquickmail-NOTFOUND)
    message(FATAL_ERROR "No se encuentra la libreria")
else()
    message(STATUS "Encontrada en: ${myquickmail}")
endif()

add_executable(Proyecto main.c cJSON.h cJSON.c)
target_link_libraries(Proyecto quickmail)