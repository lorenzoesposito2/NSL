#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
#include "gbm.h"

using namespace std;

double error(double* AV, double* AV2, int n) {
    if (n == 0) return 0;
    return sqrt((AV2[n] - AV[n] * AV[n]) / n);
}

int main(int argc, char* argv[]) {

    if(argc != 2){
        cerr << "Usage: insert n_step value as argument" << endl;
        return 1;
    }
    //############################### Point 1 ###############################

    double S_0 = 100.; // initial asset price
    double T = 1.; // delivery time
    double K = 100.; // strike price
    double r = 0.1; // interest rate
    double sigma = 0.25; // volatility
    double t = 0.;
    double n_steps = atof(argv[1]); // number of steps
    int M = 1000; // number of simulations
    int N = 100; // number of blocks

    double S = S_0;
    gbm asset(S_0, r, sigma, "../../Libraries/Parallel_Random_Number_Generator/seed2.in", "../../Libraries/Parallel_Random_Number_Generator/Primes");
    asset.set_dt(T / n_steps);


    double* call_ave = new double[N]();
    double* call_ave2 = new double[N]();
    double* put_ave = new double[N]();
    double* put_ave2 = new double[N]();

    std::ostringstream filename;
    filename << "../data/option_prices_" << n_steps << ".dat";

    ofstream out(filename.str());
    if (!out.is_open()) {
        cerr << "Errore in output file opening" << endl;
        return 1;
    }
    out << "#Block Call_Ave Call_Error Put_Ave Put_Error" << endl;

    cout << "saving results in file " << filename.str() << endl;
    for (int i = 0; i < N; i++) {
        double call = 0.;
        double put = 0.;
        for (int j = 0; j < M / N; j++) {
            asset.reset(S_0);
        
            for (int k = 0; k < n_steps; k++) {
                asset.update();
            }
            call += exp(-r * T) * max(0., asset.get_S() - K);
            put += exp(-r * T) * max(0., K - asset.get_S());
        }
        call /= (M / N);
        put /= (M / N);

        if (i == 0) {
            call_ave[i] = call;
            call_ave2[i] = call * call;
            put_ave[i] = put;
            put_ave2[i] = put * put;
        } else {
            call_ave[i] = call_ave[i - 1] + (call - call_ave[i - 1]) / (i + 1);
            call_ave2[i] = call_ave2[i - 1] + (call * call - call_ave2[i - 1]) / (i + 1);
            put_ave[i] = put_ave[i - 1] + (put - put_ave[i - 1]) / (i + 1);
            put_ave2[i] = put_ave2[i - 1] + (put * put - put_ave2[i - 1]) / (i + 1);
        }

        double call_error = error(call_ave, call_ave2, i);
        double put_error = error(put_ave, put_ave2, i);

        out << i + 1 << " " << call_ave[i] << " " << call_error << " " << put_ave[i] << " " << put_error << endl;
    }

    out.close();

    delete[] call_ave;
    delete[] call_ave2;
    delete[] put_ave;
    delete[] put_ave2;

    return 0;
}