#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include "lib.cpp"
#include "BA.h"

using namespace std;

int main(int argc, char *argv[]) {
    if(argc != 3) {
        cerr << "Usage: " << " <mu> <sigma>" << endl;
        return 1;
    }


    ofstream output("../data/sample.dat");
    if (!output) {
        cerr << "Error opening file" << endl;
        return 1;
    }
    double x0 = 0.;
    double step = 2.4;
    double mu = atof(argv[1]);
    double sigma = atof(argv[2]);
    double n = 1000000;
    double n_blocks = 1000;

    Random rand_gen("../../Libraries/Parallel_Random_Number_Generator/Primes", "../../Libraries/Parallel_Random_Number_Generator/seed.in");
    double * samples = metropolis(psi_trial_abs, rand_gen, x0, step, n, mu, sigma);

    auto bound_function = bind(H_psi_frac_psi, placeholders::_1, mu, sigma);
    BA_integral mean_H(n_blocks, n, bound_function, [](double x) { return 1.0; });
    mean_H.compute(samples, "../data/mean_H.dat");

    return 0;
}