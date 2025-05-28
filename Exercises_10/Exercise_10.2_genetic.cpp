#include <iostream>
#include <fstream>
#include <iomanip>
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


int main() {
    
    int n_cities = 110;
    int n_population = 500;
    int n_gen = 10000;
    Random rnd("../Libraries/Parallel_Random_Number_Generator/Primes", "../Libraries/Parallel_Random_Number_Generator/seed.in");
    string path = "cap_prov_ita.dat";

    GA ga(n_cities, n_population, path);

    ofstream out("data/best_loss_genetic.dat");
    if (!out.is_open()) {
        cerr << "Error opening file: data/best_loss_genetic.dat" << endl;
        return 1;
    }
    out << "Generation Loss" << endl;
    out.close();

    
    ofstream out_trip("data/best_trip_genetic.dat");
    if (!out_trip.is_open()) {
        cerr << "Error opening file: /data/best_trip_genetic.dat" << endl;
        return 1;
    }
    out_trip << "Generation Loss Trip" << endl;
    out_trip.close();
    
    // Genetic Algorithm
    for (int i = 0; i < n_gen; i++) {
        multimap<double, trip> new_population;
        // write best trip
        ga.save_trip(0, i, "data/best_trip_genetic.dat");
        for (int j = 0; j < n_population/2; j++) {

            float progress = 100.0f * (i * (n_population/2) + j + 1) / (n_gen * (n_population/2));
            print_progress_bar(progress);
            
            // t1 and t2 are a copy to insert into new population
            trip t1 = ga.select_individual(2.0);
            trip t2 = ga.select_individual(2.0);
            
            if (rnd.Rannyu() < 0.80) ga.crossover(t1, t2);

            
            double prob = rnd.Rannyu();
            int m_shift = int(rnd.Rannyu(1, n_cities - 2));
            int n_shift = int(rnd.Rannyu(1, n_cities - 2));
            int m_perm = int(rnd.Rannyu(1, n_cities/2 - 1));
            if (prob < 0.1) t1.pair_permutation(rnd);
            else if (prob < 0.2) t1.shift_mutation(rnd, m_shift, n_shift);
            else if (prob < 0.3) t1.m_permutation(rnd, m_perm);
            else if (prob < 0.35) t1.inversion();

            prob = rnd.Rannyu();
            m_shift = int(rnd.Rannyu(1, n_cities - 2));
            n_shift = int(rnd.Rannyu(1, n_cities - 2));
            m_perm = int(rnd.Rannyu(1, n_cities/2-1));
            if (prob < 0.1) t2.pair_permutation(rnd);
            else if (prob < 0.2) t2.shift_mutation(rnd, m_shift, n_shift);
            else if (prob < 0.3) t2.m_permutation(rnd, m_perm);
            else if (prob < 0.35) t2.inversion();

            new_population.insert({t1.distance(), t1});
            new_population.insert({t2.distance(), t2});
        }
        ga.population = new_population;
        ga.write_loss(i, "data/best_loss_genetic.dat");
        
    }  

    return 0;
}


