#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "../Libraries/Parallel_Random_Number_Generator/random.h"
#include "Buffon.h"
#include <utility>

using namespace std;

int main(){

    // Buffon's needle simulation
    double L = 0.8;
    double d = 1.;

    int n_throws = 10e7;
    int n_blocks = 100;

    Buffon buffon(L, d, n_throws, n_blocks);
    buffon.simulate("../data/Buffon.dat");

    return 0;
}