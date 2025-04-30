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
    string path;

    ofstream coutf("../OUTPUT/seed.out");
    coutf << "0000 0000 0000 0001" << endl;
    coutf.close();
    
    System SYS;
    SYS.initialize(false);
    if(SYS.get_sim_type() == 0) sim_type = "NVE";
    else if(SYS.get_sim_type() == 1) sim_type = "NVT";
    else {
        cerr << "Error: Invalid simulation type." << endl;
        return 1;
    }

    SYS.get_restart() ? path = "../OUTPUT/ARGON/" + sim_type + "/" : path = "../OUTPUT/ARGON/EQUILIBRATION/" + sim_type + "/";
    
    SYS.initialize_properties(path);
    SYS.block_reset(0);

    cout << "Simulation started: " << sim_type << endl;
    for (int i = 0; i < SYS.get_nbl(); i++) {
        print_progress_bar((float)(i + 1) / SYS.get_nbl() * 100.0);
        for (int j = 0; j < SYS.get_nsteps(); j++) {
            SYS.step();
            SYS.measure();
                
            if (j % 50 == 0) {
                //SYS.write_XYZ(nconf);
                nconf++;
            }
        }

        SYS.averages(i + 1, path);
        SYS.block_reset(i + 1);
    }
    if(SYS.get_sim_type() == 1) cout << "\nmetropolis acceptance ratio: " << (double)SYS.get_naccepted()/(double)SYS.get_nattempts() << endl;
    SYS.finalize(path);
    cout << endl;

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
