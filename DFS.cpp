/*
    Outputs the number of nodes in the five biggest strongly
    connected components of a graph. In this case, the number 
    of nodes was 875714, loaded into a linked list from a text
    file named "SCC.txt" with each line containing one node, 
    ie ( a b ) -> (a,b). Load() implemented with DFS in mind so 
    it automatically uploads reverse graph. rLoad() then reverses 
    it to get original. 
    
    Run time was 31 seconds on a computer with specs:
    Windows, 4gb RAM, i3, 64 bit OS, 2.00 GHz
    Stack size: 32 mb
*/

#include "std_lib_facilities.h"

//Number of nodes of graph
constexpr int size_of_graph = 875714;  //875714 for assignment

int counter = 1;    //keep track of post-values
int nth_cc = 0;     //keep track of the number of connected components

struct Node {
    int vertex;
    Node * next;
};

Node ** Load(string file);
Node ** rLoad(Node ** head);
void dfs(Node ** head, int * post);
void dfs2(Node ** head, int * post, int * scc);
void explore_component(Node ** head, int i, bool * explored, int * post, int a, int * scc = nullptr);
    //call with a = 1 implies DFS on reverse graph,
    //and a call with a = 2 implies DFS on straight graph
void delete_graph(Node ** head);
void delete_list(Node * head);

int main()
{
    string file_name = "SCC.txt"; //"SCC.txt" for assignment

    int * post = new (nothrow) int [size_of_graph + 1]; //record post-values

    //Load reverse graph from text file
    Node ** rgraph = Load(file_name);


    //call dfs on reverse graph
    dfs(rgraph, post);

    //arrange nodes in increasing order of post values
    int * post_ordering = new (nothrow) int [size_of_graph + 1];
    post_ordering[0] = 0;
    for(int i = 1; i < size_of_graph + 1; ++i)
        post_ordering[post[i]] = i;

    //free memory after completion
    delete [] post;

    //Load graph from text file
    Node ** graph = rLoad(rgraph);

    delete_graph(rgraph);

    //array to figure out which cc the node is a part of
    int * cc_val = new (nothrow) int [size_of_graph + 1];
    cc_val[0] = 0;

    //fill cc_val[] with call to dfs on straight graph
    dfs2(graph, post_ordering, cc_val);

    //free memory for post_ordering after completion
    delete [] post_ordering;

    //Determine size of each SCC
    int * nodes_per_cc = new (nothrow) int [nth_cc];
    for(int i = 0; i < nth_cc; ++i) nodes_per_cc[i] = 0;
    for(int i = 1; i < size_of_graph + 1; ++i)
        nodes_per_cc[cc_val[i] - 1]++;

    //Print number of nodes per SCC in reverse order
    vector <int> percc;
    for(int i = 0; i < nth_cc; ++i) percc.push_back(nodes_per_cc[i]);
    sort(percc);
    vector <int> :: reverse_iterator ir;
    for(ir = percc.rbegin(); ir != percc.rbegin() + 5; ++ir)
        cout << *ir << ",";

    delete [] cc_val;           //
    delete [] nodes_per_cc;     //   ----   free memory   ----
    delete_graph(graph);        //

    return 0;
}

//Load linked list from file
Node ** Load(string file) {

    //allocate memory for array of pointers to Nodes
    Node ** graph = new (nothrow) Node * [size_of_graph + 1];
    if(graph == nullptr) {
        cerr << "Error: could not allocate memory for graph\n";
        exit(1);
    }
    for(int i = 0; i < size_of_graph + 1; ++i) graph[i] = nullptr;

    //Read from file
    ifstream myfile(file);
    string line;
    int a, b;
    while(getline(myfile, line)) {

        //each edge written as (a, b)
        stringstream(line) >> a >> b;

        //called for reverse graph so (a, b) reversed
        Node * new_edge = new (nothrow) Node;
        new_edge->vertex = a;
        new_edge->next = graph[b];
        graph[b] = new_edge;
    }
    myfile.close();

    //Return array of pointers to linked lists
    return graph;
}

//given graph, return reverse graph
Node ** rLoad(Node ** head) {

    Node ** rgraph = new (nothrow) Node * [size_of_graph + 1];
    if(rgraph == nullptr) {
        cerr << "Error: Memory for graph could not be allocated\n";
        exit(4);
    }
    for(int i = 0; i < size_of_graph + 1; ++i) rgraph[i] = nullptr;

    //load reverse graph of graph provided
    //in this case, called on reverse, so returns straight
    for(int i = 1; i < size_of_graph + 1; ++i) {
        Node * tracker = head[i];
        while(tracker != nullptr) {
            Node * new_edge = new (nothrow) Node;
            new_edge->vertex = i;
            new_edge->next = rgraph[tracker->vertex];
            rgraph[tracker->vertex] = new_edge;
            tracker = tracker->next;
        }
    }

    return rgraph;
}

//Depth-First-Search
void dfs(Node ** head, int * post) {

    //Initialize a boolean array for all nodes
    bool * explored = new (nothrow) bool [size_of_graph + 1];
    if(!explored) {
        cerr << "Error: memory allocation for boolean array failed\n";
        exit(2);
    }

    for(int i = 0; i < size_of_graph + 1; ++i) explored[i] = false;

    //Run dfs for each strongly connected component
    for(int i = 1; i < size_of_graph + 1; ++i)
        if(explored[i] == false) explore_component(head, i, explored, post, 1);

    //free memory for boolean array
    delete [] explored;
}

//DFS2 for straight graph, logic similar to above
void dfs2(Node ** head, int * post, int * scc) {

    bool * explored = new (nothrow) bool [size_of_graph + 1];
    if(!explored) {
        cerr << "Error: memory allocation for boolean array failed\n";
        exit(3);
    }
    for(int i = 0; i < size_of_graph + 1; ++i) explored[i] = false;

    for(int i = size_of_graph; i > 0; i--) {
        if(explored[post[i]] == false) {
            nth_cc++; //increment number of cc's found
            explore_component(head, post[i], explored, post, 2, scc);
        }
    }

    delete [] explored;
}

//DFS for a strongly connected component
void explore_component(Node ** head, int i, bool * explored, int * post, int a, int * scc) {

    //mark current node as explored
    explored[i] = true;

    Node * tracker = head[i];
    while(tracker != nullptr) {
        if(explored[tracker->vertex] == false)
            explore_component(head, tracker->vertex, explored, post, a, scc);
        tracker = tracker->next;
    }

    if(a == 1) {
        //keep track of post values
        post[i] = counter;
        counter++;
    }

    if (a == 2) {
        //keep track of which cc current node is part of
        scc[i] = nth_cc;
    }
}

//delete entire graph (deallocate assigned memory from heap)
void delete_graph(Node ** head) {
    for(int i = 1; i < size_of_graph + 1; ++i) {
        if(head[i] != nullptr) delete_list(head[i]);
    }
    delete [] head;
}

//delete linked list connected to each head pointer
void delete_list(Node * head) {
    if(head->next != nullptr) delete_list(head->next);
    delete head;
    head = nullptr;
}
