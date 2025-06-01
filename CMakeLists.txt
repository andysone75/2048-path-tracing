cmake_minimum_required (VERSION 3.8)

project ("2048-vulkan")

file(GLOB_RECURSE SRC_FILES "src/main.cpp" DEPENDS src/cpp/*.cpp)
add_executable(main src/main.cpp ${SRC_FILES})

target_include_directories(main PRIVATE
    ${CMAKE_SOURCE_DIR}/src/include
    ${CMAKE_SOURCE_DIR}/third-party/glm)

# Vulkan
find_package(Vulkan REQUIRED)

# GLFW
set(GLFW_ROOT ${CMAKE_SOURCE_DIR}/third-party/glfw)
target_include_directories(main PRIVATE "${GLFW_ROOT}/include")
target_link_directories(main PRIVATE "${GLFW_ROOT}/lib")

target_link_libraries(main PRIVATE Vulkan::Vulkan glfw3)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET main PROPERTY CXX_STANDARD 17)
endif()

add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/shaders"
        "$<TARGET_FILE_DIR:main>/shaders"
)