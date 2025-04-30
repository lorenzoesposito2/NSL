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
    cout << "Search for optimal temperature" << endl;
  for(int i=0; i < 16; i++){
    double temp = 1.9 + i*0.01;
    System SYS;
    SYS.initialize(false);
    SYS.set_temp(temp);
    if(SYS.get_sim_type() == 0) sim_type = "NVE";
    else {
        cerr << "Error: search for optimal temperature can be used only with NVE." << endl;
        return 1;
    }

    path = "../OUTPUT/EQUILIBRATION/" + sim_type + "/TEMP_EQ/";
    
    SYS.initialize_properties(path);
    SYS.block_reset(0);

    cout << "Simulation at temperature : " << setprecision(2) <<  temp << endl;
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
    SYS.finalize(path);
    cout << endl;
  }
  cout << "Search completed!" << endl;
  return 0;
}