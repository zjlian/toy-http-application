FILE (GLOB EXAMPLES_SOURCES "*.cpp" "**/*.cpp" )
FILE (GLOB EXAMPLES_HEADERS "*.h" "**/*.h" )
message(STATUS "EXAMPLES_SOURCES ${EXAMPLES_SOURCES}")
message(STATUS "EXAMPLES_HEADERS ${EXAMPLES_HEADERS}")

add_executable(
    app_examples
    ${EXAMPLES_SOURCES}
    ${EXAMPLES_HEADERS}
)

target_include_directories(
    app_examples PRIVATE
    .
)

target_link_libraries(app_examples PRIVATE http_app)