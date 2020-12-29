set(TEST_TARGET toml_json_converter_test)
add_executable(${TEST_TARGET} main.cpp)

# Add include path and library
target_include_directories(
    ${TEST_TARGET}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../src>)
target_link_libraries(
    ${TEST_TARGET}
    PRIVATE toml_json_converter)

set_target_properties(${TEST_TARGET} PROPERTIES LINK_FLAGS "-Wl,-rpath,../lib")

add_custom_target(example_copy ALL)
add_custom_command(
        TARGET example_copy POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/example.toml
                ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/example.toml)