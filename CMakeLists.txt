cmake_minimum_required (VERSION 3.0)
set(PROJECT_NAME crypto)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

find_package(Protobuf REQUIRED)

include_directories(include)

set(SOURCES src/*.cpp)
set(PROTO_FILES proto/block.proto)
set(HDR_FILES)

PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HDR ${PROTO_FILES})

file(GLOB SOURCES "src/*.cpp")
add_executable(
    ${PROJECT_NAME} ${SOURCES} ${HDR_FILES}
    ${PROTO_SRC} ${PROTO_HDR} ${PROTO_FILES}
)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${PROTOBUF_INCLUDE_DIRS}
)
target_link_libraries(crypto cryptopp)
target_link_libraries(crypto gmp gmpxx)
target_link_libraries(crypto pthread leveldb)
target_link_libraries(crypto ${PROTOBUF_LIBRARIES})