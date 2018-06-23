/*
 *      This is an implementation of the Nearest
 *      Neighbor heuristic for TSP. The algorithm is:
 *      1) Add first city to tour.
 *      2) Select the city nearest to the last added
 *          city. Add that city and the distance to the
 *          TSP tour.
 *      3) Add distance from final city to first city
 *          to complete tour.
 *      Structure of input file:
 *      [number of cities]
 *      [city number] [x of current city] [y of current city]
 *      ......
 */

#include "std_lib_facilities.h"
#include <iomanip>
#define _INFINITY 100000000000

using namespace std;

struct city {
    int number;
    bool visited;
    double x, y;
};

int number_of_cities;
vector<city> cities;
double dist(city x, city y);

int main()
{
    //Load coordinates from file
    string filename = "coordinates.txt", line;
    ifstream data(filename);
    getline(data, line);
    stringstream(line) >> number_of_cities;

    city dummy; dummy.visited = false;
    for(int i = 0; i < number_of_cities; ++i) {
        getline(data, line);
        stringstream(line) >> dummy.number >> dummy.x >> dummy.y;
        cities.push_back(dummy);
    }

    data.close();
    // =========================

    cities[0].visited = true;
    double minimum, min_candidate, answer = 0.0000; int _next;
    city current_city;
    vector<int> all_visited; all_visited.push_back(1);

    //In the loop, select the nearest city to the city last added
    //and then add it to the tour
    while(all_visited.size() != number_of_cities) {
        current_city = cities[all_visited.back() - 1];
        minimum = _INFINITY;

        for(int i = 0; i < number_of_cities; ++i) {
            if(cities[i].visited == false) {
                min_candidate = dist(cities[i], current_city);
                if(minimum > min_candidate) {
                    minimum = min_candidate;
                    _next = cities[i].number;
                }
            }
        }

        answer += sqrt(minimum);
        all_visited.push_back(_next);
        cities[_next - 1].visited = true;
    }

    //Finally, add the return distance from final city to first city
    answer += sqrt(dist(cities[0], cities[all_visited.back() - 1]));

    cout << setprecision(4) << fixed;
    cout << "Min TSP tour using Nearest Neighbor heuristic: " << answer << endl;

    return 0;
}

double dist(city one, city two) {
    double x_diff = one.x - two.x;
    double y_diff = one.y - two.y;
    return (x_diff * x_diff) + (y_diff * y_diff);
}
