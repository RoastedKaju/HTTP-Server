cmake_minimum_required(VERSION 3.10)
project(HTTP-Server)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)


# Add include directory
include_directories(include)

# Find all source files in src/ and add them to executeable
file(GLOB SOURCES "src/*.c")

# Create executeable
add_executable(HttpServer ${SOURCES})

# Windows-specific configuration
if(WIN32)
	target_link_libraries(HttpServer ws2_32)
endif()

# Set the output directory
set_target_properties(HttpServer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_BINARY_DIR}/Debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release
)

# Function to copy www folder
function(copy_www TARGET_NAME)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/www"
            "$<TARGET_FILE_DIR:${TARGET_NAME}>/www"
        COMMENT "Copying www folder to output directory"
    )
endfunction()

copy_www(HttpServer)