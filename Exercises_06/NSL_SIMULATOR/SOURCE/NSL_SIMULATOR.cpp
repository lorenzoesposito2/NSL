/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#include <iostream>
#include "system.h"
#include <iomanip>

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

int main(int argc, char *argv[]) {
  int nconf = 1;
    string sim_type;

    ofstream coutf("../OUTPUT/seed.out");
    coutf << "0000 0000 0000 0001" << endl;
    coutf.close();
    
    for (int k = 0; k <= 15; k++) {
        double temp = 0.5 + k * 0.1;
        System SYS;
        SYS.set_temp(temp);
        SYS.initialize(true);
        if(SYS.get_sim_type() == 2) sim_type = "METROPOLIS";
        else if(SYS.get_sim_type() == 3) sim_type = "GIBBS";
        else {
            cerr << "Error: Invalid simulation type." << endl;
            return 1;
        }
        cout << "1D Ising simulation at temperature: " << temp << " using " << sim_type << endl;
        SYS.initialize_properties("../OUTPUT/" + sim_type + "/");
        SYS.block_reset(0);

        // equilibration
        //SYS.equilibration("../OUTPUT/EQUILIBRATION/" + sim_type, 1000);
        //SYS.block_reset(0);

        for (int i = 0; i < SYS.get_nbl(); i++) {
            print_progress_bar((float)(i + 1) / SYS.get_nbl() * 100.0);
            for (int j = 0; j < SYS.get_nsteps(); j++) {
                SYS.step();
                SYS.measure();
                
                if (j % 50 == 0) {
                    nconf++;
                }
            }

            SYS.averages(i + 1, "../OUTPUT/" + sim_type );
            SYS.block_reset(i + 1);
        }

        SYS.finalize();
        cout << endl;
    }

  cout << "Simulation completed!" << endl;
  return 0;
}

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
