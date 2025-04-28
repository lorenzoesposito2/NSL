cmake_minimum_required(VERSION 3.30)

project(Exercise_01)

include_directories("../Libraries/Parallel_Random_Number_Generator")

set(CMAKE_CXX_STANDARD 20)

add_executable(Exercise_01.1 Exercise_1.1.cpp ../Libraries/Parallel_Random_Number_Generator/random.cpp)
add_executable(Exercise_01.2 Exercise_1.2.cpp ../Libraries/Parallel_Random_Number_Generator/random.cpp)
add_executable(Exercise_01.3 Exercise_1.3.cpp ../Libraries/Parallel_Random_Number_Generator/random.cpp Buffon.cpp)
