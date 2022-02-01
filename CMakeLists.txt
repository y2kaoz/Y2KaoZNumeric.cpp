cmake_minimum_required(VERSION 3.22)

SET(CMAKE_INSTALL_PREFIX "./Y2KaoZNumeric.cpp/")
project(Y2KaoZNumeric VERSION 0.0.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory("src/Y2KaoZ/Numeric/BigInt")

add_library(${PROJECT_NAME}Shared SHARED $<TARGET_OBJECTS:BigIntOptimized>)
target_include_directories(${PROJECT_NAME}Shared PUBLIC "./src/")
set_target_properties(${PROJECT_NAME}Shared PROPERTIES OUTPUT_NAME ${PROJECT_NAME}-${PROJECT_VERSION})
install(TARGETS ${PROJECT_NAME}Shared DESTINATION "lib")

add_library(${PROJECT_NAME}SharedDebug SHARED $<TARGET_OBJECTS:BigIntDebug>)
target_include_directories(${PROJECT_NAME}SharedDebug PUBLIC "./src/")
set_target_properties(${PROJECT_NAME}SharedDebug PROPERTIES OUTPUT_NAME ${PROJECT_NAME}Debug-${PROJECT_VERSION})
install(TARGETS ${PROJECT_NAME}SharedDebug DESTINATION "lib")

add_library(${PROJECT_NAME}Static STATIC $<TARGET_OBJECTS:BigIntOptimized>)
target_include_directories(${PROJECT_NAME}Static PUBLIC "./src/")
set_target_properties(${PROJECT_NAME}Static PROPERTIES OUTPUT_NAME ${PROJECT_NAME}-${PROJECT_VERSION})
install(TARGETS ${PROJECT_NAME}Static DESTINATION "lib")

add_library(${PROJECT_NAME}StaticDebug STATIC $<TARGET_OBJECTS:BigIntDebug>)
target_include_directories(${PROJECT_NAME}StaticDebug PUBLIC "./src/")
set_target_properties(${PROJECT_NAME}StaticDebug PROPERTIES OUTPUT_NAME ${PROJECT_NAME}Debug-${PROJECT_VERSION})
install(TARGETS ${PROJECT_NAME}StaticDebug DESTINATION "lib")

configure_file(CMakeLists.txt.in CMakeLists.txt.out @ONLY)
install(FILES "${PROJECT_BINARY_DIR}/CMakeLists.txt.out" DESTINATION "." RENAME CMakeLists.txt)

add_subdirectory("./tests")
