cmake_minimum_required(VERSION 3.28)

project(temp_control VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2_ttf REQUIRED CONFIG REQUIRED COMPONENTS SDL2_ttf)
find_package(SDL2_image REQUIRED CONFIG REQUIRED COMPONENTS SDL2_image)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

include_directories(BEFORE ../libbpl_graphics/libbpl_graphics/include
        ../pinode/libpinode/include
        ../libbpl_net/libbpl_net/include
        ../libbpl_sys/libbpl_sys/include
        ../libbpl_storage/libbpl_storage/include
        ../libbpl_controls/libbpl_controls/include)

add_executable(temp_control src/App.cpp src/main.cpp src/SensorUpdater.cpp src/ScreenSensor.cpp src/SystemMenu.cpp src/SystemMenuLogic.cpp)

target_link_libraries(temp_control PRIVATE SDL2::SDL2 SDL2_ttf::SDL2_ttf SDL2_image::SDL2_image bpl_sys bpl_controls bpl_graphics pinode fmt bpl_net devices_dht22)

install(TARGETS temp_control DESTINATION usr/bin)
install(DIRECTORY resources/fonts DESTINATION usr/share)
install(DIRECTORY resources/ui DESTINATION usr/share/games/temp_control)
install(DIRECTORY resources/views DESTINATION usr/share/games/temp_control)