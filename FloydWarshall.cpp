/*
 *      This is Floyd-Warshall's algorithm for
 *      computing all-pairs shortest paths. Either
 *      had to quit if there was a negative cycle or
 *      had to output the shortest shortest-path.
 *      Used two n x n arrays rather than a n x n x n
 *      array. the input file had the following format:
 *      [number of nodes] [number of edges]
 *      [tail of edge 1] [head of edge 1] [weight of edge 1]
 *      ...
 */

#include "std_lib_facilities.h"

using namespace std;

int _INFINITY = 1000000000;
int number_of_nodes = 1000;

void _delete(int ** first, int ** second);

int main()
{
    //Allocate space for two n x n matrices for
    //holding A[i, j, k] and A[i, j, k - 1]
    // ==== BEGIN ====

    int ** first = new int * [number_of_nodes];
    int ** second = new int * [number_of_nodes];

    for(int i = 0; i < number_of_nodes; ++i) {
        first[i] = new int [number_of_nodes];
        second[i] = new int [number_of_nodes];
    }

    // ==== END ====

    //Populate first 2D array with base cases for k = 0
    // ==== BEGIN ====

    for(int i = 0; i < number_of_nodes; ++i) {
        for(int j = 0; j < number_of_nodes; ++j) {
            if(i == j) first[i][j] = 0;
            else first[i][j] = _INFINITY;
        }
    }

    string filename = "graph1.txt", line;
    int nodes, edges, node1, node2, weight;

    ifstream graph_file(filename);
    getline(graph_file, line);
    stringstream(line) >> nodes >> edges;

    for(int i = 0; i < edges; ++i) {
        getline(graph_file, line);
        stringstream(line) >> node1 >> node2 >> weight;
        first[node1 - 1][node2 - 1] = weight;
    }

    graph_file.close();

    // ==== END ====

    //Main Loop of Floyd-Warshall's Algorithm
    // ==== BEGIN ====

    int candidate1, candidate2;
    int ** dummy;

    for(int k = 0; k < nodes; ++k) {
        for(int i = 0; i < nodes; ++i) {
            for(int j = 0; j < nodes; ++j) {
                candidate1 = first[i][j];
                candidate2 = first[i][k] + first[k][j];
                if(candidate1 >= _INFINITY && candidate2 >= _INFINITY)
                    second[i][j] = _INFINITY;
                else second[i][j] = (candidate1 < candidate2) ?
                                      candidate1 : candidate2;
            }
        }
        dummy = first;
        first = second;
        second = dummy;
    }

    // ==== END ====

    //Check for negative cycles, if any.
    for(int i = 0; i < nodes; ++i) {
        if(first[i][i] < 0) {
            cerr << "Contains negative cycle\n";
            //If negative cycle, deallocate memory and exit
            _delete(first, second);
            exit(1);
        }
    }

    //If no negative cycle, output
    int shortest = _INFINITY;
    for(int i = 0; i < nodes; ++i) {
        for(int j = 0; j < nodes; ++j) {
            if(shortest > first[i][j])
                shortest = first[i][j];
        }
    }
    cout << "The shortest path in the graph is: " << shortest << endl;

    //Deallocate memory
    _delete(first, second);

    return 0;
}

void _delete(int ** first, int ** second) {
    for(int i = 0; i < number_of_nodes; ++i) {
        delete [] first[i];
        delete [] second[i];
    }
    delete [] first;
    delete [] second;
}
