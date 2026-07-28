find_package(Threads REQUIRED)
find_package(GTest CONFIG REQUIRED)

add_executable(homelylog_unit_tests
  enricher/EnricherChaneBaseTest.cpp
)

target_compile_features(homelylog_unit_tests PRIVATE cxx_std_23)

target_include_directories(homelylog_unit_tests PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/
  ${CMAKE_CURRENT_SOURCE_DIR}/mocks
  ${CMAKE_CURRENT_SOURCE_DIR}/enricher
)

target_link_libraries(homelylog_unit_tests PRIVATE
  homelylog
  GTest::gtest
  GTest::gmock
  GTest::gtest_main
  Threads::Threads
)

include(GoogleTest)
gtest_discover_tests(homelylog_unit_tests)