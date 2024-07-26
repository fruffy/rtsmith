
add_custom_target(
  p4rtsmith_runtime_dir ${CMAKE_COMMAND} -E make_directory
                   ${CMAKE_CURRENT_BINARY_DIR}/control_plane/p4runtime &&
                   ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/control_plane/bfruntime
)

# Unity builds do not work for Protobuf...
set(CMAKE_UNITY_BUILD_PREV ${CMAKE_UNITY_BUILD})
set(CMAKE_UNITY_BUILD OFF)
add_library(p4rtsmith-control-plane-runtime OBJECT ${BFRUNTIME_RT_PROTO} ${BFRUNTIME_TESTS_PROTO})
target_include_directories(p4rtsmith-control-plane-runtime PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(p4rtsmith-control-plane-runtime PRIVATE controlplane)
add_dependencies(p4rtsmith-control-plane-runtime p4rtsmith_runtime_dir)
protobuf_generate(
  TARGET p4rtsmith-control-plane-runtime
  LANGUAGE cpp python
  IMPORT_DIRS ${Protobuf_INCLUDE_DIRS} ${P4C_SOURCE_DIR}/control-plane ${P4C_SOURCE_DIR}
  PROTOC_OUT_DIR ${P4C_BINARY_DIR}
)
# Reset temporary variable modifications.
set(CMAKE_UNITY_BUILD ${CMAKE_UNITY_BUILD_PREV})
