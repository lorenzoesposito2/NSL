#include <mpi.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cmath>
#include "lib.h"
#include "random.h"

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
    double T_min = 0.01;
    double log_Tmax = log(T_max);
    double log_Tmin = log(T_min);
    for (int i = 0; i < size; ++i) {
        temp[i] = exp(log_Tmax - (log_Tmax - log_Tmin) * i / (size - 1));
       //temp[i] = abs(T_max - (T_max - T_min) * i / (size - 1));
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
    int SA_steps = 5000000;

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
        if (r < 0.35) {
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

        
        // every 1000 steps, swap attempt
        if (step % 1000 == 0) {
            int partner = -1;
            // partner selection 
            // use a even-odd pattern to avoid deadlocks
            if (((step/500) % 2) == 0) {
                // if rank is even, partner is next rank, if odd, partner is previous rank
                if (rank % 2 == 0 && rank < size - 1) partner = rank + 1;
                else if (rank % 2 == 1) partner = rank - 1;
            } else {
                if (rank % 2 == 1 && rank < size - 1) partner = rank + 1;
                else if (rank % 2 == 0 && rank > 0) partner = rank - 1;
            }

            // Ensure partner is within bounds
            if (partner >= 0 && partner < size) {
                // fitness exchange
                double send_f = f_current, recv_f;
                MPI_Request reqs[2]; // Isend and Irecv are non-blocking so we need requests
                MPI_Isend(&send_f, 1, MPI_DOUBLE, partner, 0, MPI_COMM_WORLD, &reqs[0]);
                MPI_Irecv(&recv_f, 1, MPI_DOUBLE, partner, 0, MPI_COMM_WORLD, &reqs[1]);
                MPI_Waitall(2, reqs, MPI_STATUSES_IGNORE); // wait for both sends and receives

                // swap probability calculation
                double delta = (beta[partner] - beta[rank]) * (recv_f - f_current);
                double p_swap = min(1.0, exp(delta));
                int do_swap = (rnd.Rannyu() < p_swap) ? 1 : 0;
                int partner_swap;

                // Sendrecv avoids deadlocks by sending and receiving in one call
                MPI_Sendrecv(&do_swap, 1, MPI_INT, partner, 1,
                     &partner_swap, 1, MPI_INT, partner, 1,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                // if swap is accepted, exchange paths
                if (do_swap && partner_swap) {
                    vector<int> send_path = current.get_path();
                    vector<int> recv_path(n_cities);
                    MPI_Request reqs2[2];
                    MPI_Isend(send_path.data(), n_cities, MPI_INT, partner, 2, MPI_COMM_WORLD, &reqs2[0]);
                    MPI_Irecv(recv_path.data(), n_cities, MPI_INT, partner, 2, MPI_COMM_WORLD, &reqs2[1]);
                    MPI_Waitall(2, reqs2, MPI_STATUSES_IGNORE);

                    current.set_path(recv_path);
                    f_current = recv_f;
                    if (f_current < f_best) {
                    f_best = f_current;
                    best = current;
                    }
                }
            }
        }

        if (rank == 0) print_progress_bar(100.0 * step / SA_steps);
    }

    // Find the best path across all ranks
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
    // Broadcast the best path to all ranks
    MPI_Bcast(global_path.data(), n_cities, MPI_INT, global.rank, MPI_COMM_WORLD);


    // Output the best path found by rank 0
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
        cout << endl;
        cout << "Best path fitness: " << global.value << endl;
    }

    if (loss_out.is_open()) loss_out.close();
    MPI_Finalize();
    return 0;
}