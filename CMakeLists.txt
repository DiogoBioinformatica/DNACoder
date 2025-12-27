cmake_minimum_required(VERSION 3.24)
project(dnacoder_backend LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(gRPC REQUIRED)
find_package(protobuf REQUIRED)
find_package(TBB REQUIRED)
find_package(libpqxx REQUIRED)

set(PROTO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/proto)
set(PROTO_FILE ${PROTO_DIR}/sequence.proto)

# Protoc + gRPC plugin
get_target_property(GRPC_CPP_PLUGIN_LOCATION gRPC::grpc_cpp_plugin LOCATION)

set(GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GEN_DIR})

set(PROTO_SRC ${GEN_DIR}/sequence.pb.cc)
set(PROTO_HDR ${GEN_DIR}/sequence.pb.h)
set(GRPC_SRC  ${GEN_DIR}/sequence.grpc.pb.cc)
set(GRPC_HDR  ${GEN_DIR}/sequence.grpc.pb.h)

add_custom_command(
  OUTPUT ${PROTO_SRC} ${PROTO_HDR} ${GRPC_SRC} ${GRPC_HDR}
  COMMAND protobuf::protoc
  ARGS --proto_path=${PROTO_DIR}
       --cpp_out=${GEN_DIR}
       --grpc_out=${GEN_DIR}
       --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN_LOCATION}
       ${PROTO_FILE}
  DEPENDS ${PROTO_FILE}
  VERBATIM
)

add_library(proto_gen
  ${PROTO_SRC} ${GRPC_SRC}
)
target_include_directories(proto_gen PUBLIC ${GEN_DIR})
target_link_libraries(proto_gen PUBLIC gRPC::grpc++ protobuf::libprotobuf)

add_library(engine src/engine/analyze.cpp)
target_include_directories(engine PUBLIC src)
target_link_libraries(engine PUBLIC TBB::tbb)

add_library(infra_db src/infra/db.cpp)
target_include_directories(infra_db PUBLIC src)
target_link_libraries(infra_db PUBLIC libpqxx::pqxx)

add_library(api_lib src/api/sequenceserviceimpl.cpp)
target_include_directories(api_lib PUBLIC src)
target_link_libraries(api_lib PUBLIC proto_gen engine infra_db)

add_executable(grpc_server src/api/grpcservermain.cpp)
target_link_libraries(grpc_server PRIVATE api_lib)

add_executable(grpc_client src/client/grpcclientmain.cpp)
target_link_libraries(grpc_client PRIVATE proto_gen gRPC::grpc++)
target_include_directories(grpc_client PRIVATE ${GEN_DIR})

