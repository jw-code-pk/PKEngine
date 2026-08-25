cmake_minimum_required(VERSION 3.10)
project(PKEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

set(OGRE_SDK "/Users/jewagenaar/SDK/ogre/sdk") 
list(APPEND CMAKE_PREFIX_PATH "${OGRE_SDK}")
find_package(OGRE REQUIRED COMPONENTS Bites RTShaderSystem)
find_package(SDL2 REQUIRED)
find_package(nlohmann_json 3.12.0 REQUIRED)

file(GLOB_RECURSE SOURCE_FILES 
  "src/*.cpp" 
  "src/*.h"
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${OGRE_INCLUDE_DIRS}
  ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
  ${OGRE_LIBRARIES}
  ${SDL2_LIBRARIES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

if(APPLE)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OGRE_SDK}/bin/plugins.cfg" $<TARGET_FILE_DIR:${PROJECT_NAME}>
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OGRE_SDK}/bin/resources.cfg" $<TARGET_FILE_DIR:${PROJECT_NAME}>
      COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Assets" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/Assets"
  )
endif()
