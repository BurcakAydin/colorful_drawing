cmake_minimum_required(VERSION 3.10)
project(SFML_Game)

set(CMAKE_CXX_STANDARD 17)

# SFML 2.5 için doğru yolu belirt
set(SFML_DIR "/opt/homebrew/opt/sfml@2/lib/cmake/SFML")

find_package(SFML 2 REQUIRED COMPONENTS graphics window system)

add_executable(game main.cpp)
target_link_libraries(game sfml-graphics sfml-window sfml-system)
