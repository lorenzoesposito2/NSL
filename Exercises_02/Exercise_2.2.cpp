#include "../Libraries/Parallel_Random_Number_Generator/random.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include "rw.h"

int main(){
    //##################### Point 1 #####################

    int n_trajec = 10000;
    int n_blocks = 100;
    int n_steps = 100;

    double pos[3] = {0, 0, 0};
    
    RW walker(n_steps, pos);
    
    double * cum_sum = new double[n_steps];
    double * cum_sum2 = new double[n_steps];
    double * error = new double[n_steps];
    
    for(int i=0; i<n_blocks;i++){
        double * dist = new double[n_steps]();
        for(int j=0; j<n_trajec/n_blocks; j++){  // loop on trajectories in each block
            walker.restart();
            for(int k=0; k<n_steps; k++){ // single trajectory
                dist[k] += (walker.get_x()*walker.get_x() + walker.get_y()*walker.get_y() + walker.get_z()*walker.get_z())/(n_trajec/n_blocks); // evaluate the mean at the k-th step
                walker.update_discrete(); // update the position of the walker
            }
            
        }

        for(int k=0; k<n_steps; k++){ // evaluate at each step the cumulative sum
            cum_sum[k] += sqrt(dist[k]); 
            cum_sum2[k] += dist[k];
        }
    }

    for(int i=0; i<n_steps; i++){
        cum_sum[i] /= (n_blocks);
        cum_sum2[i] /= (n_blocks);
        if(i==0){
            error[i] = 0;
        } else {
        error[i] = sqrt((cum_sum2[i] - cum_sum[i]*cum_sum[i])/n_blocks);
        }
    }   

    std::ofstream out("../data/3d_discrete_random_walk.dat");
    if (!out.is_open()) {
        std::cerr << "Errore nell'apertura del file " << "../data/3d_discrete_random_walk.dat" << std::endl;
        return 1;
    }
    out << "#step r error" << std::endl;
    std::cout << "writing 3d discrete random walk data to file " << "../data/3d_discrete_random_walk.dat" << std::endl;
    for(int i=0; i<n_steps; i++){
        out << i << " " << cum_sum[i] << " " << error[i] <<  std::endl;
    }
    out.close();


    //##################### Point 2 #####################

    // same as above but for continuous random walk

    double pos_cont[3] = {0, 0, 0};
    RW walker_cont(n_steps, pos_cont);

    double * cum_sum_cont = new double[n_steps];
    double * cum_sum2_cont = new double[n_steps];
    double * error_cont = new double[n_steps];
    
    for(int i=0; i<n_blocks;i++){
        double * dist_cont = new double[n_steps]();
        for(int j=0; j<n_trajec/n_blocks; j++){
            walker_cont.restart();
            for(int k=0; k<n_steps; k++){
                dist_cont[k] += (walker_cont.get_x()*walker_cont.get_x() + walker_cont.get_y()*walker_cont.get_y() + walker_cont.get_z()*walker_cont.get_z())/(n_trajec/n_blocks);
                walker_cont.update_continuous(); // the only difference is here
            }
            
        }

        for(int k=0; k<n_steps; k++){
            cum_sum_cont[k] += sqrt(dist_cont[k]); 
            cum_sum2_cont[k] += dist_cont[k];
        }
        
    }
    

    for(int i=0; i<n_steps; i++){
        cum_sum_cont[i] /= (n_blocks);
        cum_sum2_cont[i] /= (n_blocks);
        if(i==0){
            error_cont[i] = 0;
        } else {
        error_cont[i] = sqrt((cum_sum2_cont[i] - cum_sum_cont[i]*cum_sum_cont[i])/n_blocks);
        }
    }

    std::ofstream out2("../data/3d_continuous_random_walk.dat");
    if (!out2.is_open()) {
        std::cerr << "Errore nell'apertura del file " << "../data/3d_continuous_random_walk.dat" << std::endl;
        return 1;
    }
    out2 << "#step r error" << std::endl;
    std::cout << "writing 3d continuous random walk data to file " << "../data/3d_continuous_random_walk.dat" << std::endl;
    for(int i=0; i<n_steps; i++){
        out2 << i << " " << cum_sum_cont[i] << " " << error_cont[i] <<  std::endl;
    }
    out2.close();
    
    delete[] cum_sum;
    delete[] cum_sum2;
    delete[] error;

    delete[] cum_sum_cont;
    delete[] cum_sum2_cont;
    delete[] error_cont;
    
    /*
    // sampling solid angle
    int n = 2000;
    std::string path = "../data/solid_angle_sampling.dat";
    walker.solidAngle_sampling(n, path);
    std::cout << "writing solid angle sampling data to file " << path << std::endl;
    */

    return 0;

}

