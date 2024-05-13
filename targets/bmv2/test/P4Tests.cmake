include(${CMAKE_CURRENT_LIST_DIR}/TestTemplate.cmake)
# ##################################################################################################
# TEST PROGRAMS
# ##################################################################################################
set(PYTHON_TEST_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/run_test_batch.py")

set(V1_SEARCH_PATTERNS "include.*v1model.p4" "main|common_v1_test")
# General BMv2 tests supplied by the compiler.
set(P4TESTS_FOR_BMV2
    "${P4C_SOURCE_DIR}/testdata/p4_16_samples/*.p4"
    "${P4C_SOURCE_DIR}/testdata/p4_16_samples/dash/*.p4"
    "${P4C_SOURCE_DIR}/testdata/p4_16_samples/fabric_*/fabric.p4"
    "${P4C_SOURCE_DIR}/testdata/p4_16_samples/omec/*.p4"
    "${P4C_SOURCE_DIR}/testdata/p4_16_samples/pins/*.p4"
    # Custom tests
    "${CMAKE_CURRENT_LIST_DIR}/programs/*.p4"
)

p4c_find_tests("${P4TESTS_FOR_BMV2}" P4_16_V1_TESTS INCLUDE "${V1_SEARCH_PATTERNS}" EXCLUDE "")

# Filter some programs  because they have issues that are not captured with Xfails.
list(
  REMOVE_ITEM
  P4_16_V1_TESTS
  # These tests time out and require fixing.
)

set (EXTRA_OPTS "")

p4tools_add_tests(
  TESTS
  "${P4_16_V1_TESTS}"
  TAG
  "rtsmith-bmv2-v1model"
  DRIVER
  ${PYTHON_TEST_SCRIPT}
  TARGET
  "bmv2"
  ARCH
  "v1model"
  TEST_ARGS
  "${EXTRA_OPTS}"
)