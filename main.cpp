/*
 *      In this implementation of 2SUM, I found the number
 *      of targets in the closed range [-10000,10000].
 *      The restraint of the assignment was that if x and y
 *      are in the hash table, x + y would only be a valid
 *      target if it was in the prescribed range, and x and y
 *      were 'not' equal.
 *
 *      Input was from a file named '2sum.txt', and there were
 *      one million lines, each line containing a single integer.
 */

#include "std_lib_facilities.h"
#include <functional>

using namespace std;

constexpr int total_data = 1000000;     //total magnitude of data from input
constexpr int hash_array_size = 100000;

hash<long long> hasher;         //STL hash function to hash input values, returns unsigned int

struct data {
    long long _value;
    data * next;
};

bool check_uniqueness(data ** hash_array, int index, long long val);
bool check_target(data ** hash_array, int t);
bool lookup(data ** hash_array, long long y);

int main()
{
    //Initialize hash table array
    data ** hash_array = new (nothrow) data * [hash_array_size];
    if(!hash_array) {
        cerr << "Error: memory for hash table array could not be allocated\n";
        exit(1);
    }
    for(int i = 0; i < hash_array_size; ++i) hash_array[i] = nullptr;

    //Populate hash table
    string filename = "2sum.txt", line;
    ifstream data_file(filename);
    while(getline(data_file, line)) {
        long long new_val;
        stringstream(line) >> new_val;
        unsigned int new_val_hash = hasher(new_val);
        int new_val_index = new_val_hash % hash_array_size;

        bool is_unique = check_uniqueness(hash_array, new_val_index, new_val);

        //if value unique, add to hash table
        if(is_unique) {
            data * new_data = new (nothrow) data;
            new_data->_value = new_val;
            new_data->next = hash_array[new_val_index];
            hash_array[new_val_index] = new_data;
        }
    }

    //2sum main loop - assignment requires number of targets in range [-10000,10000]
    int targets_in_range = 0;
    for(int i = -10000; i <= 10000; ++i) {
        int target = i;
        bool is_target = check_target(hash_array, target);
        if(is_target) { ++targets_in_range; cout << target << '\t' << "true" << endl; }
    }
    cout << targets_in_range << endl;

    //free all dynamically allocated memory
    for(int i = 0; i < hash_array_size; ++i) {
        data * tracker = hash_array[i];
        data * dummy_tracker = tracker;
        while(tracker != nullptr) {
            tracker = tracker->next;
            dummy_tracker->next = nullptr;
            delete dummy_tracker;
            dummy_tracker = tracker;
        }
        hash_array[i] = nullptr;
    }
    delete [] hash_array;

    return 0;
}

//if val in hash table, return false. else return true
bool check_uniqueness(data ** hash_array, int index, long long val) {
    data * tracker = hash_array[index];
    while(tracker != nullptr) {
        if(tracker->_value == val) return false;
        tracker = tracker->next;
    }
    return true;
}

//if t is a 2sum, return true. else return false
bool check_target(data ** hash_array, int t) {
    long long target = t;
    for(int i = 0; i < hash_array_size; ++i) {
        data * tracker = hash_array[i];
        while(tracker != nullptr) {
            long long x = tracker->_value;
            long long y = target - x;

            //if y is in hash table, target exists
            if(x != y)
                if(lookup(hash_array, y)) return true;
            tracker = tracker->next;
        }
    }
    return false;
}

//if y in hash table, return true. else return false
bool lookup(data ** hash_array, long long y) {
    unsigned int hash_of_y = hasher(y);
    int index_of_y = hash_of_y % hash_array_size;
    data * tracker = hash_array[index_of_y];
    while(tracker != nullptr) {
        if(tracker->_value == y) return true;
        tracker = tracker->next;
    }
    return false;
}
