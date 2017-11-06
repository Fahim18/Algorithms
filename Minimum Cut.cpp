#include <iostream>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <sstream>
#include <istream>

/* This program reads into an array a graph provided
   by a text file "graph.txt" which contains, in each line,
   first, the node we are on, and then space-separated values
   representing the nodes the principal node is connected
   directly to by an edge. The graph contained less than 3000
   edges, as can be deduced from the program. */

using namespace std;

int MinCuts(int arr[][2], int distinct, int numberofedges);
int totalnumberofedges;

int main() {
    srand(time(NULL));

    ifstream myfile("graph.txt");
    string line;
    int initialedges[3000][2];
    int ith = 0;

    while( getline( myfile, line ) ) {
         istringstream iss( line );
         int number;
         int firstnum = 0;
         int initialize;
         while( iss >> number ) {
            if(firstnum == 0) {
                initialize = number;
                firstnum++;
            }
            else {
                if(number > initialize) {
                    initialedges[ith][0] = initialize;
                    initialedges[ith][1] = number;
                    ith++;
                }
            }
         }
    }

    int edges[ith][2];
    for(int k = 0; k < ith; k++) {
        edges[k][0] = initialedges[k][0];
        edges[k][1] = initialedges[k][1];
    }
    myfile.close();

    totalnumberofedges = ith;

    int minimum = MinCuts(edges, 200, ith);
    int secondminimum;

    for(long int turns = 0; turns < 200000; turns++) {
        srand(time(NULL));

        ifstream myfile("graph.txt");
        string line;
        int initialedges[3000][2];
        int ith = 0;

        while( getline( myfile, line ) ) {
             istringstream iss( line );
             int number;
             int firstnum = 0;
             int initialize;
             while( iss >> number ) {
                if(firstnum == 0) {
                    initialize = number;
                    firstnum++;
                }
                else {
                    if(number > initialize) {
                        initialedges[ith][0] = initialize;
                        initialedges[ith][1] = number;
                        ith++;
                    }
                }
             }
        }

        int edges[ith][2];
        for(int k = 0; k < ith; k++) {
            edges[k][0] = initialedges[k][0];
            edges[k][1] = initialedges[k][1];
        }
        myfile.close();

        totalnumberofedges = ith;

        secondminimum = MinCuts(edges, 200, ith);
        if(secondminimum < minimum) minimum = secondminimum;
    }

    cout << "number of cuts (prone to error): " << minimum << endl;
    return 0;
}

int MinCuts(int arr[][2], int distinct, int numberofedges) {

    //if completely divided into two cuts, determine min-cuts
    if (distinct == 2) {
        int minimumcuts = 0;
        for(int i = 0; i < totalnumberofedges; i++) {
            if(arr[i][0] != arr[i][1]) minimumcuts++;
        }
        return minimumcuts;
    }

    //keep track of self loops
    int self_loops = 0;

    //Generate a random edge
    int random_edge = 1 + std::rand() % (numberofedges);

    int bigger, smaller;
    int countof = 0;
    int index = 0;

    //Select (smaller, bigger) of randomly generated edge
    do {
        if(arr[index][0] >= arr[index][1]) {
            bigger = arr[index][0];
            smaller = arr[index][1];
        }
        else {
            bigger = arr[index][1];
            smaller = arr[index][0];
        }
        if(bigger != smaller) countof++;

        index++;
    } while(countof != random_edge);

    //Now we have (smaller, bigger)

    for(int i = 0; i < totalnumberofedges; i++) {
        if(arr[i][0] == bigger) arr[i][0] = smaller;
        if(arr[i][1] == bigger) arr[i][1] = smaller;
        if(arr[i][0] == arr[i][1]) self_loops++;
    }

    return MinCuts(arr, distinct - 1, totalnumberofedges - self_loops);

}


