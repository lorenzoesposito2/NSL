#include <iostream>
#include <vector>
#include <fstream>
#include "random.h"
#include "lib.cpp"

using namespace std;

int main(int argc, char *argv[]) {

    if(argc != 2) {
        cerr << "Usage: \n- 0 to sample uniform \n- 1 to sample Gauss " << endl;
        return 1;
    }

    bool Gaussian = (atoi(argv[1]) != 0);
    
    Random rand_gen("../../Libraries/Parallel_Random_Number_Generator/Primes", "../../Libraries/Parallel_Random_Number_Generator/seed.in");
    Random rand_gen_2("../../Libraries/Parallel_Random_Number_Generator/Primes", "../../Libraries/Parallel_Random_Number_Generator/seed.in");
    int n = 100000;
    int n_blocks = 100;
    int n_steps = n / n_blocks;

    vector<double> x(3);
    x[0] = 100.;
    x[1] = 100.;
    x[2] = 100.;

    double step_1s = (Gaussian == false) ? 1.2 : 0.75;
    double step_2p = (Gaussian == false) ? 2.95 : 1.9;

    // 50% acceptance rate with uniform distribution
    /*double step_1s = 1.2;
    double step_2p = 2.95;

    double step_1s_Gauss = 0.75;
    double step_2p_Gauss = 1.9;*/

    cout << "acceptance 1s" << endl;
    vector<vector<double>> samples_1s = metropolis(p_1s, rand_gen, x, step_1s, n, Gaussian);

    x[0] = 100.;
    x[1] = 100.;
    x[2] = 100.;

    cout << "acceptance 2p" << endl;
    vector<vector<double>> samples_2p = metropolis(p_2p, rand_gen_2, x, step_2p, n, Gaussian);

    // write samples to file
    ofstream out_1s_sample;
    Gaussian == false ? out_1s_sample.open("../data/1s_far.dat") : out_1s_sample.open("../data/1s_Gauss_far.dat");
    if (!out_1s_sample.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }
    out_1s_sample << "#x y z" << endl;
    for (int i = 0; i < n; ++i) {
        out_1s_sample << samples_1s[i][0] << " " << samples_1s[i][1] << " " << samples_1s[i][2] << endl;
    }
    out_1s_sample.close();
    ofstream out_2p_sample;
    Gaussian == false ? out_2p_sample.open("../data/2p_far.dat") : out_2p_sample.open("../data/2p_Gauss_far.dat");
    if (!out_2p_sample.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }
    out_2p_sample << "x y z" << endl;
    for (int i = 0; i < n; ++i) {
        out_2p_sample << samples_2p[i][0] << " " << samples_2p[i][1] << " " << samples_2p[i][2] << endl;
    }
    out_2p_sample.close();
    cout << "sampling data written in file" << endl;


    // evaluate blocking average of r
    vector<double> r(n_blocks);
    vector<double> r_2p(n_blocks);
    
    ofstream out;
    Gaussian == false ? out.open("../data/r_1s_far.dat") : out.open("../data/r_1s_Gauss_far.dat");
    ofstream out_2p;
    Gaussian == false ? out_2p.open("../data/r_2p_far.dat") : out_2p.open("../data/r_2p_Gauss_far.dat");
    if (!out.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }
    if(!out_2p.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    out << "#Block r_avg r_err" << endl;
    out_2p << "#Block r_avg r_err" << endl;
    double r_avg = 0;
    double r_avg2 = 0;
    double r_avg_2p = 0;
    double r_avg2_2p = 0;

    cout << "writing data to file" << endl;
    for (int i = 0; i < n_blocks; ++i) {
        double sum = 0;
        double sum_2p = 0;
        for (int j = 0; j < n_steps; ++j) {
            sum += norm(samples_1s[i * n_steps + j]);
            sum_2p += norm(samples_2p[i * n_steps + j]);
        }
        r[i] = sum / n_steps;
        r_2p[i] = sum_2p / n_steps;
        r_avg += r[i];
        r_avg_2p += r_2p[i];
        r_avg2 += r[i] * r[i];
        r_avg2_2p += r_2p[i] * r_2p[i];

        double r_avg_progressive = r_avg / (i + 1);
        double r_avg2_progressive = r_avg2 / (i + 1);
        double r_err_progressive = (i > 0) ? sqrt((r_avg2_progressive - r_avg_progressive * r_avg_progressive) / i) : 0;

        double r_avg_2p_progressive = r_avg_2p / (i + 1);
        double r_avg2_2p_progressive = r_avg2_2p / (i + 1);
        double r_err_2p_progressive = (i > 0) ? sqrt((r_avg2_2p_progressive - r_avg_2p_progressive * r_avg_2p_progressive) / i) : 0;

        out << i + 1 << " " << r_avg_progressive << " " << r_err_progressive << endl;
        out_2p << i + 1 << " " << r_avg_2p_progressive << " " << r_err_2p_progressive << endl;
    }

    out.close();
    out_2p.close();

    return 0;

}