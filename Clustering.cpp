/*
 *      This program uses Kruskal's MST algorithm
 *      to find the maximum spacing of a 4-clustering
 *      (4 clusters remain at the end). Input is in the
 *      following format:
 *      [number of nodes]
 *      [node1 of edge1] [node2 of edge1] [weight of edge1]
 *      ...
 *      Union-rank used to find and merge clusters.
 */

#include "std_lib_facilities.h"
#include <algorithm>

using namespace std;

struct edge {
    int node1, node2, weight;
};

bool acompare(edge lhs, edge rhs) { return lhs.weight < rhs.weight; }
int clustering(edge * edges, int nodes, int arcs);
int find_set(int * root, int node);
void union_set(int * root, int * height, int node1, int node2);

int main()
{
    int number_of_nodes;

    string filename = "clustering1.txt", line;
    ifstream graph_file(filename);

    getline(graph_file, line);
    stringstream(line) >> number_of_nodes;

    //Allocate memory for all the edges
    int number_of_edges = (number_of_nodes - 1) * (number_of_nodes/2);
    edge * all_edges = new (nothrow) edge [number_of_edges];
    if(!all_edges) {
        cerr << "Error: Memory for all edges could not be allocated." << endl;
        exit(1);
    }

    //Store edges into the array
    int index = 0;
    while(getline(graph_file, line)) {
        stringstream(line) >> all_edges[index].node1
                           >> all_edges[index].node2
                           >> all_edges[index].weight;
        index++;
    }

    graph_file.close();

    //Sort the array of edges according to weight
    sort(all_edges, all_edges + number_of_edges, acompare);

    int maximum_spacing = clustering(all_edges, number_of_nodes, number_of_edges);
    cout << "Maximum spacing of a 4-clustering: " << maximum_spacing << endl;

    //Delete array of edges after completion
    delete [] all_edges;
    all_edges = nullptr;

    return 0;
}

int clustering(edge * edges, int nodes, int arcs) {
    int number_of_clusters = nodes;
    int maximum_spacing;
    int maximum_edge = 0;

    //root array keeps track of the cluster in which a node is. See union-rank
    int * root = new (nothrow) int [nodes];
    for(int i = 0; i < nodes; ++i) root[i] = i;

    //height array is the 'rank' in union-rank. book-keeping for clustering
    int * height = new (nothrow) int [nodes];
    for(int i = 0; i < nodes; ++i) height[i] = 0;

    //main loop of kruskal's algorithm
    for(int i = 0; i < arcs; ++i) {

        //if number of clusters reaches four, find the weight of the next cluster crossing edge and return it
        if(number_of_clusters == 4) {

            maximum_spacing = edges[maximum_edge].weight;
            while(find_set(root, edges[maximum_edge].node1 - 1) == find_set(root, edges[maximum_edge].node2 - 1)) {
                maximum_edge++;
                maximum_spacing = edges[maximum_edge].weight;
            }
            break;
        }

        //if edge does not create a cycle, add it to MST
        if(find_set(root, edges[i].node1 - 1) != find_set(root, edges[i].node2 - 1)) {

            union_set(root, height, edges[i].node1 - 1, edges[i].node2 - 1);
            maximum_edge = i + 1;
            number_of_clusters--;
        }

    }

    delete [] root;
    delete [] height;
    root = nullptr;
    height = nullptr;

    return maximum_spacing;
}

//Return the parent of the cluster that 'node' is a part of
int find_set(int * root, int node) {
    while(node != root[node]) node = root[node];
    return node;
}

//join the clusters that node1 and node2 are parts of
void union_set(int * root, int * height, int node1, int node2) {

    int parent1 = find_set(root, node1);
    int parent2 = find_set(root, node2);

    //point head of smaller tree to head of larger tree
    if(height[parent1] > height[parent2]) root[parent2] = parent1;
    else {
        root[parent1] = parent2;

        //increment height if trees (representing clusters) of equal size merged
        if(height[parent1] == height[parent2])
            height[parent2] = height[parent2] + 1;
    }
}
