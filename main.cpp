#include "std_lib_facilities.h"

/*
 *      Graph provided in a text file with the first number being
 *      the vertex number and then the rest of the values in the
 *      line being 'node,dist' pairs with 'dist' being the 'node''s
 *      distance from the head node. Assignment on graph with 200
 *      nodes. Delete code under 'requirement of assignment' section
 *      and use shortest_path array however you like.
 *
 *      NOTE: Shortest paths are computed from node 1 in this code
 */

using namespace std;

constexpr int number_of_nodes = 200; //Initialize to number of nodes in graph
int number_of_connected_nodes = 0;

struct Node {
    int node_number;
    int dist_from_head;  //distance of this node from node at head of linked list
    Node * next;
};

Node * Load(string filename);
void dfs(Node * header, int node, bool * connected);
void dijkstra(Node * graph, int * shortest_path);
void _insert(int dist, vector<int> &heap, int node, int * node_index, int * index_node);
void _delete(vector<int> &heap, int index, int * node_index, int * index_node);
void _bubble_up(vector<int> &heap, int index, int * node_index, int * index_node);
void _bubble_down(vector<int> &heap, int index, int * node_index, int * index_node);
void delete_graph(Node * graph);
void delete_list(Node * header);

int main()
{
    //Load graph into a linked list
    //In this case, graph was in file named 'myfile3.txt'
    Node * graph = Load("myfile3.txt");

    //Keep track of the connected component that node 1 is a part of
    bool * is_connected = new (nothrow) bool [number_of_nodes];
    if(!is_connected) {
        cerr << "Error: Memory for boolean array could not be allocated\n";
        exit(3);
    }
    for(int i = 0; i < number_of_nodes; ++i) is_connected[i] = false;
    dfs(graph, 1, is_connected);

    //Declare and initialize array for shortest paths
    //i'th node's shortest path at shortest_path[i - 1]
    int * shortest_path = new (nothrow) int [number_of_nodes];
    for(int i = 0; i < number_of_nodes; ++i) shortest_path[i] = -1;

    //Call dijkstra
    dijkstra(graph, shortest_path);

    //Requirement of assignment
    int arr [] = {7,37,59,82,99,115,133,165,188,197};
    int array_size = sizeof(arr)/sizeof(*arr);
    for(int i = 0; i < array_size; ++i) {
        cout << shortest_path[arr[i] - 1];
        if(i != array_size - 1) cout << ",";
    }

    //delete graph before exit
    delete_graph(graph);
    //free memory for other things
    delete [] is_connected;
    delete [] shortest_path;

    return 0;
}

Node * Load(string filename) {

    //Create and initialize linked list to store graph
    Node * graph = new (nothrow) Node [number_of_nodes];
    if(!graph) {
        cerr << "Error: Could not allocate memory for graph" << endl;
        exit(1);
    }
    for(int i = 0; i < number_of_nodes; ++i) {
        graph[i].node_number = i + 1;
        graph[i].dist_from_head = 0;
        graph[i].next = nullptr;
    }

    ifstream graph_file(filename);
    string line;

    while(getline(graph_file, line)) {

        //read first integer to get head of linked list
        stringstream per_line(line);
        int linked_list_index;
        per_line >> linked_list_index;
        --linked_list_index;

        string node_dist_pair;
        while(per_line >> node_dist_pair) {
            //Separate "node,dist" into 'node' and 'dist' integers
            int node, dist;
            stringstream node_dist(node_dist_pair);
            node_dist >> node;
            node_dist.ignore();         //ignore comma in "node,dist"
            node_dist >> dist;

            //Once (node, dist) pair read, save in a Node, and insert into linked list
            Node * new_node = new (nothrow) Node;
            if(!new_node) {
                cerr << "Error: Memory for node could not be allocated\n";
                exit(2);
            }
            new_node->node_number = node;
            new_node->dist_from_head = dist;
            new_node->next = graph[linked_list_index].next;
            graph[linked_list_index].next = new_node;
        }
    }

    graph_file.close();

    return graph;
}

//Run DFS from node 1 to discover its connected component
void dfs(Node * header, int node, bool * connected) {

    connected[node - 1] = true;
    ++number_of_connected_nodes;

    Node * tracker = &header[node - 1];
    while(tracker->next != nullptr) {
        tracker = tracker->next;
        if(!connected[(tracker->node_number) - 1])
            dfs(header, tracker->node_number, connected);
    }
}

