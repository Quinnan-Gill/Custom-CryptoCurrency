cmake_minimum_required (VERSION 3.0)
project (crypto)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

include_directories(include)

set(SOURCES src/*.cpp)

file(GLOB SOURCES "src/*.cpp")
add_executable(crypto ${SOURCES})
target_link_libraries(crypto cryptopp)
target_link_libraries(crypto gmp)
target_link_libraries(crypto gmpxx)