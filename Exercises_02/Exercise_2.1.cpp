#include <iostream>
#include <fstream>
#include "../Libraries/Parallel_Random_Number_Generator/random.h"
#include "../Libraries/lib/BA.h"



int main(){
    //########################### Point 1 ###########################
    // compute the integral with uniform sampling
    int n_throw = 1000000;
    int n_block = 100;
    Random rand_gen("../../Libraries/Parallel_Random_Number_Generator/Primes", "../../Libraries/Parallel_Random_Number_Generator/seed.in");

    auto inverse_cdf = [](double x){return 1.-sqrt(1.-x);};

    double * rnd_data = new double[n_throw];
    double * rnd_data2 = new double[n_throw];

    for (int i = 0; i < n_throw; i++){
        rnd_data[i] = rand_gen.Rannyu();
        rnd_data2[i] = inverse_cdf(rand_gen.Rannyu());
    }

    // function to integrate
    std::function<double(double)> f = [](double x){return (M_PI/2)*cos((M_PI/2)*x);};
    // denominator function for the importance sampling
    std::function<double(double)> den = [](double x){return 2.-2.*x;};

    BA_integral integral(n_block, n_throw, f, den);

    integral.compute(rnd_data, "../data/integral_uniform_sampling.dat");
    integral.compute_importance_sampling(rnd_data2, "../data/integral_importance_sampling.dat");

    return 0;

}