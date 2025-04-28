#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "../Libraries/Parallel_Random_Number_Generator/random.h"

using namespace std;

double error(double *AV, double *AV2, int n){
    if (n == 0){
        return 0;
    } else {
        return sqrt((AV2[n] - pow(AV[n], 2))/n);
    }
}


double chiSq(double *obs, int n, int M){
    double sum = 0;
    for (int i=0; i<M; i++){
        sum += pow(obs[i] - n/M, 2);
    }
    return sum/(n/M);
}


int main(int argc, char *argv[]){

    int M = 1000000;    // Number of random numbers to generate
    int N = 100;        // Number of blocks
    int L = int(M/N);   // Number of random numbers in each block

    // initialize the random number generator
    Random rand_gen("../../Libraries/Parallel_Random_Number_Generator/Primes", "../../Libraries/Parallel_Random_Number_Generator/seed.in");
    
    double *r = new double[M];
    double *ave = new double[N]();  // mean value of the blocks
    double *av2 = new double[N]();  // mean value of the blocks squared
    double *sum_prog = new double[N](); // cumulative mean value
    double *su2_prog = new double[N](); // cumulative mean value squared
    double *err_prog = new double[N](); // cumulative error

    double *x = new double[N]; for (int i = 0; i < N; i++){x[i] = i;}
    
    // evaluate the mean value for each block
    cout << "Evaluating the mean value for each block \n" << endl;
    for(int i = 0; i < N; i++){  // cycle over the blocks
        double sum = 0;
        for (int j = 0; j < L; j++){   // cycle over the random numbers in the block
            int k = j + i*L;  
            r[k] = rand_gen.Rannyu();
            sum += r[k]; // sum of the random numbers in the block
        }   
        ave[i] = sum/L; // mean value of the block
    }

    for(int i=0; i<N; i++){
        for(int j=0; j<i+1; j++){
            sum_prog[i] += ave[j];
            su2_prog[i] += pow(ave[j], 2);
        }
        sum_prog[i] /= (i+1);
        su2_prog[i] /= (i+1);
        err_prog[i] = error(sum_prog, su2_prog, i);
    }

    // Write the results to a file
    cout << "Writing the results into file average.dat \n" << endl;
    ofstream output;
    output.open("../data/average.dat");
    if (output.is_open()){
        output << "x sum_prog err_prog" << endl;
        for (int i = 0; i < N; i++){
            output << x[i] << " " << sum_prog[i] << " " << err_prog[i] << endl;
        }
    } else cerr << "PROBLEM: Unable to open average.dat" << endl;
    output.close();

    //########################### Point 2 ###########################
    
    // reset the arrays
    fill(ave, ave + N, 0.0);
    fill(av2, av2 + N, 0.0);
    fill(sum_prog, sum_prog + N, 0.0);
    fill(su2_prog, su2_prog + N, 0.0);
    fill(err_prog, err_prog + N, 0.0);

    cout << "Evaluating the variance for each block \n" << endl;
    for (int i=0; i<N; i++){
        double sum = 0;
        for (int j=0; j<L; j++){
            int k = j + i*L;
            r[k] = pow(rand_gen.Rannyu() - 0.5, 2); 
            sum += r[k];
        }
        ave[i] = sum/L;
    }

    for(int i=0; i<N; i++){
        for(int j=0; j<i+1; j++){
            sum_prog[i] += ave[j];
            su2_prog[i] += pow(ave[j], 2);
        }
        sum_prog[i] /= (i+1);
        su2_prog[i] /= (i+1);
        err_prog[i] = error(sum_prog, su2_prog, i);
    }

    // Write the results to a file
    cout << "Writing the results into file variance.dat \n" << endl;
    output.open("../data/variance.dat");
    if (output.is_open()){
        output << "x sum_prog err_prog" << endl;
        for (int i = 0; i < N; i++){
            output << x[i] << " " << sum_prog[i] << " " << err_prog[i] << endl;
        }
    } else cerr << "PROBLEM: Unable to open variance.dat" << endl;
    output.close();

    //########################### Point 3 ###########################

    M = 100;
    int n_attempts = 1000;
    int n = 10000;

    double *chi2 = new double[N]();
    double *obs = new double[M]();


    cout << "Evaluating chi2 \n" << endl;
    for (int i=0; i<n_attempts; i++){
        fill(obs,obs+M,0.0); // reset the array to zero
        for (int j=0;j<n;j++){
            obs[int(rand_gen.Rannyu()*M)] += 1; // generate a random number and increment the corresponding bin
        }
        chi2[i] = chiSq(obs, n, M); // evaluate chi2
    }

    cout << "Writing the results into file chi2_1000.dat \n" << endl;
    // Write the results to a file
    output.open("../data/chi2_1000.dat");
    if (output.is_open()){
        output << "attempts chi2" << endl;
        for (int i = 0; i < n_attempts; i++){
            output << i << " " << chi2[i] << endl;
        }
    } else cerr << "PROBLEM: Unable to open chi2.dat" << endl;
    output.close();

    delete[] r;
    delete[] ave;
    delete[] av2;
    delete[] sum_prog;
    delete[] su2_prog;
    delete[] err_prog;
    delete[] x;
    delete[] chi2;
    delete[] obs;

    return 0;
}

