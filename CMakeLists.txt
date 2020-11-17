cmake_minimum_required(VERSION 2.8)

project(color_coded)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE RELEASE)
endif()

find_package(Boost 1.65 COMPONENTS filesystem REQUIRED)
link_libraries(${Boost_LIBRARIES})
include_directories(${Boost_INCLUDE_DIR})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

message(STATUS "Updating submodules")
execute_process(
  COMMAND git submodule update --recursive --init
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

include(cmake/lua.cmake)

find_package(Curses   REQUIRED)
find_package(ZLIB     REQUIRED)
find_package(Threads  REQUIRED)
find_package(LLVM     REQUIRED)
find_package(LibClang REQUIRED)

include_directories(
  ${LLVM_INCLUDE_DIRS}
  ${CLANG_INCLUDE_DIRS}

  ${CURSES_INCLUDE_PATH}
  ${ZLIB_INCLUDE_PATH}

  $ENV{PWD}/include/
)

# Allow easier dynamic linking with clang
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_RPATH ${LLVM_LIB_PATH})

add_library(${PROJECT_NAME} SHARED src/main.cpp)

add_custom_target(${PROJECT_NAME}_track_api
  COMMAND ${CMAKE_CURRENT_LIST_DIR}/lib/track_api
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_track_api)

# Force the same naming across platforms
set_property(TARGET ${PROJECT_NAME} PROPERTY PREFIX "")
set_property(TARGET ${PROJECT_NAME} PROPERTY OUTPUT_NAME "${PROJECT_NAME}.so")
set_property(TARGET ${PROJECT_NAME} PROPERTY SUFFIX "")

set(WARNING_FLAGS
    "-Wall -Wextra -pedantic -Wno-missing-field-initializers -Wno-unused-variable -Wno-deprecated-declarations -Wno-unused-parameter")
set_property(TARGET ${PROJECT_NAME} PROPERTY COMPILE_FLAGS ${WARNING_FLAGS})

target_link_libraries(${PROJECT_NAME}
  ${CLANG_LIBS}
  ${LLVM_LIBRARIES}
  ${LLVM_LDFLAGS}
)

target_link_libraries(${PROJECT_NAME}
  ${LUA_LIBRARIES}
  ${CURSES_LIBRARY}
  ${ZLIB_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_LIST_DIR})
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_LIST_DIR})

include_directories(
  include
  lib/jest/include
  test/include
)
add_definitions(-D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS)
add_executable(${PROJECT_NAME}_config_test ${CMAKE_CURRENT_LIST_DIR}/test/src/config/main.cpp)
target_link_libraries(${PROJECT_NAME}_config_test
  ${CLANG_LIBS}
  ${LLVM_LIBRARIES}
  ${LLVM_LDFLAGS}
  ${LUA_LIBRARIES}
  ${CURSES_LIBRARY}
  ${ZLIB_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
)
set_property(TARGET ${PROJECT_NAME}_config_test PROPERTY COMPILE_FLAGS ${WARNING_FLAGS})

add_custom_target(${PROJECT_NAME}_test
  COMMAND ${PROJECT_NAME}_config_test
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)
add_dependencies(${PROJECT_NAME}_test ${PROJECT_NAME}_config_test)
