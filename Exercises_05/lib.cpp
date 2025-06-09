#include <iostream>
#include <vector>
#include <cmath>
#include "random.h"

using namespace std;

double norm(vector<double> r){
    double norm = 0;
    for (int i = 0; i < r.size(); ++i) {
        norm += r[i]*r[i];
    }
    return sqrt(norm);
}

// probability density functions for the 1s and 2p orbitals
double p_1s(vector<double> r){
    return exp(-2*norm(r));
}

double p_2p(vector<double> r){
    return pow(r[2],2)*exp(-norm(r));
}


/*
metropolis algorithm
- f: function to sample
- x0: initial point
- step: step size
- n: number of samples
- returns: vector of samples
*/
vector<vector<double>> metropolis(double (*f)(vector<double>), Random& rnd, vector<double> x0, double step, int n, bool Gaussian = false) {
    vector<vector<double>> x(n, vector<double>(3));
    int accepted = 0;
    x[0] = x0;

    if(!Gaussian){
        for (int i = 1; i < n; i++) {
            vector<double> x_new(3);
            for (int j = 0; j < 3; j++) {
                x_new[j] = x[i-1][j] + step * (rnd.Rannyu(-1, 1));
            }
            double p_accept = f(x_new) / f(x[i-1]); // assuming T(x_n|x_n-1) is symmetric
            if (rnd.Rannyu() < p_accept) {
                x[i] = x_new;
                accepted++;
            } else {
                x[i] = x[i-1];
            }
        }
    } else {
        for (int i = 1; i < n; i++) {
            vector<double> x_new(3);
            for (int j = 0; j < 3; j++) {
                x_new[j] = x[i-1][j] + step * (rnd.Gauss(0., 1.));
            }
            double p_accept = f(x_new) / f(x[i-1]); // assuming T(x_n|x_n-1) is symmetric
            if (rnd.Rannyu() < p_accept) {
                x[i] = x_new;
                accepted++;
            } else {
                x[i] = x[i-1];
            }
        }
    }
    cout << "Acceptance rate: " << (double)accepted / n << endl;
    return x;
}