#ifndef __BA_H__
#define __BA_H__

#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <functional>
#include <utility>

using namespace std;

class BA {
    protected:

    int n_blocks, n_throws;
    double error(double *AV, double *AV2, int n);

    public: 

    BA(int n_blocks, int n_throws);
    ~BA();

    virtual void compute(double * rnd_data, string filename) = 0;

    //set functions
    void set_n_blocks(int n_blocks){this->n_blocks = n_blocks;};
    void set_n_throws(int n_throws){this->n_throws = n_throws;};

    //get functions
    int get_n_blocks(){return n_blocks;};
    int get_n_throws(){return n_throws;};
};

class BA_integral : public BA {
    private:
    // function to integrate
    function<double(double)> f;
    // denominator function for the importance sampling
    function<double(double)> den;

    public:
    BA_integral(int n_blocks, int n_throws, function<double(double)> f, function<double(double)> den) :
     BA(n_blocks, n_throws), f(f), den(den){};

    ~BA_integral(){};

    void compute(double * data, string filename) override; // compute the integral using block averaging
    void compute_importance_sampling(double * rnd_data, string filename); // compute the integral using block averaging and importance sampling
    pair<double, double> compute(double * data); // compute the integral and return the estimate and error
};

#endif