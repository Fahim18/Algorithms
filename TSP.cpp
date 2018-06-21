/*
 *      This is an implementation of the dynamic
 *      programming algorithm for the TSP. The
 *      structure of the input file was as follows:
 *      [number_of_cities]
 *      [x coordinate of city1][y coordinate of city1]
 *      ....
 *      Subsets of cities computed as bit strings where
 *      a '1' in the i'th position shows presence of
 *      i'th city in the subset.
 */

#include "std_lib_facilities.h"
#include <unordered_map>

#define _INFINITY 100000000

using namespace std;

int number_of_cities;           //25 for assignment

struct coordinates {
    double x, y;
};

//Contains bit strings of subsets of size m
vector<string> _subsets;
vector<coordinates> cities;

void all_subsets(int _size);
void make_subsets(string _set, int _size, int _depth);
double dist(int m, int n);

int main()
{
    // ==== READ COORDINATES FROM FILE =====
    coordinates current;
    string filename = "tsp1.txt", line;
    ifstream cityfile(filename);
    getline(cityfile, line);
    stringstream(line) >> number_of_cities;

    while(getline(cityfile, line)) {
        stringstream(line) >> current.x >> current.y;
        cities.push_back(current);
    }

    cityfile.close();
    //        ===================

    //Calculate the distances to avoid redundancy
    double _dist[number_of_cities][number_of_cities];
    for(int i = 0; i < number_of_cities; ++i)
        for(int j = 0; j < number_of_cities; ++j)
            _dist[i][j] = dist(i, j);


    unordered_map<string, int> map1, map2;
    vector<vector<double>> first, second;
    vector<double> dummyarray;
    dummyarray.push_back(_INFINITY);
    dummyarray.resize(number_of_cities, 0);

    /* Handle case for subsets of size 1, and finish vertex 1 */
    string dummy, sizeoness;
    for(int i = 0; i < number_of_cities; ++i) dummy += "0";

    sizeoness = dummy; sizeoness[0] = '1';
    //Hash with index in vector 'first'
    map1[sizeoness] = 0;
    first.push_back(dummyarray);
    first[0][0] = 0;

    /* Main Loop */
    string _bitset, track; vector<int> _set; double minimum;
    for(int m = 2; m <= number_of_cities; ++m) {
        //Compute all subsets of size m that have 1
        all_subsets(m);

        //Hash all subsets with its index in vector 'second'
        for(int i = 0; i < _subsets.size(); ++i) {
            map2[_subsets[i]] = i;
            //Place a dummy array that needs to be filled later
            second.push_back(dummyarray);
        }

        for(int i = 0; i < _subsets.size(); ++i) {
            //Fill _set with city numbers that are present in path
            //e.g _set = {c1, c2, c3....} that are on path
            _set.clear();
            _bitset = _subsets[i];
            for(int j = 0; j < number_of_cities; ++j)
                if(_bitset[j] == '1') _set.push_back(j);

            //For each j in S (j=/=1) and each k in S(k=/=j)
            for(int j = 1; j < _set.size(); ++j) {
                minimum = _INFINITY;
                for(int k = 0; k < _set.size(); ++k) {
                    if(k != j) {
                        track = _bitset;
                        track[_set[j]] = '0';
                        if(minimum > first[map1[track]][_set[k]] + _dist[_set[k]][ _set[j]])
                            minimum = first[map1[track]][_set[k]] + _dist[_set[k]][ _set[j]];
                    }
                }
                second[map2[_bitset]][_set[j]] = minimum;
            }
        }
        swap(map1, map2); map2.clear();
        first = second; second.clear();
    }

    double tspmin = _INFINITY;
    for(int i = 1; i < number_of_cities; ++i)
        if(tspmin > first[0][i] + _dist[0][i])
            tspmin = first[0][i] + _dist[0][i];
    cout << "Min TSP dist: " << tspmin << endl;

    return 0;
}

//Empty vector and populate it with subsets of size _size
void all_subsets(int _size) {
    _subsets.clear();

    make_subsets("1", _size - 1, 0);
}

//Recursively computes subsets to populate vector _subsets
void make_subsets(string _set, int _size, int _depth) {
    if(_depth <= number_of_cities - 1 && _size == 0) {
        int padding = number_of_cities - _set.length();
        for(int i = 0; i < padding; ++i) _set += "0";
        _subsets.push_back(_set);
        return;
    }
    if(_depth == number_of_cities - 1) return;

    make_subsets(_set + "1", _size - 1, _depth + 1);
    make_subsets(_set + "0", _size, _depth + 1);
}

//Calculates the euclidean distance between two cities
double dist(int m, int n) {
    double x_diff = cities[m].x - cities[n].x;
    double y_diff = cities[m].y - cities[n].y;
    return sqrt(pow(x_diff, 2.0000) + pow(y_diff, 2.0000));
}
