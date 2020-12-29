set(TARGET_LIB toml_json_converter)
add_library(${TARGET_LIB} STATIC converter.cpp converter.h)

# Add include path
target_include_directories(
    ${TARGET_LIB}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../external/rapidjson/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../external/toml11>)