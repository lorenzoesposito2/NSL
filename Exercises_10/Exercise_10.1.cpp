#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include "lib.h"

using namespace std;

void print_progress_bar(float progress) {
    int bar_width = 50;
    int pos = bar_width * progress / 100.0;
  
    cout << "\rProgress: [";
    for (int j = 0; j < bar_width; ++j) {
        if (j < pos) cout << "=";
        else if (j == pos) cout << ">";
        else cout << " ";
    }
    cout << "] " << fixed << setprecision(1) << progress << "%";
    cout.flush();
}

int main(){

    Random rnd("../../Libraries/Parallel_Random_Number_Generator/Primes", "../../Libraries/Parallel_Random_Number_Generator/seed.in");
    int n_cities = 110;
    trip ita(n_cities, "../cap_prov_ita.dat", rnd);

    // parameters for annealing
    double T0 = 1.;
    double T, beta;
    double fitness_new = ita.distance(), fitness_old = fitness_new, fitness_best = fitness_new;
    double p_accept;
    int counter = 1;
    int SA_steps = 1000000;
    int mutation_type;
    trip appo_trip = ita;
    trip best_trip = ita;

    ofstream output("../data/tsp_annealing.dat");
    if (!output.is_open()){
        cerr << "Error opening file" << endl;
        return 1;
    }
    output << "step T fitness" << endl;

    // simulated annealing
    for(int i = 0; i < SA_steps; i++){
        print_progress_bar((float)i/SA_steps*100);
        
        // udpate lower temperature
        beta = i+1;
        T = 1./beta;
        appo_trip = ita;

        // a step in the parameter space corresponds to a genetic mutation
        double r = rnd.Rannyu();
        if (r < 0.3) {
            ita.pair_permutation(rnd);
        } else if (r < 0.6) {
            int m = int(rnd.Rannyu(1, n_cities - 2));
            int n = int(rnd.Rannyu(1, n_cities - 2));
            ita.shift_mutation(rnd, m, n);
        } else if (r < 0.9) {
            int m = int(rnd.Rannyu(1, n_cities/2 - 1));
            ita.m_permutation(rnd, m);
        } else {
            ita.inversion();
        }
        ita.check();

        fitness_new = ita.distance();
        output << i << " " << T << " " << fitness_new << endl;

        // accept or reject new configuration
        p_accept = min(1., exp(-beta*(fitness_new - fitness_old)));
        if(rnd.Rannyu() < p_accept){
            // Accetta la nuova soluzione
            fitness_old = fitness_new;
            if(fitness_new < fitness_best){
                fitness_best = fitness_new;
                best_trip = ita;
            }
        } else{
            // Rifiuta: torna alla vecchia soluzione
            ita = appo_trip;
        }
    }
    
    output.close();
    ofstream out("../data/tsp_best_path.dat");
    if (!out.is_open()){
        cerr << "Error opening file" << endl;
        return 1;
    }
    for (int i = 0; i < n_cities; i++){
        out << best_trip.cities[i].id << " ";
    }
    out << endl;
    cout << "\n Best fitness: " << best_trip.distance() << endl;
    out.close();
    return 0;
}