cmake_minimum_required(VERSION 3.10)
 
project(FOOGAME
  VERSION 0.1.0
)
 
find_package(gf REQUIRED)
 
add_executable(template
  gameTemplate.cc
)

add_executable(tutorial
  tutorial.cc)
 
target_link_libraries(template
  PRIVATE
    gf::graphics
    gf::net
)

target_link_libraries(tutorial
  PRIVATE
    gf::graphics
    gf::net
)