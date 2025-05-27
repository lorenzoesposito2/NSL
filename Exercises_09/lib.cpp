#include "lib.h"
#include <cmath>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <random>

using namespace std;

city::city(double x, double y, int id) : id(id),x(x), y(y) {}


trip::trip(int n_cities, string path, Random& rnd) {
    _n_cities = n_cities;
    int count = 0;
    cities.resize(n_cities);
    for (int i = 0; i < n_cities; i++) {
        cities[i].id = i;
        cities[i].x = 0;
        cities[i].y = 0;
    }
    // Read city positions from file
    ifstream in(path);
    if (!in) {
        cerr << "Error opening file: " << path << endl;
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n_cities; i++) {
        in >> cities[i].x >> cities[i].y;
        count++;
    }
    in.close();
    if (count != n_cities) {
        cerr << "Error: Number of cities read from file does not match expected number." << endl;
        exit(EXIT_FAILURE);
    }
    // Check if the trip is valid
    check();
    // shuffle the cities
    shuffle(cities.begin() + 1, cities.end(), default_random_engine(rnd.Rannyu()*1e6));
    check();
}

double trip::distance() {
    double d = 0;
    for (int i = 0; i < _n_cities - 1; i++) {
        d += sqrt(pow(cities[i].x - cities[i+1].x, 2) + pow(cities[i].y - cities[i+1].y, 2));
    }
    // Add distance from last city to first city
    d += sqrt(pow(cities[_n_cities - 1].x - cities[0].x, 2) + pow(cities[_n_cities - 1].y - cities[0].y, 2));
    return d;
}

/*
swap two cities in the trip
*/
void trip::swap(int i, int j) {
    if (i < 0 || i >= _n_cities || j < 0 || j >= _n_cities) {
        cerr << "Error: Index out of bounds." << endl;
        return;
    }
    std::swap(cities[i], cities[j]);
}

/*
print the cities vector in order
*/
void trip::print_trip() {
    for (int i = 0; i < _n_cities; i++) {
        cout << cities[i].id << " ";
    }
    cout << endl;
}

void trip::check() {
    map<int, bool> visited;
    for (int i = 0; i < _n_cities; i++) {
        if (visited.find(cities[i].id) != visited.end()) {
            exit(EXIT_FAILURE);
        }
        visited[cities[i].id] = true;
    }
}

/*
perform a pair permutation mutation
*/
void trip::pair_permutation(Random& rnd) {
    int first_city_index = int(rnd.Rannyu(1, _n_cities));
    int second_city_index = int(rnd.Rannyu(1, _n_cities));
    while (first_city_index == second_city_index) {
        second_city_index = int(rnd.Rannyu(1, _n_cities));
    }
    swap(first_city_index, second_city_index);
}

/*
perform a shift mutation, moving a block of m cities to the right by n positions
*/
void trip::shift_mutation(Random& rnd, int m, int n) {
    if (m < 1 || m > _n_cities - 2) return;
    if (n < 1 || n > _n_cities - 2) return;

    int size = _n_cities - 1;
    int max_start = size - m + 1;
    if (max_start < 1) return;

    int start = int(rnd.Rannyu(1, max_start + 1));

    vector<city> temp(cities.begin() + 1, cities.end());
    vector<city> block(temp.begin() + (start - 1), temp.begin() + (start - 1 + m));
    temp.erase(temp.begin() + (start - 1), temp.begin() + (start - 1 + m));

    int insert_pos = (start - 1 + n) % (size - m + 1);
    temp.insert(temp.begin() + insert_pos, block.begin(), block.end());

    for (int i = 1; i < _n_cities; ++i) {
        cities[i] = temp[i - 1];
    }
}

