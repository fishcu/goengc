cmake_minimum_required(VERSION 3.10)

# Project name
project(goengc VERSION 0.1.0 LANGUAGES C)

# Set C standard to C17
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Library sources
add_library(${PROJECT_NAME} 
  src/board.c
  src/floodfill.c
  src/popcount.c
  src/constants.c
)

# Include directories
target_include_directories(${PROJECT_NAME}
  PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Installation configuration
install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

install(DIRECTORY include/ DESTINATION include)

# Add examples subdirectory
add_subdirectory(examples)

# Optional: Add tests subdirectory if present
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
  add_subdirectory(tests)
endif() 