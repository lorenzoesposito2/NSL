#include "Buffon.h"
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <string>
#include <utility>

Buffon::Buffon() : L(0.), d(0.1), n_throws(0), rand_gen(), n_blocks(0) {}

Buffon::Buffon(double length, double distance, int throws, int blocks) 
    : L(length), d(distance), n_throws(throws), n_blocks(blocks),
    rand_gen("../../Libraries/Parallel_Random_Number_Generator/Primes", "../../Libraries/Parallel_Random_Number_Generator/seed.in") {
        if(d < L){
            throw std::invalid_argument("Distance between lines must be greater than the length of the needle (d > L).");
        }
    }

    bool Buffon::throwNeedle() {
        // y position of the center of the needle uniformly between 0 and d
        double y_center = rand_gen.Rannyu(0., d); 
    
        // Sample orientation using rejection method on unit disk
        double x_dir, y_dir, norm;
        do {
            x_dir = rand_gen.Rannyu(-1., 1.); 
            y_dir = rand_gen.Rannyu(-1., 1.); 
            norm = sqrt(x_dir*x_dir + y_dir*y_dir);
        } while (norm >= 1.0 || norm == 0.0); 
    
        // Calculate the vertical projection of HALF the needle length
        double y_projection_half = (L / 2.0) * std::abs(y_dir / norm);
    
        // Calculate the minimum and maximum y coordinates reached by the needle
        double y_min = y_center - y_projection_half;
        double y_max = y_center + y_projection_half;
    
        // Verify if the needle crosses a line (y=0 or y=d)
        if (y_min <= 0.0 || y_max >= d) {
            return true; 
        }
        
        return false; 
    }

void Buffon::simulate(std::string filename) {
    std::ofstream out(filename);
    if(!out.is_open()){
        throw std::runtime_error("Error opening file.");
    }else{
        std::cout << "writing Buffon simulation data to file " << filename << std::endl;
        out << "#Blocks Estimate Error" << std::endl;
    }

    double sum = 0, sum2 = 0;
    double prog_sum = 0, prog_sum2 = 0;
    double error = 0;
    int n_hit = 0, n_tot = 0;

    for(int i=0; i<n_blocks; i++){
        n_hit = 0;
        n_tot = 0;
        for (int j=0; j<n_throws/n_blocks; j++){
            if(throwNeedle()){
                n_hit++;
            }
            n_tot++;
        }

        double pi_estimate = (2.0 * L * n_tot) / (n_hit * d);
        sum += pi_estimate;
        sum2 += pi_estimate * pi_estimate;

        prog_sum = sum / (i + 1);
        prog_sum2 = sum2 / (i + 1);
        if (i == 0) {
            error = 0;
        } else {
            error = sqrt((prog_sum2 - prog_sum * prog_sum) / i);
        }

        out << i + 1 << " " << prog_sum << " " << error << std::endl;
    }

    out.close();
}
