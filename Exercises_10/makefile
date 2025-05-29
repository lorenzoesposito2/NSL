CXX = mpicxx
CXXFLAGS = -O2 -Wall -I../Libraries/Parallel_Random_Number_Generator -I../Exercises_09
TARGET = Exercise_10.2
SRC = Exercise_10.2.cpp \
      ../Libraries/Parallel_Random_Number_Generator/random.cpp \
      ../Exercises_09/lib.cpp

GENETIC_CXX = g++
GENETIC_TARGET = Exercise_10.2_genetic
GENETIC_SRC = Exercise_10.2_genetic.cpp \
              ../Libraries/Parallel_Random_Number_Generator/random.cpp \
              ../Exercises_09/lib.cpp

all: $(TARGET) $(GENETIC_TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

$(GENETIC_TARGET): $(GENETIC_SRC)
	$(GENETIC_CXX) $(CXXFLAGS) -o $(GENETIC_TARGET) $(GENETIC_SRC)

clean:
	rm -f $(TARGET) $(GENETIC_TARGET)