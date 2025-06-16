# NUMERICAL SIMULATION LABORATORY

This repository contains the exercises of the course "laboratorio di simulazione numerica" (academic year 2024/2025).

Every numerical exercise is contained in a separate folder, with all the C++ code and the necessary files to run it. Inside each folder, you will find a jupyter notebook with the explanation and the results of the exercise.

## Requirements

The code is written in c++ and requires a C++ compiler to run (I personally used Apple clang version 17.0.0 (*clang-1700.0.13.3*)). In order to run the code you need `make` and `Cmake` installed on your system. 

- If there is already a `Makefile` in the folder, you can run `make` to compile the code.

- If there is no `Makefile`, you can compile the code creating a directory called `build` and running `cmake ..` inside it, then running `make` to compile the code the configuration files `CmakeLists.txt` are already in the correct directories.

### C++ libraries

The code uses the following libraries:

- [Armadillo](http://arma.sourceforge.net/): a C++ linear algebra library for fast matrix operations (Exercises 4,6 and 7).

- [mpich](https://www.mpich.org/): a high-performance and widely portable implementation of the Message Passing Interface (MPI) standard (Exercise 10).

### Python libraries

All the python code requirements are listed in the `requirements.txt` file. You can install them using pip:

```bash
pip install -r requirements.txt
```

*DISCLAIMER*: I personally used two different python environments to run the code, because [tensorflow v2.17](https://www.tensorflow.org/) is compatible with Python 3.8–3.11. All the details are in the `requirements.txt` file.

## License
This repository is licensed under GNU General Public License v3.0. See the [LICENSE](LICENSE) file for more details.