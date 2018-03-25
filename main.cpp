/*
 *      This is an implementation of Prim's Algorithms
 *      using a heap that stores nodes rather than edges.
 *      Input is from a file with the first line stating
 *      the number of nodes, and the rest of the lines being
 *      triplets of integers of the form:
 *      [node1 of edge1] [node2 of edge1] [weight of edge1]
 */

#include "std_lib_facilities.h"

using namespace std;

struct edge {
    int node;
    int weight_from_head;
    edge * next;
};

long long prim(edge ** graph, int number_of_nodes);
void _delete(vector<int> &heap, int index, int * node_to_index, int * key);
void _insert(vector<int> &heap, int node, int * node_to_index, int * key);
void _bubble_up(vector<int> &heap, int index, int * node_to_index, int * key);
void _bubble_down(vector<int> &heap, int index, int * node_to_index, int * key);
void delete_graph(edge ** graph, int number_of_nodes);
void delete_list(edge * header);

int main()
{
    int number_of_nodes;

    string filename = "edges.txt", line;
    ifstream graph_file(filename);

    getline(graph_file, line);
    stringstream(line) >> number_of_nodes;

    //Allocate space for a linked list to represent graph
    edge ** graph = new (nothrow) edge * [number_of_nodes];
    if(!graph) {
        cerr << "Error: Memory for graph could not be allocated.";
        exit(1);
    }
    for(int i = 0; i < number_of_nodes; ++i)
        graph[i] = nullptr;

    //Load graph
    int node1, node2, weight;
    while(getline(graph_file, line)) {
        stringstream(line) >> node1 >> node2 >> weight;

        edge * new_edge1 = new (nothrow) edge;
        * new_edge1 = { node2, weight, graph[node1 - 1] };
        graph[node1 - 1] = new_edge1;

        edge * new_edge2 = new (nothrow) edge;
        * new_edge2 = { node1, weight, graph[node2 - 1] };
        graph[node2 - 1] = new_edge2;
    }

    graph_file.close();     //Close text file once loading is complete

    long long min_span_tree_size = prim(graph, number_of_nodes);

    cout << "Weight of MST: " << min_span_tree_size << endl;

    delete_graph(graph, number_of_nodes);
    return 0;
}

long long prim(edge ** graph, int number_of_nodes) {

    long long weight_of_tree = 0;

    //initialize heap to store nodes
    vector<int> heap;
    heap.reserve(number_of_nodes - 1);
    for(int i = 0; i < number_of_nodes - 1; ++i)
        heap.push_back(i + 2);

    //Now initialize node_to_index and key arrays
    int * node_to_index = new (nothrow) int [number_of_nodes];
    node_to_index[0] = -1;
    for(int i = 1; i < number_of_nodes; ++i)
        node_to_index[i] = i - 1;      //n'th node's index is at node_to_index[n - 1]

    int * key = new (nothrow) int [number_of_nodes];
    key[0] = 0;
    for(int i = 1; i < number_of_nodes; ++i)
        key[i] = 1000000;              //n'th node's key is at key[n - 1]

    //Initialize Set and Boolean array of explored nodes
    vector<int> explored;
    explored.reserve(number_of_nodes);
    explored.push_back(1);

    bool * node_explored = new (nothrow) bool [number_of_nodes];
    node_explored[0] = true;
    for(int i = 1; i < number_of_nodes; ++i) node_explored[i] = false;

    //main while loop
    while(explored.size() != number_of_nodes) {

        edge * tracker = graph[explored.back() - 1];

        while(tracker != nullptr) {

            if(!node_explored[(tracker->node) - 1]) {

                int change_key = tracker->weight_from_head;
                if(change_key < key[(tracker->node) - 1]) {
                    _delete(heap, node_to_index[(tracker->node) - 1], node_to_index, key);
                    key[(tracker->node) - 1] = change_key;
                    _insert(heap, tracker->node, node_to_index, key);
                }

            }

            tracker = tracker->next;
        }

        //add next smallest edge weight to spanning tree size
        //and also the next node in spanning tree
        explored.push_back(heap[0]);
        weight_of_tree += key[heap[0] - 1];
        node_explored[heap[0] - 1] = true;
        _delete(heap, 0, node_to_index, key);
    }

    delete [] node_to_index;
    delete [] key;
    delete [] node_explored;

    return weight_of_tree;
}

void _delete(vector<int> &heap, int index, int * node_to_index, int * key) {

    node_to_index[heap.back() - 1] = index;
    node_to_index[heap[index] - 1] = -1;
    heap[index] = heap.back();
    heap.pop_back();

    if(index == heap.size()) return;
    if(index == 0 || key[heap[index] - 1] >= key[heap[(index - 1)/2] - 1])
        _bubble_down(heap, index, node_to_index, key);
    else _bubble_up(heap, index, node_to_index, key);
}

void _insert(vector<int> &heap, int node, int * node_to_index, int * key) {

    heap.push_back(node);
    node_to_index[node - 1] = heap.size() - 1;
    _bubble_up(heap, heap.size() - 1, node_to_index, key);

}

void _bubble_up(vector<int> &heap, int index, int * node_to_index, int * key) {

    if(index == 0) return;

    int parent_index = (index - 1)/2;
    while(key[heap[index] - 1] < key[heap[parent_index] - 1]) {

        node_to_index[heap[index] - 1] = parent_index;
        node_to_index[heap[parent_index] - 1] = index;

        int swap_node = heap[index];
        heap[index] = heap[parent_index];
        heap[parent_index] = swap_node;

        index = parent_index;
        if(index == 0) return;
        parent_index = (index - 1)/2;
    }
}

void _bubble_down(vector<int> &heap, int index, int * node_to_index, int * key) {

    //boundary cases
    if((2*index + 1) > (heap.size() - 1)) return;
    if((2*index + 1) == (heap.size() - 1)) {

        if(key[heap[index] - 1] <= key[heap[2*index + 1] - 1]) return;

        node_to_index[heap[index] - 1] = 2*index + 1;
        node_to_index[heap[2*index + 1] - 1] = index;

        int swap_node = heap[index];
        heap[index] = heap[2*index + 1];
        heap[2*index + 1] = swap_node;

        return;
    }
    if(key[heap[index] - 1] <= key[heap[2*index + 1] - 1] &&
        key[heap[index] - 1] <= key[heap[2*index + 2] - 1]) return;

    //else, swap, and recall _bubble_down();
    int index_of_smaller = (key[heap[2*index + 1] - 1] < key[heap[2*index + 2] - 1]) ? (2*index + 1) : (2*index + 2);

    node_to_index[heap[index] - 1] = index_of_smaller;
    node_to_index[heap[index_of_smaller] - 1] = index;

    int swap_node = heap[index];
    heap[index] = heap[index_of_smaller];
    heap[index_of_smaller] = swap_node;

    _bubble_down(heap, index_of_smaller, node_to_index, key);
}

void delete_graph(edge ** graph, int number_of_nodes) {
    for(int i = 0; i < number_of_nodes; ++i)
        if(graph[i] != nullptr) delete_list(graph[i]);
    for(int i = 0; i < number_of_nodes; ++i)
        graph[i] = nullptr;
    delete [] graph;
}

void delete_list(edge * header) {
    if(header->next != nullptr) delete_list(header->next);
    header->next = nullptr;
    delete header;
    header = nullptr;
}
