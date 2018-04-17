/*
 *      This is an implementation of the algorithm
 *      used to compute the maximum weight independent
 *      set of a path graph. The format of the file in
 *      which it was stored is:
 *      [number of nodes]
 *      [weight of node 1]
 *      .....
 *      Also implemented within is the reconstruction of
 *      the solution to output the exact nodes used.
 */

#include "std_lib_facilities.h"

using namespace std;

int main()
{
    int number_of_nodes;
    string filename = "mwis.txt", line;
    ifstream path_graph(filename);

    getline(path_graph, line);
    stringstream(line) >> number_of_nodes;

    //Load all the node weights into an array
    long long * path = new (nothrow) long long [number_of_nodes];
    for(int i = 0; i < number_of_nodes; ++i) {
        getline(path_graph, line);
        stringstream(line) >> path[i];
    }

    path_graph.close();

    //keeps track of the max-weight IS value of the first i nodes
    long long * mwis = new (nothrow) long long [number_of_nodes + 1];

    //Initialize first two values for the loop's base cases
    mwis[0] = 0;
    mwis[1] = path[0];

    //Main loop of the max-weight IS algorithm
    for(int i = 2; i < (number_of_nodes + 1); ++i)
        mwis[i] = (mwis[i - 1] >= mwis[i - 2] + path[i - 1]) ? mwis[i - 1] : (mwis[i - 2] + path[i - 1]);

    //Stores all the nodes in the max-weight IS
    vector<int> nodes;

    //Reconstruct the solution to find which nodes are in it
    int index = number_of_nodes;
    while(index >= 1) {
        if(index == 1) {
            nodes.push_back(index);
            break;
        }

        if(mwis[index - 1] >= mwis[index - 2] + path[index - 1]) index--;

        else {
            nodes.push_back(index);
            index -= 2;
        }
    }

    //Prints out the max-weight IS for the whole path-graph
    cout << mwis[number_of_nodes] << endl;

    //Prints out the nodes in the max-weight IS of whole graph
    for(int i = 0; i < nodes.size(); ++i)
        cout << nodes[i] << endl;

    delete [] path;
    path = nullptr;
    delete [] mwis;
    mwis = nullptr;

    return 0;
}