void dijkstra(Node * graph, int * shortest_path) {

    vector<int> nodes_explored;    //keeps track of the number of nodes explored and their order
    bool full_explored[number_of_nodes];    //keeps track of nodes that have received final shortest_path value
    for(int i = 0; i < number_of_nodes; ++i) full_explored[i] = false;

    //declare heap and initialize arrays for heap-to-node mappings
    vector<int> heap;
    int INDEX_TO_NODE[number_of_nodes];
    int NODE_TO_INDEX[number_of_nodes];     //i'th node maps to index NODE_TO_INDEX[i - 1]
    for(int i = 0; i < number_of_nodes; ++i) {
        INDEX_TO_NODE[i] = -1;
        NODE_TO_INDEX[i] = -1;
    }

    //start at first node, i.e. node 1
    shortest_path[0] = 0;
    nodes_explored.push_back(1);
    full_explored[0] = true;

    while(nodes_explored.size() != number_of_connected_nodes) {

        Node * tracker = graph[nodes_explored.back() - 1].next;
        int prev_dist = shortest_path[nodes_explored.back() - 1];

        while(tracker != nullptr) {

            if(!full_explored[(tracker->node_number) - 1]) {
                //if no intermediate shortest path available, insert initial to heap
                if(shortest_path[(tracker->node_number) - 1] == -1) {
                    int dist = prev_dist + (tracker->dist_from_head);
                    shortest_path[(tracker->node_number) - 1] = dist;
                    _insert(dist, heap, tracker->node_number, NODE_TO_INDEX, INDEX_TO_NODE);
                }

                else {
                    //pluck off old value of shortest_path from heap and re-insert new one (if changes)
                    int change_dist = prev_dist + (tracker->dist_from_head);
                    if(change_dist < shortest_path[(tracker->node_number) - 1]) {
                        shortest_path[(tracker->node_number) - 1] = change_dist;
                        _delete(heap, NODE_TO_INDEX[(tracker->node_number) - 1], NODE_TO_INDEX, INDEX_TO_NODE);
                        _insert(change_dist, heap, tracker->node_number, NODE_TO_INDEX, INDEX_TO_NODE);
                    }
                }
            }
            tracker = tracker->next;
        }

        //add next node to conquered territory, i.e. finalize shortest path
        int new_node = INDEX_TO_NODE[0];
        _delete(heap, 0, NODE_TO_INDEX, INDEX_TO_NODE);
        nodes_explored.push_back(new_node);
        full_explored[new_node - 1] = true;
    }
}

void _insert(int dist, vector<int> &heap, int node, int * node_index, int * index_node) {
    heap.push_back(dist);
    node_index[node - 1] = heap.size() - 1;
    index_node[heap.size() - 1] = node;
    _bubble_up(heap, heap.size() - 1, node_index, index_node);
}

void _delete(vector<int> &heap, int index, int * node_index, int * index_node) {
    heap[index] = heap.back();
    heap.pop_back();

    //make appropriate changes to node_index
    node_index[index_node[heap.size()] - 1] = index; //
    node_index[index_node[index] - 1] = -1;          //
    //and index_node                                 //size decreased by 1 after pop, so no need for size() - 1
    index_node[index] = index_node[heap.size()];     //
    index_node[heap.size()] = -1;                    //

    //bubble up or down (as needed) to restore heap property
    if(index == heap.size()) return;
    if(index == 0 || heap[index] >= heap[(index - 1)/2])
        _bubble_down(heap, index, node_index, index_node);
    //if not called from root of heap, and replaced value violates
    //heap property with parent, call bubble_up instead of bubble_down
    else _bubble_up(heap, index, node_index, index_node);
}

void _bubble_up(vector<int> &heap, int index, int * node_index, int * index_node) {
    //if only the beginning of heap, no bubbling effect
    if(index == 0) return;

    int child_dist = heap[index];
    int parent_dist = heap[(index - 1)/2];

    //interchange child_dist and parent_dist until heap property restored
    while(child_dist < parent_dist) {

        heap[index] = parent_dist;
        heap[(index - 1)/2] = child_dist;

        //swap index in node_index (NODE_TO_INDEX mapping)
        node_index[index_node[index] - 1] = (index - 1)/2;
        node_index[index_node[(index - 1)/2] - 1] = index;

        //swap node in index_node (INDEX_TO_NODE mapping)
        int swap_node = index_node[index];
        index_node[index] = index_node[(index - 1)/2];
        index_node[(index - 1)/2] = swap_node;

        //Changes for next iteration of while loop
        index = (index - 1)/2;
        if(index == 0) return;
        child_dist = heap[index];
        parent_dist = heap[(index - 1)/2];
    }
}

void _bubble_down(vector<int> &heap, int index, int * node_index, int * index_node) {

    if((2*index + 1) > (heap.size() - 1)) return;
    if((2*index + 1) == (heap.size() - 1)) {
        int child_dist = heap[2*index + 1];
        int parent_dist = heap[index];
        if(parent_dist <= child_dist) return;

        //make appropriate changes if child_dist < parent_dist
        heap[index] = child_dist;
        heap[2*index + 1] = parent_dist;

        node_index[index_node[index] - 1] = 2*index + 1;
        node_index[index_node[2*index + 1] - 1] = index;
        int swap_node = index_node[index];
        index_node[index] = index_node[2*index + 1];
        index_node[2*index + 1] = swap_node;
        return;
    }
    if(heap[index] <= heap[2*index + 1] && heap[index] <= heap[2*index + 2]) return; //no changes required in this case

    //else, choose index of smaller of two, interchange, and then recall _bubble_down with new index value
    int index_of_smaller = (heap[2*index + 1] < heap[2*index + 2]) ? (2*index + 1) : (2*index + 2);
    int parent_dist = heap[index];
    heap[index] = heap[index_of_smaller];
    heap[index_of_smaller] = parent_dist;

    node_index[index_node[index] - 1] = index_of_smaller;
    node_index[index_node[index_of_smaller] - 1] = index;
    int swap_node = index_node[index];
    index_node[index] = index_node[index_of_smaller];
    index_node[index_of_smaller] = swap_node;

    _bubble_down(heap, index_of_smaller, node_index, index_node);
}

void delete_graph(Node * graph) {
    for(int i = 0; i < number_of_nodes; ++i)
        if(graph[i].next != nullptr) delete_list(graph[i].next);
    for(int i = 0; i < number_of_nodes; ++i)
        graph[i].next = nullptr;
    delete [] graph;
}

void delete_list(Node * header) {
    if(header->next != nullptr) delete_list(header->next);
    header->next = nullptr;
    delete header;
    header = nullptr;
}
