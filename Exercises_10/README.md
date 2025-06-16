Exercise 10.1 can be compiled and run using `Cmake` in a new `build` directory and contains a simple annealing algorithm without using MPI that i used to test the code before implementing the parallel version. The MPI code can be compiled using the `makefile`in the current directory. 

` Exercise_10.2_genetic.cpp` is the main file that contains the serial genetic algorithm implementation used for a comparison with parallel tempering.

### Data

there are two data directories

- `data` which contains the results running the code in my personal computer.

- `datatolab` (compressed) which contains the results running the code on a lab computer with 12 cores.