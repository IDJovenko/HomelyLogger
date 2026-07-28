add_library(homelylog STATIC
  # TODO: Remove StubSource.cxx after adding the actual source files.
  StubSource.cxx
)

target_compile_features(homelylog PRIVATE cxx_std_23)

target_include_directories(homelylog PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/logger
    ${CMAKE_CURRENT_SOURCE_DIR}/log_manager
    ${CMAKE_CURRENT_SOURCE_DIR}/enricher
)
target_include_directories(homelylog PRIVATE
)
