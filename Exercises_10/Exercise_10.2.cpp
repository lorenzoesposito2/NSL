#include <mpi.h>
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


int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Temperatures and inverse temperatures
    vector<double> temp(size), beta(size);
    double T_max = 5.0;
    for (int i = 0; i < size; ++i) {
        temp[i] = T_max / pow(2.0, i);
        beta[i] = 1.0 / temp[i];
    }
    cout << "Rank " << rank << " has temperature T = " << temp[rank] << endl; 

    // RNG and TSP setup
    Random rnd("../Libraries/Parallel_Random_Number_Generator/Primes", 
               "../Libraries/Parallel_Random_Number_Generator/seed.in");
    int n_cities = 110;
    trip current(n_cities, "cap_prov_ita.dat", rnd);
    trip best = current;
    
    double f_current = current.distance();
    double f_best = f_current;
    int SA_steps = 500000;

    ofstream loss_out;
    if (rank == size - 1) {
        loss_out.open("data/parallel_tempering.dat");
        loss_out << "step T fitness\n";
    }

    // Parallel Tempering loop
    for (int step = 1; step <= SA_steps; ++step) {
        // Simulated Annealing step
        trip trial = current;
        double r = rnd.Rannyu();
        if (r < 0.3) {
            trial.pair_permutation(rnd);
        } else if (r < 0.6) {
            int m = rnd.Rannyu(1, n_cities - 2);
            int n = rnd.Rannyu(1, n_cities - 2);
            trial.shift_mutation(rnd, m, n);
        } else if (r < 0.9) {
            int m = rnd.Rannyu(1, n_cities/2 - 1);
            trial.m_permutation(rnd, m);
        } else {
            trial.inversion();
        }
        trial.check();

        // acceptance criterion
        double f_trial = trial.distance();
        double p_acc = min(1.0, exp(-beta[rank] * (f_trial - f_current)));
        if (rnd.Rannyu() < p_acc) {
            current = trial;
            f_current = f_trial;
            if (f_current < f_best) {
                f_best = f_current;
                best = current;
            }
        }

        // Output loss for the last rank
        if (rank == size - 1) {
            loss_out << step << " " << temp[rank] << " " << f_current << "\n";
        }

        if (rank != size - 1) {
            // send fitness value and trip to next rank
            double f_send = best.distance();
            vector<int> path = best.get_path();
            MPI_Send(&f_send, 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
            MPI_Send(path.data(), path.size(), MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        }

        if (rank != 0) {
            // receive fitness value and trip from previous rank
            double f_recv;
            MPI_Recv(&f_recv, 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            vector<int> path_recv(n_cities);
            MPI_Recv(path_recv.data(), n_cities, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            // create new trip from received path
            trip received_trip(n_cities, "cap_prov_ita.dat", rnd);
            received_trip.set_path(path_recv);
            double f_received = received_trip.distance();

            // Compare and possibly update best trip
            double delta = beta[rank] - beta[rank - 1] * (f_received - f_current);
            if (delta > 0 || rnd.Rannyu() < exp(delta)) {
                current = received_trip;
                f_current = f_received;
                if (f_current < f_best) {
                    f_best = f_current;
                    best = current;
                }
            }
        }

        if (rank == 0) print_progress_bar(100.0 * step / SA_steps);
    }

    // Trova il best globale tra tutti i processi
    struct {
        double value;
        int rank;
    } local, global;
    local.value = f_best;
    local.rank = rank;
    MPI_Allreduce(&local, &global, 1, MPI_DOUBLE_INT, MPI_MINLOC, MPI_COMM_WORLD);

    vector<int> global_path(n_cities);
    if (rank == global.rank) {
        global_path = best.get_path();
    }
    // Diffondi il best path globale a tutti
    MPI_Bcast(global_path.data(), n_cities, MPI_INT, global.rank, MPI_COMM_WORLD);

    if (rank == 0) {
        ofstream best_out("data/parallel_tempering_best_path.dat");
        if (!best_out.is_open()) {
            cerr << "Error opening file for best path output." << endl;
            return 1;
        }
        best_out << "Best path found with fitness: " << global.value << "\n";
        for (int id : global_path) {
            best_out << id << " ";
        }
        best_out << endl;
        best_out.close();
        cout << "Best path fitness: " << global.value << endl;
    }

    if (loss_out.is_open()) loss_out.close();
    MPI_Finalize();
    return 0;
}