/*
perform a m-permutation mutation, swapping two blocks of m cities
*/
void trip::m_permutation(Random& rnd, int m) {
    if (m < 1 || m >= _n_cities/2) return;
    int max_start = _n_cities - m;
    if (max_start < 2) return; // Non ci sono abbastanza blocchi distinti

    int start1 = int(rnd.Rannyu(1, max_start));
    int start2;
    int attempts = 0;
    const int max_attempts = 100;
    do {
        start2 = int(rnd.Rannyu(1, max_start));
        attempts++;
        if (attempts > max_attempts) return; // Evita loop infinito
    } while (abs(start1 - start2) < m);

    for (int i = 0; i < m; ++i) {
        swap(start1 + i, start2 + i);
    }
}

/*
perform an inversion mutation, reversing the order of cities between two indices
*/
void trip::inversion() {
    int start = 1;
    int end = _n_cities - 2;
    while (start < end) {
        swap(start, end);
        start++;
        end--;
    }
}

GA::GA(int l_cities, int n_population, bool circle_config)
    : rnd("../../Libraries/Parallel_Random_Number_Generator/Primes", "../../Libraries/Parallel_Random_Number_Generator/seed.in"),
      _l_cities(l_cities), _n_population(n_population) {

    // Check if the number of permutations (factorial) is bigger than the population size
    /*if(factorial(l_cities) < n_population) {
        cerr << "Error: The number of permutations is smaller than the population size." << endl;
        exit(EXIT_FAILURE);
    }*/  // No factorial function available in C++ standard library?

    // Initialize positions of cities
    vector<double> x, y;
    if(circle_config){
        for(int i = 0; i < l_cities; i++){
            double angle = rnd.Rannyu(0, 2 * M_PI);
            x.push_back(cos(angle));
            y.push_back(sin(angle));
        }
    }else{
        for (int i = 0; i < l_cities; i++) {
            x.push_back(rnd.Rannyu());
            y.push_back(rnd.Rannyu());
        }
    }
    // Write cities positions to file
    string conf = circle_config ? "circle" : "square";
    write_cities_config(x, y, "../data/cities_config_" + conf + ".dat");
    
    // Initialize population in order
    for (int i = 0; i < n_population; i++) {
        vector<city> temp_cities;
        for (int j = 0; j < l_cities; j++) {
            temp_cities.push_back(city(x[j], y[j], j));
        }
        // Save first city to keep it fixed
        city first_city = temp_cities[0];
        // Shuffle the rest of the cities
        shuffle(temp_cities.begin() + 1, temp_cities.end(), default_random_engine(rnd.Rannyu()*1e6));
        temp_cities[0] = first_city;
        trip t;
        t._n_cities = l_cities;
        t.cities = temp_cities;
        // Check if trip is valid
        check_trip(t);
        // Insert trip into population
        population.insert(make_pair(t.distance(), t));
    }
}

/*
print each trip in the population with its distance
*/
void GA::print_population() {
    // population is a multimap, so it is sorted by distance
    for (auto it = population.begin(); it != population.end(); it++) {
        cout << "Distance: " << it->first << endl;
        for (int i = 0; i < _l_cities; i++) {
            cout << it->second.cities[i].id << " ";
        }
        cout << endl;
    }
}

/*
write the cities configuration to a file
*/
void GA::write_cities_config(vector<double> x, vector<double> y, string path) {
    ofstream out(path);
    out << "city_id x y" << endl;
    if (!out.is_open()) {
        cerr << "Error opening file: " << path << endl;
        return;
    }
    for (int i = 0; i < x.size(); i++) {
        out << i << " " << x[i] << " " << y[i] << endl;
    }
    out.close();
}

/*
check if the trip is valid, i.e. all cities are unique
*/
void GA::check_trip(trip t) {
    map<int, bool> visited;
    for (int i = 0; i < t._n_cities; i++) {
        if (visited.find(t.cities[i].id) != visited.end()) {
            exit(EXIT_FAILURE);
        }
        visited[t.cities[i].id] = true;
    }
}

