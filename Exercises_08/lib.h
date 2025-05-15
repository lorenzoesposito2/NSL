#ifndef LIB_H
#define LIB_H

#include <iostream>
#include <vector>
#include <cmath>
#include "random.h"

using namespace std;

// Useful functions
double psi_trial_abs(double x, double mu, double sigma);
double psi_trial(double x, double mu, double sigma);
double psi_trial_derivative(double x, double mu, double sigma);
double H_psi_frac_psi(double x, double mu, double sigma);

double* metropolis(double (*f)(double, double, double), Random& rnd, double x0, double step, int n, double mu, double sigma);

#endif // LIB_H