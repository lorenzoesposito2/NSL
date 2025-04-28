cmake_minimum_required(VERSION 3.30)

project(Exercise_02)

include_directories("../Libraries/Parallel_Random_Number_Generator" "../Libraries/lib")

set(CMAKE_CXX_STANDARD 20)

add_executable(Exercise_02.1 Exercise_2.1.cpp ../Libraries/Parallel_Random_Number_Generator/random.cpp ../Libraries/lib/BA.cpp )
add_executable(Exercise_02.2 Exercise_2.2.cpp ../Libraries/Parallel_Random_Number_Generator/random.cpp rw.cpp )