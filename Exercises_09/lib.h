#ifndef LIB_H
#define LIB_H

#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include "random.h"

using namespace std;

/*
struct city represents a city with its coordinates and an identifier.
*/
struct city {
    int id;
    double x, y;
    city() : id(0), x(0.), y(0.) {}
    city(double x, double y, int id);
};

/*
class trip represents a trip consisting of a sequence of cities.
It provides methods to calculate the total distance of the trip.
*/
class trip {
public:
    trip() : _n_cities(0) {}; // default constructor
    trip(int n_cities, string path, Random& rnd); // constructor with file input
    int _n_cities; // number of cities in the trip
    vector<city> cities; // vector of cities in the trip
    double distance(); // calculates the total distance of the trip
    void swap(int i, int j);
    void print_trip(); 
    void check(); // checks if the trip is valid (all cities are unique and in correct order)
    
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
    vector<city> new_cities;
    for (int id : path) {
        // find the city with the given id
        auto it = std::find_if(cities.begin(), cities.end(), [id](const city& c){ return c.id == id; });
        if (it == cities.end()) throw std::runtime_error("City id not found in set_path");
        new_cities.push_back(*it);
    }
    cities = new_cities;
    }
    

    // mutations
    void pair_permutation(Random& rnd);
    void shift_mutation(Random& rnd, int m, int n); 
    void m_permutation(Random& rnd, int m);
    void inversion();
};

/*
class GA represents a Genetic Algorithm for solving the Traveling Salesman Problem (TSP).
*/
class GA {
private:
    Random rnd;

public:
    int _l_cities; // length of the cities vector
    int _n_population; // number of individuals in the population
    multimap<double, trip> population; // population of trips, sorted by distance

    GA(int l_cities, int n_population, bool circle_config); // constructor for circle and square configuration
    GA(int l_cities, int n_population, string path); // constructor for file input
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