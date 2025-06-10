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
 
        System SYS;
        SYS.initialize(false);
        if(SYS.get_sim_type() == 2) sim_type = "METROPOLIS";
        else if(SYS.get_sim_type() == 3) sim_type = "GIBBS";
        else {
            cerr << "Error: Invalid simulation type." << endl;
            return 1;
        }
        cout << "equilibration simulation using " << sim_type << endl;
        SYS.initialize_properties("../OUTPUT/EQUILIBRATION/" + sim_type + "/");
        SYS.block_reset(0);


        for (int i = 0; i < SYS.get_nbl(); i++) {
            print_progress_bar((float)(i + 1) / SYS.get_nbl() * 100.0);
            for (int j = 0; j < SYS.get_nsteps(); j++) {
                SYS.step();
                SYS.measure();
            }
            SYS.averages(i + 1, "../OUTPUT/EQUILIBRATION" + sim_type );
            SYS.block_reset(i + 1);
        }

        SYS.finalize();
        cout << endl;
    
  cout << "Simulation completed!" << endl;
  return 0;
}