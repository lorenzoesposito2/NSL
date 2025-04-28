#include <iostream>
#include <fstream>
#include <string>
#include "../Libraries/Parallel_Random_Number_Generator/random.h"

using namespace std;


int main(){
    //########################### Point 1 ###########################
    int n = 10000;
    Random rand_gen("../../Libraries/Parallel_Random_Number_Generator/Primes", "../../Libraries/Parallel_Random_Number_Generator/seed.in");

    // Write the results to a file
    cout << "Writing the results into file exponential.dat and lorentz.dat \n" << endl;
    ofstream output;
    output.open("../data/exponential.dat");
    if (output.is_open()){
        output << "exp" << endl;
        for (int i = 0; i < n; i++){
            output << rand_gen.Exponential() << endl;
        }
    } else cerr << "PROBLEM: Unable to open exponential.dat" << endl;
    output.close();

    output.open("../data/lorentz.dat");
    if (output.is_open()){
        output << "lorentz" << endl;
        for (int i = 0; i < n; i++){
            output << rand_gen.Lorentz() << endl;
        }
    } else cerr << "PROBLEM: Unable to open lorentz.dat" << endl;
    output.close();

    //########################### Point 2 ###########################

    const int realizations = 10000;  // 10^4 realizzazioni
    const int N[4] = {1, 2, 10, 100};

    double s_std[4], s_exp[4], s_cauchy[4];

    ofstream std_output("../data/std_dice.dat");
    ofstream exp_output("../data/exp_dice.dat");
    ofstream cauchy_output("../data/cauchy_dice.dat");

    if (!std_output.is_open() || !exp_output.is_open() || !cauchy_output.is_open()) {
        cerr << "Error in opening output files" << endl;
        return 1;
    }
    std_output << "N1 N2 N10 N100" << endl;
    exp_output << "N1 N2 N10 N100" << endl;
    cauchy_output << "N1 N2 N10 N100" << endl;

    cout << "writing results in files std_dice.dat, exp_dice.dat and cauchy_dice.dat" << endl;

    for (int i = 0; i < realizations; i++) {
        for (int k = 0; k < 4; k++) {
            double sum_std = 0.0, sum_exp = 0.0, sum_cauchy = 0.0;
            for (int j = 0; j < N[k]; j++) {
                sum_std += rand_gen.Rannyu();
                sum_exp += rand_gen.Exponential();
                sum_cauchy += rand_gen.Lorentz(0.0, 1.0);
            }
            s_std[k]   = sum_std / N[k];
            s_exp[k]   = sum_exp / N[k];
            s_cauchy[k] = sum_cauchy / N[k];
        }

        for (int k = 0; k < 4; k++) {
            std_output << s_std[k]   << (k < 3 ? " " : "\n");
            exp_output << s_exp[k]   << (k < 3 ? " " : "\n");
            cauchy_output << s_cauchy[k] << (k < 3 ? " " : "\n");
        }
    }

    std_output.close();
    exp_output.close();
    cauchy_output.close();

    return 0;
}