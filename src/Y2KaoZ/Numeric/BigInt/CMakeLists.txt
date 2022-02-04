set(HEADERS "BigInt.hpp")
set(SOURCES "BigInt.cpp")

add_library(BigIntOptimized OBJECT ${SOURCES} ${HEADERS})
set_target_properties(BigIntOptimized PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(BigIntOptimized PUBLIC "../../../")
target_compile_options(BigIntOptimized PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Os)

add_library(BigIntDebug OBJECT ${SOURCES} ${HEADERS})
set_target_properties(BigIntDebug PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(BigIntDebug PUBLIC "../../../")
target_compile_options(BigIntDebug PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Og -g2)

foreach(HEADER ${HEADERS})
configure_file("${HEADER}" "${CMAKE_BINARY_DIR}/${PROJECT_NAME}/include/Y2KaoZ/Numeric/BigInt/${HEADER}" COPYONLY)
endforeach(HEADER)

install(FILES ${HEADERS} DESTINATION "include/Y2KaoZ/Numeric/BigInt")
