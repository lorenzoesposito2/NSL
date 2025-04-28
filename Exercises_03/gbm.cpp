#include "gbm.h" 
#include <cmath>   

gbm::gbm(double S_0_, double mu_, double sigma_, const char* seed_path, const char* primes_path) 
    : S_0(S_0_), mu(mu_), sigma(sigma_), rand_gen(primes_path, seed_path) {
    t = 0.0; 
}

void gbm::reset(double S_0_){
    t = 0.;
    S_0 = S_0_;
}

void gbm::update(){
    double z = rand_gen.Gauss(0., 1.);
    S_0 = S_0*exp((mu - 0.5*sigma*sigma)*dt + sigma*sqrt(dt)*z);
    t += dt;
}