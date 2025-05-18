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