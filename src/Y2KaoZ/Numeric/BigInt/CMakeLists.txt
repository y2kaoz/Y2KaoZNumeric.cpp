set(HEADERS "BigInt.hpp")
set(SOURCES "BigInt.cpp")

add_library(BigIntOptimized OBJECT ${SOURCES} ${HEADERS})
target_include_directories(BigIntOptimized PUBLIC "../../../")
target_compile_options(BigIntOptimized PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Os)

add_library(BigIntDebug OBJECT ${SOURCES} ${HEADERS})
target_include_directories(BigIntDebug PUBLIC "../../../")
target_compile_options(BigIntDebug PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Og -g2)

install(FILES ${HEADERS} DESTINATION "include/Y2KaoZ/Numeric/BigInt")