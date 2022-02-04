add_executable(BigIntTests Y2KaoZ/Numeric/BigInt/BigInt.cpp)
add_test(NAME BigIntTests COMMAND BigIntTests)

find_package(Catch2 3 REQUIRED)
target_link_libraries(BigIntTests PRIVATE Catch2::Catch2WithMain ${PROJECT_NAME})
