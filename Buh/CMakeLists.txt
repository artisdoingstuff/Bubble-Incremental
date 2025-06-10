project(Buh)

set(CMAKE_CXX_STANDARD 17)

include_directories("C:/SFML-3.0.0+32-Bit/include")

link_directories("C:/SFML-3.0.0+32-Bit/lib")

add_executable (Buh "Buh.cpp" "Buh.h")

target_link_libraries(Buh sfml-graphics sfml-window sfml-system sfml-audio sfml-network)