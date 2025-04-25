# Examples CMakeLists.txt

# Add executable for the main example
add_executable(example_main main.c)

# Link against the main library
target_link_libraries(example_main PRIVATE goengc)

# Include directories
target_include_directories(example_main PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# Set C17 standard for examples
set_target_properties(example_main PROPERTIES
  C_STANDARD 17
  C_STANDARD_REQUIRED ON
  C_EXTENSIONS OFF
) 