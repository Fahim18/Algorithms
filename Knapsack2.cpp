/*
 *      This is a recursive implementation of the knapsack
 *      algorithm. Recursion was suitable over iteration
 *      because of the large input size. Used a hash-table
 *      to store relevant data to avoid redundancy (memoization).
 *      The function to_string() is available in the header file
 *      (essentially uses the usual ostringstream to convert int
 *      to a string). Structure of input file:
 *      [knapsack_size] [number_of_items]
 *      [value of item 1] [weight of item 1]
 *      [value of item 2] [weight of item 2]
 *      ...
 *      Used string hashing where the hash of "a,b" stores
 *      the optimal knapsack solution of 'b' items for a knapsack
 *      size of 'a'. (the 'b' items being all the items from 1 to
 *                     'b' among the input items).
 */

#include "std_lib_facilities.h"

using namespace std;

int knapsack(int Size, int items, int * weights, int * values);

unordered_map<string, int> prev_solutions;

int main()
{
    int knapsack_size, number_of_items;
    string filename = "knapsack3.txt", line;
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

    data.close();

    int optimal = knapsack(knapsack_size, number_of_items, weights, values);

    cout << "Optimal Solution for " << number_of_items
         << " items with a knapsack size of " << knapsack_size
         << " is: " << optimal << endl;

    delete [] weights;
    delete [] values;
    return 0;
}

int knapsack(int Size, int items, int * weights, int * values) {
    //Base case for recursive approach
    if(items == 0 || weights == 0) return 0;

    string _current = to_string(Size) + "," + to_string(items);

    //If solution in hash table, return
    if(prev_solutions.find(_current) != prev_solutions.end())
        return prev_solutions[_current];

    int _max;

    //if weight of current item more than knapsack size, recurse on A[i - 1, w]
    if(weights[items - 1] > Size) {
        _max = knapsack(Size, items - 1, weights, values);

        //Insert solution of current knapsack(size, item) to map
        prev_solutions[_current] = _max;

        return _max;
    }

    //If all above fails, recursively compute best and insert into hash table
    int _first = knapsack(Size, items - 1, weights, values);
    int _second = knapsack(Size - weights[items - 1], items - 1, weights, values) + values[items - 1];
    _max = _first > _second ? _first : _second;
    prev_solutions[_current] = _max;

    return _max;
}
