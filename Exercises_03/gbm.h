#ifndef __gbm_h__
#define __gbm_h__

#include "../Libraries/Parallel_Random_Number_Generator/random.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class gbm {
    private:
        double S_0; // initial asset price
        double mu; // drift
        double sigma; // volatility
        double t=0.;
        double dt;
        Random rand_gen;

    public:
        gbm(double S_0_, double mu_, double sigma_, const char* seed_path, const char* primes_path);
        ~gbm() {};

        // set/get methods
        void set_t(double t_){t = t_;};
        void set_dt(double dt_){dt = dt_;};
        void set_S_0(double S_0_){S_0 = S_0_;};
        void set_mu(double mu_){mu = mu_;};
        void set_sigma(double sigma_){sigma = sigma_;};
        double get_S_0(){return S_0;};
        double get_mu(){return mu;};
        double get_sigma(){return sigma;};
        double get_t(){return t;};  
        double get_dt(){return dt;};
        double get_S(){return S_0;};

        void reset(double S_0_);
        void update();
       

    

};


#endif