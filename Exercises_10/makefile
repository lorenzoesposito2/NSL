# Makefile for exercise_10.2.cpp using MPI and random.h

CXX = mpicxx
CXXFLAGS = -O2 -Wall -I../Libraries/Parallel_Random_Number_Generator -I../Exercises_09
TARGET = exercise_10.2
SRC = exercise_10.2.cpp ../Libraries/Parallel_Random_Number_Generator/random.cpp ../Exercises_09/lib.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
