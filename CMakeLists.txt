cmake_minimum_required(VERSION 3.28)

project(temp_control VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2_ttf REQUIRED CONFIG REQUIRED COMPONENTS SDL2_ttf)
find_package(SDL2_image REQUIRED CONFIG REQUIRED COMPONENTS SDL2_image)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

add_executable(temp_control src/App.cpp src/main.cpp src/SensorUpdater.cpp)

target_link_libraries(temp_control PRIVATE SDL2::SDL2 SDL2_ttf::SDL2_ttf SDL2_image::SDL2_image bpl_sys bpl_graphics pinode fmt bpl_net)

install(PROGRAMS temp_control DESTINATION bin)
install(DIRECTORY resources/fonts DESTINATION share)
install(DIRECTORY resources/ui DESTINATION share/games/temp_control)
install(DIRECTORY resources/views DESTINATION share/games/temp_control)