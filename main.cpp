/*
 *      This is an implementation of Papadimitriou's
 *      algorithm for the 2SAT problem. The structure
 *      of the input file was as follows:
 *      [number_of_clauses = number_of_variables]
 *      [var1 or negation_of_var1] [var2 or negation_of_var2]
 *      .....
 *      negations represented by a minus sign in front of
 *      variable number. Since input files were really big,
 *      optimized it by finding the symmetric difference
 *      between variables and their negations to eliminate
 *      those clauses with variables that only appeared in one
 *      form (negation or no) throughout the input file, since
 *      they can be trivially satisfied. This brought down
 *      the number of clauses and variables to a manageable size.
 */

#include <iostream>
#include <ctime>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <string>

using namespace std;

struct clause {
    int first_var, second_var;
};

int number_of_clauses;
int number_of_var;

//Contains all the clauses
vector<clause> clauses;

/* At every iteration of inner loop, contains
   all the unsatisfied clauses */
vector<clause> unsatisfied;

/* Contains variable assignment. -x is at index of x + numOfVar.
   Since -x is just negation of x, computed as x + numOfVar = !x */
vector<bool> var_val;

bool test_assignment();

int main()
{
    //Seed pseudo-random number generator(i.e. rand())
    srand(time(NULL));

    //Read clauses from file
    string filename = "2sat.txt", line;
    ifstream input_file(filename);

    getline(input_file, line);
    stringstream(line) >> number_of_clauses;
    number_of_var = number_of_clauses;

    clause dummy; int first, second;
    while(getline(input_file, line)) {
        stringstream(line) >> first >> second;

        //if negation of x (represented as -x), save it as x + numOfVar
        if(first < 0)
            first = number_of_var + (-first);
        if(second < 0)
            second = number_of_var + (-second);

        //Insert into vector
        dummy.first_var = first;
        dummy.second_var = second;
        clauses.push_back(dummy);
    }

    input_file.close();

    //Papadimitriou's Algorithm
    int outerloop = log2(number_of_var), random_clause, random_var, flip_var;
    long long innerloop = (long long) 2 * number_of_var * number_of_var;
    bool _result = false;
    for(int i = 0; i < outerloop; ++i) {
        cout << i << endl;
        //Clear previous iteration assignment
        var_val.clear();

        //Construct a random variable assignment
        for(int j = 0; j < number_of_var; ++j)
            var_val.push_back(rand() % 2);

        //Deduce their negations. negation of x is represented by 2x
        for(int j = 0; j < number_of_var; ++j)
            var_val.push_back(!var_val[j]);

        //repeat 2n^2 times
        for(long long j = 0; j < innerloop; ++j) {

            //Check current assignment of boolean variables
            _result = test_assignment();

            //Check result of test_assignment()
            if(_result) {
                cout << "Solution to 2SAT instance exists" << endl;
                return 0;
            }

            //If not satisfied, choose randomly an unsatisfied
            //clause and randomly choose and flip a variable in it
            random_clause = rand() % unsatisfied.size();
            random_var = rand() % 2;

            if(random_var == 0)
                flip_var = unsatisfied[random_clause].first_var;
            else
                flip_var = unsatisfied[random_clause].second_var;


            if(flip_var <= number_of_var) {
                var_val[flip_var - 1] = !var_val[flip_var - 1];
                var_val[number_of_var + flip_var - 1] = !var_val[flip_var - 1];
            } else {
                var_val[flip_var - 1] = !var_val[flip_var - 1];
                var_val[flip_var - 1 - number_of_var] = !var_val[flip_var - 1];
            }
        }
    }

    //If loop exited without returning, say instance is unsatisfiable
    cout << "Solution does not exist for this instance." << endl;

    return 0;
}

/* Tests current assignment of variables. Returns
   true if all the clauses are satisfied. */
bool test_assignment() {
    //Clear last set of unsatisfied clauses
    unsatisfied.clear();

    //Compute new set of unsatisfied clauses
    for(int i = 0; i < number_of_clauses; ++i) {
        if(!(var_val[clauses[i].first_var - 1] || var_val[clauses[i].second_var - 1]))
            unsatisfied.push_back(clauses[i]);
    }

    if(unsatisfied.size() == 0)
        return true;

    return false;
}
