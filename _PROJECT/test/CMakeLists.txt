cmake_minimum_required(VERSION 3.10)
 
project(SWIFTNESS_TEST_ONE
  VERSION 0.1.0
)
 
find_package(gf REQUIRED)

add_executable(game
  main.cc
  squareEntity.cc
  plateforme.cc)

target_include_directories(game
PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR})
  

 
target_link_libraries(game
  PRIVATE
    gf::graphics
    gf::net
)