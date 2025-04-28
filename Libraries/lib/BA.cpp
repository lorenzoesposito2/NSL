#include "BA.h"

BA::BA(int n_blocks, int n_throws) : n_blocks(n_blocks), n_throws(n_throws) {}

BA :: ~BA(){}

void BA_integral::compute(double * data, string filename) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "Errore nell'apertura del file " << filename << std::endl;
        return;
    }

    double *ave = new double[n_blocks]();
    double *sum_prog = new double[n_blocks]();
    double *sum2_prog = new double[n_blocks]();
    double *err = new double[n_blocks](); 
    
    double sum, sum2;

    std::cout << "writing integral data to file " << filename << std::endl;
    out << "#Blocks Estimate Error" << std::endl;

    for (int i = 0; i < n_blocks; ++i) {
        sum = 0.0;
        for (int j = 0; j < n_throws/n_blocks; ++j) {
            int k = j + i * n_throws/n_blocks;
            sum += f(data[k]);
        }
        ave[i] = sum / (n_throws/n_blocks);
    }
    
    for(int i=0; i<n_blocks; i++){
        for(int j=0; j<i+1; j++){
            sum_prog[i] += ave[j];
            sum2_prog[i] += ave[j] * ave[j];
        }
        sum_prog[i] /= (i+1);
        sum2_prog[i] /= (i+1);
        err[i] = error(sum_prog, sum2_prog, i);
        out << i+1 << " " << sum_prog[i] << " " << err[i] << std::endl;
    }

    out.close();

    delete[] ave;
    delete[] sum_prog;
    delete[] sum2_prog;
    delete[] err;

}

void BA_integral::compute_importance_sampling(double * data, string filename) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "Errore nell'apertura del file " << filename << std::endl;
        return;
    }

    double *ave = new double[n_blocks]();
    double *ave2 = new double[n_blocks]();
    double *sum_prog = new double[n_blocks]();
    double *sum2_prog = new double[n_blocks]();
    double *err = new double[n_blocks](); 
    
    double sum, sum2;

    std::cout << "writing integral data to file " << filename << std::endl;
    out << "#Blocks Estimate Error" << std::endl;

    for (int i = 0; i < n_blocks; ++i) {
        sum = 0.0;
        for (int j = 0; j < n_throws/n_blocks; ++j) {
            int k = j + i * n_throws/n_blocks;
            sum += f(data[k])/den(data[k]);
        }
        ave[i] = sum / (n_throws/n_blocks);
        ave2[i] = ave[i] * ave[i];

    }
    
    for(int i=0; i<n_blocks; i++){
        for(int j=0; j<i+1; j++){
            sum_prog[i] += ave[j];
            sum2_prog[i] += ave2[j];
        }
        sum_prog[i] /= (i+1);
        sum2_prog[i] /= (i+1);
        if(i==0){
            err[i] = 0;
        } else {
            err[i] = error(sum_prog, sum2_prog, i);
        }
        out << i+1 << " " << sum_prog[i] << " " << err[i] << std::endl;
    }

    out.close();

    delete[] ave;
    delete[] ave2;
    delete[] sum_prog;
    delete[] sum2_prog;
    delete[] err;
    
}
