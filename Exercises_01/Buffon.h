#ifndef __Buffon__
#define __Buffon__

#include "../Libraries/Parallel_Random_Number_Generator/random.h"
#include <stdexcept>

class Buffon {
    private:
    double L,d;
    int n_throws, n_blocks;
    Random rand_gen;

    public:
    
    Buffon();
    Buffon(double length, double distance, int throws, int blocks);

    ~Buffon() {}

    void setL(double l) { L = l; }
    void setD(double D) { d = D; }
    void setNThrows(int throws) { n_throws = throws; }
    void setNBlocks(int blocks) { n_blocks = blocks; }

    double getL() const { return L; }
    double getD() const { return d; }
    int getNThrows() const { return n_throws; }
    int getNBlocks() const { return n_blocks; }

    bool throwNeedle();
    void simulate(std::string filename);
};

#endif