#ifndef __rw_h__
#define __rw_h__

#include "../Libraries/Parallel_Random_Number_Generator/random.h"
#include <string>
#include <cmath>
#include <iostream>
#include <fstream>

using namespace std;

class RW {
    protected:
    int n_steps;
    int a = 1;
    double pos[3];
    Random rand_gen;

    public:
    RW(int n_steps, double pos[3]);
    ~RW();

    void restart(){for(int i=0; i<3; i++){pos[i] = 0;}};

    void set_n_steps(int n_steps){this->n_steps = n_steps;};
    int get_n_steps(){return n_steps;};
    void set_pos(double pos[3]){for(int i=0; i<3; i++){this->pos[i] = pos[i];}};
    double * get_pos(){return pos;};
    void set_a(int a){this->a = a;};
    int get_a(){return a;};
    void set_rand_gen(Random rand_gen){this->rand_gen = rand_gen;};
    Random get_rand_gen(){return rand_gen;};
    void solidAngle_sampling(int n, string path);

    //get x,y,z position
    double get_x(){return pos[0];};
    double get_y(){return pos[1];};
    double get_z(){return pos[2];};

    void update_discrete();
    void update_continuous();
};

#endif