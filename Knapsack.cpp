/*
 *      Iterative implementation of the Knapsack
 *      algorithm. Structure of input text file was:
 *      [knapsack_size] [number_of_items]
 *      [value of item 1] [weight of item 1]
 *      [value of item 2] [weight of item 2]
 *      ....
 *      Took under 1 second for 100 items for a knapsack
 *      size of 10,000
 */

#include "std_lib_facilities.h"

using namespace std;

int knapsack(int _size, int _items, int * weights, int * values);

int main()
{
    int knapsack_size, number_of_items;
    string filename = "knapsack1.txt", line;
    ifstream data(filename);

    getline(data, line);
    stringstream(line) >> knapsack_size >> number_of_items;

    int * weights = new (nothrow) int [number_of_items];
    int * values = new (nothrow) int [number_of_items];

    int index = 0;
    while(getline(data, line)) {
        stringstream(line) >> values[index] >> weights[index];
        ++index;
    }

    int optimal_solution = knapsack(knapsack_size, number_of_items, weights, values);

    cout << "Optimal Solution for " << number_of_items
         << " items with a knapsack size of " << knapsack_size
         << " is: " << optimal_solution << endl;

    delete [] weights;
    delete [] values;
    return 0;
}

int knapsack(int _size, int _items, int * weights, int * values) {

    //Allocate memory for 2D array containing solutions to all subproblems
    int ** _solution = new (nothrow) int * [_items + 1];
    for(int i = 0; i <= _items; ++i)
        _solution[i] = new (nothrow) int [_size + 1];

    //Base cases for iterative dynamic algorithm implementation
    for(int i = 0; i <= _items; ++i) _solution[i][0] = 0;
    for(int i = 0; i <= _size; ++i) _solution[0][i] = 0;

    for(int i = 1; i <= _items; ++i) {
        for(int j = 1; j <= _size; ++j) {
            if(weights[i - 1] > j)
                _solution[i][j] = _solution[i - 1][j];      //Another base case
            else
                _solution[i][j] = (_solution[i - 1][j] > (_solution[i - 1][j - weights[i - 1]] + values[i - 1])) ?
                    (_solution[i - 1][j]) : (_solution[i - 1][j - weights[i - 1]] + values[i - 1]);
        }
    }

    int solution = _solution[_items][_size];

    //Deallocate dynamically assigned memory
    for(int i = 0; i <= _items; ++i) delete [] _solution[i];
    delete [] _solution;

    return solution;
}
