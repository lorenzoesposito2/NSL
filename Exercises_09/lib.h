#ifndef LIB_H
#define LIB_H

#include <vector>
#include <map>
#include <string>
#include "random.h"

using namespace std;

struct city {
    int id;
    double x, y;
    city() : id(0), x(0.), y(0.) {}
    city(double x, double y, int id);
};

class trip {
public:
    trip() : _n_cities(0) {}; // default constructor
    trip(int n_cities, string path, Random& rnd); // constructor with file input
    int _n_cities;
    vector<city> cities;
    double distance();
    void swap(int i, int j);
    void print_trip(); 
    void check();
    vector<int> get_path() {
        vector<int> path;
        for (const auto& c : cities) {
            path.push_back(c.id);
        }
        return path;
    }

    void set_path(const vector<int>& path) {
        if (path.size() != _n_cities) {
            throw std::invalid_argument("Path size does not match number of cities.");
        }
        for (int i = 0; i < _n_cities; ++i) {
            cities[i].id = path[i];
        }
    }
    

    // mutations
    void pair_permutation(Random& rnd);
    void shift_mutation(Random& rnd, int m, int n); 
    void m_permutation(Random& rnd, int m);
    void inversion();
};

class GA {
private:
    Random rnd;

public:
    int _l_cities;
    int _n_population;
    multimap<double, trip> population;

    GA(int l_cities, int n_population, bool circle_config);
    void print_population();
    void write_cities_config(vector<double> x, vector<double> y, string path);
    void check_trip(trip t);
    void save_trip(int i, int n_gen ,string path);
    void write_loss(int n_gen, string path);
    void write_mean_loss(int n_gen, string path);
    trip& get_trip(int i) {
        auto it = population.begin();
        advance(it, i);
        return it->second;
    }
    Random& get_random() {return rnd;};

    // selections
    trip select_individual(int p);
    trip select_individual_probabilistic();

    // crossover
    void crossover(trip& t1, trip& t2);

};

#endif // LIB_H