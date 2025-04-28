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

int main (int argc, char *argv[]){
  if(argc != 2){
    cerr << "Usage: \n-Ex4.1 \n-Ex4.2 \n-Ex4.3" << endl;
    return 1;
  }
  if(string(argv[1]) != "Ex4.1" && string(argv[1]) != "Ex4.2" && string(argv[1]) != "Ex4.3"){
    cerr << "PROBLEM: invalid argument" << endl;
    cerr << "Usage: \n-Ex4.1 \n-Ex4.2 \n-Ex4.3" << endl;
    return 1;
  }

  // Ex4.1 ////////////////////////////////////////////////////////////
  if(string(argv[1]) == "Ex4.1"){
   
  int nconf = 1;
  System SYS;
  SYS.initialize();
  SYS.initialize_properties();
  SYS.block_reset(0);

  for(int i = 0; i < SYS.get_nbl(); i++){
    
    float progress = (float)(i + 1) / SYS.get_nbl() * 100;

  
    cout << "\rProgress: [";
    int bar_width = 50; 
    int pos = bar_width * progress / 100;
    for (int j = 0; j < bar_width; ++j) {
      if (j < pos) cout << "=";
      else if (j == pos) cout << ">";
      else cout << " ";
    }
    cout << "] " << fixed << setprecision(1) << progress << "%";
    cout.flush();


    for(int j = 0; j < SYS.get_nsteps(); j++){ 
      SYS.step();
      SYS.measure();
      if(j % 50 == 0){
        // SYS.write_XYZ(nconf); // Write actual configuration in XYZ format
        nconf++;
      }
    }
    SYS.averages(i + 1);
    SYS.block_reset(i + 1);
  }
  SYS.finalize();
  }

  // Ex4.2 ////////////////////////////////////////////////////////////
  else if(string(argv[1]) == "Ex4.2"){
  int nconf = 1;
  System SYS;
  SYS.initialize();
  SYS.initialize_properties();
  SYS.block_reset(0);

  for(int i = 1; i < SYS.get_nbl(); i++){
    
    float progress = (float)(i + 1) / SYS.get_nbl() * 100;

  
    cout << "\rProgress: [";
    int bar_width = 50; 
    int pos = bar_width * progress / 100;
    for (int j = 0; j < bar_width; ++j) {
      if (j < pos) cout << "=";
      else if (j == pos) cout << ">";
      else cout << " ";
    }
    cout << "] " << fixed << setprecision(1) << progress << "%";
    cout.flush();


    for(int j = 0; j < SYS.get_nsteps(); j++){ 
      SYS.step();
      SYS.measure();
      if(j % 50 == 0){
        // SYS.write_XYZ(nconf); // Write actual configuration in XYZ format
        nconf++;
      }
    }
    SYS.averages(i + 1);
    SYS.block_reset(i + 1);
  }
  SYS.finalize();
  }

  // Ex4.3 ////////////////////////////////////////////////////////////
  else if(string(argv[1]) == "Ex4.3"){
    System SYS;
  SYS.initialize();
  SYS.initialize_properties();
  SYS.block_reset(0);
  int total_blocks = SYS.get_nbl();
 
  cout << "forward simulation" << endl;

  for(int i = 0; i < total_blocks; i++){
    
    float progress = (float)(i + 1) / total_blocks * 100;

    cout << "\rProgress: [";
    int bar_width = 50; 
    int pos = bar_width * progress / 100;
    for (int j = 0; j < bar_width; ++j) {
      if (j < pos) cout << "=";
      else if (j == pos) cout << ">";
      else cout << " ";
    }
    cout << "] " << fixed << setprecision(1) << progress << "%";
    cout.flush();


    for(int j = 0; j < SYS.get_nsteps(); j++){ 
      SYS.step();
      SYS.measure();
    }
    SYS.averages(i + 1);
    SYS.block_reset(i + 1);
  }
   
  // backward simulation

  SYS.invert_velocities();
  SYS.reset_properties();
  SYS.block_reset(0);

  cout << "\nbackward simulation" << endl;
  for(int i = 51; i <= 2*total_blocks; i++){
    
    float progress = (float)(i + 1) / total_blocks * 100;

    cout << "\rProgress: [";
    int bar_width = 50; 
    int pos = bar_width * progress / 100;
    for (int j = 0; j < bar_width; ++j) {
      if (j < pos) cout << "=";
      else if (j == pos) cout << ">";
      else cout << " ";
    }
    cout << "] " << fixed << setprecision(1) << progress << "%";
    cout.flush();


    for(int j = 0; j < SYS.get_nsteps(); j++){ 
      SYS.step();
      SYS.measure();
    }
    SYS.averages(i + 1);
    SYS.block_reset(i + 1);
  }
  SYS.finalize();

  }
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
