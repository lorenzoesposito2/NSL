#include <iostream>
#include <fstream>
#include <iomanip>
#include "random.h"
#include "BA.h"
#include "lib.h"

using namespace std;

void print_progress_bar(float progress) {
    int bar_width = 50;
    int pos = bar_width * progress / 100.0;
  
    cout << "\rProgress: [";
    for (int j = 0; j < bar_width; ++j) {
        if (j < pos) cout << "=";
        else if (j == pos) cout << ">";
        else cout << " ";
    }
    cout << "] " << fixed << setprecision(1) << progress << "%";
    cout.flush();
}

int main(){

    Random rnd("../../Libraries/Parallel_Random_Number_Generator/Primes", "../../Libraries/Parallel_Random_Number_Generator/seed.in");

    // set parameters
    double mu = 1.; double sigma = 1.;
    double T, beta;                 
    double T0 = 1.;
    double mu_old = mu, sigma_old = sigma;
    double mu_best, sigma_best;
    double H_new = 0., H_old = 0., H_best = H_new;
    double H_err = 0., H_err_best = 0.;
    double p_accept;
    double delta_mu = 0.5, delta_sigma = 0.5;
    double d_mu, d_sigma;
    int counter = 1;
    int SA_steps = 1000;
    int n = 100000;
    int n_blocks = 150;

    ofstream output("../data/T_vs_H.dat");
    if (!output.is_open()){
        cerr << "Error opening file" << endl;
        return 1;
    }
    ofstream out("../data/mu_sigma.dat");
    if (!out.is_open()){
        cerr << "Error opening file" << endl;
        return 1;
    }
    output << "step T H err" << endl;
    out << "step mu sigma" << endl;

    // simulated annealing
    for (int i = 0; i < SA_steps; i++){
        print_progress_bar((float)i/SA_steps*100);
        // New lower temperature
        beta = i+1;
        T = 1./beta;

        // Mu and sigma steps must be smaller than the old ones
        d_mu = d_mu > mu_old ? mu_old : delta_mu;
        d_sigma = d_sigma > sigma_old ? sigma_old : delta_sigma;

        // Gaussian step in parameter space
        mu = fabs(mu_old + rnd.Gauss(0.,d_mu*T/T0));
        sigma = fabs(sigma_old + rnd.Gauss(0.,d_sigma*T/T0));

        out << i << " " << mu << " " << sigma << endl;


        // Evaluate H with new parameters
        double * samples = metropolis(psi_trial_abs, rnd, 0.0, 3.0, n, mu, sigma);
        auto bound_function = bind(H_psi_frac_psi, placeholders::_1, mu, sigma);
        BA_integral mean_H(n_blocks, n, bound_function, [](double x) { return 1.0; });
        auto result = mean_H.compute(samples);
        double H_new = result.first;
        double H_err = result.second;
        output <<  i << " " << T << " " << H_new << " "<< H_err << endl;
        delete[] samples;

        // Accept or reject new parameters
        p_accept = min(1., exp(-beta*(H_new - H_old)));
        if (rnd.Rannyu() < p_accept){
            mu_old = mu;
            sigma_old = sigma;
            if(H_new < H_best){
                H_old = H_new;
                H_best = H_new;
                H_err_best = H_err;
                mu_best = mu;
                sigma_best = sigma;
            }
        } else {
            mu = mu_old;
            sigma = sigma_old;
        }

    }
    
    cout << "Final parameters: " << endl;
    cout << fixed << setprecision(4);
    cout << "- mu: " << mu << endl;
    cout << "- sigma: " << sigma << endl;

    output.close();
    out.close();

    //evaluate H with best parameters
    cout << "Evaluating H with best parameters: " << endl;
    double * samples = metropolis(psi_trial_abs, rnd, 0.0, 3.0, n*100, mu_best, sigma_best);
    auto bound_function = bind(H_psi_frac_psi, placeholders::_1, mu_best, sigma_best);
    BA_integral mean_H(n_blocks, n*100, bound_function, [](double x) { return 1.0; });
    mean_H.compute(samples, "../data/mean_H_best.dat");

    //sample psi with best parameters
    ofstream output_psi("../data/psi_best_sample.dat");
    if (!output_psi.is_open()){
        cerr << "Error opening file" << endl;
        return 1;
    }
    for (int i = 0; i < n*100; i++){
        output_psi << samples[i] << endl;
    }
    output_psi.close();
    delete[] samples;
    
    return 0;
}