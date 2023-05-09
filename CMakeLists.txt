cmake_minimum_required(VERSION 3.23)
project(VulkanGaming)

set(SRC_DIR "Vulkan gaming(real)")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

add_executable(VulkanGame "")

target_include_directories(VulkanGame
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_directories(VulkanGame
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/lib)

target_link_libraries(VulkanGame vulkan-1 glfw3)

set_target_properties(VulkanGame PROPERTIES CXX_STANDARD 23)

add_custom_command(TARGET VulkanGame POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR}/textures 
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/textures)

add_custom_command(TARGET VulkanGame POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR}/shaders 
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders)

add_subdirectory(${SRC_DIR})