/*
write the i-th trip in the population to a file
*/
void GA::save_trip(int i, int n_gen, string path) {
    ofstream out(path, ios::app);
    if (!out.is_open()) {
        cerr << "Error opening file: " << path << endl;
        return;
    }
    auto it = population.begin();
    std::advance(it, i);
    out << n_gen << " ";
    out << it->first << " "; 
    for (int j = 0; j < _l_cities; ++j) {
        out << it->second.cities[j].id << " ";
    }
    //out << it->second.cities[0].id << " "; // close the trip 
    out << endl;
    out.close();
}

/*
write on file the best trip in the population loss
*/
void GA::write_loss(int n_gen, string path) {
    ofstream out(path, ios::app);
    if (!out.is_open()) {
        cerr << "Error opening file: " << path << endl;
        return;
    }
    auto it = population.begin();
    out << n_gen << " " << it->first << endl;
    out.close();
}

/*
write on file the loss averaged on the best half of the population
*/
void GA::write_mean_loss(int n_gen, string path) {
    ofstream out(path, ios::app);
    if (!out.is_open()) {
        cerr << "Error opening file: " << path << endl;
        return;
    }
    double mean = 0;
    int count = 0;
    int half = population.size() / 2;
    for (auto it = population.begin(); it != population.end() && count < half; ++it, ++count) {
        mean += it->first;
    }
    if (half > 0)
        mean /= half;
    out << n_gen << " " << mean << endl;
    out.close();
}

/*
select an individual from the population using power law
*/
trip GA::select_individual(int p) {
    int M = population.size();
    double r = rnd.Rannyu(); // r in [0,1)
    int j = int(M * pow(r, p));
    if (j >= M) j = M - 1;
    auto it = population.begin();
    std::advance(it, j);
    return it->second;
}

/*
select an individual from the population with probability ~ 1/loss
*/
trip GA::select_individual_probabilistic() {
    // Sum of the inverse of the distances
    double sum_inv = 0.0;
    for (auto it = population.begin(); it != population.end(); ++it) {
        sum_inv += 1.0 / it->first;
    }

    double r = rnd.Rannyu(0, sum_inv);

    // Select an individual with probability proportional to 1/loss
    double acc = 0.0;
    for (auto it = population.begin(); it != population.end(); ++it) {
        acc += 1.0 / it->first;
        if (acc >= r) {
            return it->second;
        }
    }
    // This should never happen if the population is not empty
    return population.begin()->second;
}

/*
perform crossover between two parents in a random position
*/
void GA::crossover(trip& t1, trip& t2){
    int cut_index = int(rnd.Rannyu(1, _l_cities - 1));
    
    // sons
    vector<city> child1 = t1.cities;
    vector<city> child2 = t2.cities;

    // save left part of the trip of t1 and t1
    vector<int> ids1, ids2;
    for (int i = 0; i < cut_index; ++i) {
        ids1.push_back(t1.cities[i].id);
        ids2.push_back(t2.cities[i].id);
    }

    // fill the rest of the trip with the right part of t2 
    int idx1 = cut_index;
    for (int i = 0; i < _l_cities; ++i) {
        int cid = t2.cities[i].id;
        // Check if the city is already in the left part of the trip
        if (std::find(ids1.begin(), ids1.end(), cid) == ids1.end()) {
            child1[idx1++] = t2.cities[i];
            ids1.push_back(cid);
            if (idx1 >= _l_cities) break;
        }
    }

    // fill the rest of the trip with the right part of t1
    int idx2 = cut_index;
    for (int i = 0; i < _l_cities; ++i) {
        int cid = t1.cities[i].id;
        if (std::find(ids2.begin(), ids2.end(), cid) == ids2.end()) {
            child2[idx2++] = t1.cities[i];
            ids2.push_back(cid);
            if (idx2 >= _l_cities) break;
        }
    }

    check_trip(t1);
    check_trip(t2);

    // assign the new children to the parents
    t1.cities = child1;
    t2.cities = child2;
}

