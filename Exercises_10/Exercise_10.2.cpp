#include "mpi.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cmath>
#include "random.h"
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


int main(int argc, char* argv[]){
    MPI_Init(&argc,&argv);
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Temperatures
    vector<double> temp(size), beta(size);
    double T_max = 1.;
    for(int i=0; i<size; i++){
        temp[i] = T_max/(i+1);
        beta[i] = 1./temp[i];
    }
    cout << "Temperatura rank " << rank << ": " << temp[rank] << endl;

    // RNG e TSP
    Random rnd("../Libraries/Parallel_Random_Number_Generator/Primes", "../Libraries/Parallel_Random_Number_Generator/seed.in");
    int n_cities = 110;
    trip ita(n_cities, "cap_prov_ita.dat", rnd);
    trip best_trip = ita;
    trip appo_trip = ita;

    double fitness_new = ita.distance();
    double fitness_old = fitness_new;
    double fitness_best = fitness_new;
    int SA_steps = 10000;
    double p_accept;


    // Log del processo caldo
    if(rank == size-1){
        ofstream flog("data/parallel_tempering.dat");
        flog << "step T fitness\n";
    }

    // Parallel Tempering loop
    for(int i=0; i < SA_steps; i++){
        if(rank == 0) print_progress_bar(100. * i / SA_steps);
        appo_trip = ita;

        // mutation
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

        // Acceptance criteria
        // accept or reject new configuration
        p_accept = min(1., exp(-beta[rank]*(fitness_new - fitness_old)));
        if(rnd.Rannyu() < p_accept){
            // Accetta: aggiorna la soluzione corrente
            fitness_old = fitness_new;
            if(fitness_new < fitness_best){
                fitness_best = fitness_new;
                best_trip = ita;
            }
        } else{
            // Rifiuta: torna alla vecchia soluzione
            ita = appo_trip;
        }
        // Log the process
        if(rank == size-1) {
            ofstream flog("data/parallel_tempering.dat", ios::app);
            flog << i << " " << temp[rank] << " " << fitness_old << "\n";
        }
    
        ofstream fout("data/parallel_tempering_best_path.dat");
        vector<int> best_path = best_trip.get_path();
        for(size_t j = 0; j < best_path.size(); ++j){
            fout << best_path[j] << "\n";
        }
        fout.close();

        // parallel tempering: exchange solutions between ranks
        if (i % 100 == 0 && i > 0) {
            if (rank < size - 1) {
                // Send to the next rank
                MPI_Send(&fitness_best, 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
                MPI_Send(best_trip.get_path().data(), n_cities, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
            } else {
                // Receive from the previous rank
                double received_fitness;
                vector<int> received_path(n_cities);
                MPI_Recv(&received_fitness, 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(received_path.data(), n_cities, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                // Update if the received fitness is better
                if (received_fitness < fitness_best) {
                    fitness_best = received_fitness;
                    best_trip.set_path(received_path);
                }
            }
        }
       
    }
    
    if(rank == size -1) {
        cout << "\nBest fitness found by rank " << rank << ": " << fitness_best << endl;
    }

    MPI_Finalize();
    return 0;
}