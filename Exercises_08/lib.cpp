#include <iostream>
#include <vector>
#include <cmath>
#include "random.h"

using namespace std;

double psi_trial_abs(double x, double mu, double sigma){
    return pow(abs(exp(-pow(x-mu,2)/(2*pow(sigma,2))) + exp(-pow(x+mu,2)/(2*pow(sigma,2)))),2);
}

double psi_trial(double x, double mu, double sigma){
    return exp(-pow(x-mu,2)/(2*pow(sigma,2))) + exp(-pow(x+mu,2)/(2*pow(sigma,2)));
}

double psi_trial_derivative(double x, double mu, double sigma){
    return exp(-pow(x+mu,2)/(2*pow(sigma,2))) * (pow(x+mu,2)/pow(sigma,4) - 1/(sigma*sigma)) + exp(-pow(x-mu,2)/(2*pow(sigma,2))) * (pow(x-mu,2)/pow(sigma,4) - 1/(sigma*sigma));
}

double H_psi_frac_psi(double x, double mu, double sigma) {
    return - psi_trial_derivative(x, mu, sigma) / psi_trial(x, mu, sigma) + pow(x, 4) - 5./2. * pow(x,2) ;
}

double* metropolis(double (*f)(double, double, double), Random& rnd, double x0, double step, int n, double mu, double sigma) {
    double* x = new double[n]; 
    int accepted = 0;
    x[0] = x0;

    for (int i = 1; i < n; i++) {
        double x_new = x[i-1] + step * (rnd.Rannyu(-1, 1));
        double p_accept = f(x_new, mu, sigma) / f(x[i-1], mu, sigma); // assuming T(x_n|x_n-1) is symmetric
        if (rnd.Rannyu() < p_accept) {
            x[i] = x_new;
            accepted++;
        } else {
            x[i] = x[i-1];
        }
    }
    cout << "Acceptance rate: " << (double)accepted/n << endl;
    return x; 
}