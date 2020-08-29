cmake_minimum_required( VERSION 3.7 )

project( protobuf_example )

# this is needed to be able to use the protobuf_generate_cpp function
# because it is deprecated in the newer versions of the protobuf vcpkg
# package on windows.
if(WIN32)
  set(protobuf_MODULE_COMPATIBLE TRUE)
endif()

# On MacOS clang complains if this is not set
set (CMAKE_CXX_STANDARD 11)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/asio/ )

add_definitions(-DASIO_STANDALONE)

# Get all the .proto files in the proto subdirectory
file(GLOB PROTO_FILES "${CMAKE_CURRENT_SOURCE_DIR}/proto/*.proto")

# Find the protobuf package
find_package(Protobuf REQUIRED)

# Include the protobuf directory
include_directories(${Protobuf_INCLUDE_DIRS})
# include current directory so we can find generated protobuf files
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# generate the pb.h/pb.cc files
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

add_subdirectory(src)

add_executable(${PROJECT_NAME} ${SRC_LIST} ${PROTO_SRCS} ${PROTO_HDRS})

# link to protobuf library
target_link_libraries(${PROJECT_NAME} ${Protobuf_LIBRARIES})
