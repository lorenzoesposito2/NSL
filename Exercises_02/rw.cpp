#include "rw.h"
#include <cmath>

RW::RW(int n_steps, double pos[3]){
    this->n_steps = n_steps;
    for(int i=0; i<3; i++){
        this->pos[i] = pos[i];
    }
    rand_gen = Random("../../Libraries/Parallel_Random_Number_Generator/Primes", "../../Libraries/Parallel_Random_Number_Generator/seed.in");

}

RW::~RW(){}

void RW::update_discrete(){
    int dir = int(rand_gen.Rannyu(0,3)); // select random direction
    pos[dir] += (rand_gen.Rannyu() < 0.5) ? 1 : -1; // move forward or backward
}

void RW::update_continuous(){
    double phi = rand_gen.Rannyu(0, 2*M_PI);
    double theta = acos(1-2.*rand_gen.Rannyu());
    pos[0] += sin(theta)*cos(phi);
    pos[1] += sin(theta)*sin(phi);
    pos[2] += cos(theta);
}

void RW::solidAngle_sampling(int n, string path){
    ofstream out(path);
    if (!out.is_open()) {
        cerr << "Errore nell'apertura del file " << path << endl;
        return;
    }
    out << "x y z" << endl;
    for(int i=0; i<n; i++){
        double phi = rand_gen.Rannyu(0, 2*M_PI);
        double theta = acos(1-2.*rand_gen.Rannyu());
        out << sin(theta)*cos(phi) << " " << sin(theta)*sin(phi) << " " << cos(theta) << endl;
    }
    out.close();
}