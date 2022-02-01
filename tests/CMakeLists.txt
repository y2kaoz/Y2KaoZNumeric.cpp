add_executable(BigIntTests 
    $<TARGET_OBJECTS:BigIntOptimized>
    Y2KaoZ/Numeric/BigInt/BigInt.cpp 
)
add_test(NAME BigIntTests COMMAND BigIntTests)

target_include_directories(BigIntTests PRIVATE "../src/")
target_compile_options(BigIntTests PRIVATE -Wall -Wextra -Wpedantic -Wconversion)

find_package(Catch2 3 REQUIRED)
target_link_libraries(BigIntTests PRIVATE Catch2::Catch2WithMain)
