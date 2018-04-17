/*
 *      This is an implementation of huffman's
 *      encoding algorithm. Due to minimum
 *      computations, a heap is used to store the
 *      symbols. The format of the file is:
 *      [number of symbols]
 *      [weight of symbol 1]
 *      ....
 *      The assignment only asked for the length
 *      of the smallest and  the largest encodings
 *      so the individual codes were not computed.
 *      Can be done recursively.
 */

#include "std_lib_facilities.h"
#include <sstream>
#include <algorithm>

using namespace std;

struct symbol {
    string _symbol;           //stores integer for actual symbol
    long long weight;
    symbol * left_child, * right_child;
};

void _insert(vector<symbol *> &heap, symbol * new_symbol);
symbol * extract_min(vector<symbol *> &heap);
void _bubble_up(vector<symbol *> &heap);
void _bubble_down(vector<symbol *> &heap, int index);
void encoding_length(symbol * root, vector<int> &lengths, int current_level);
void delete_tree(symbol * root);

int main()
{
    int number_of_symbols, current_symbol = 1;
    long long current_weight;
    string filename = "huffman.txt", line;
    vector<symbol *> huffman_tree;

    ifstream encoding_file(filename);
    getline(encoding_file, line);
    stringstream(line) >> number_of_symbols;
    huffman_tree.reserve(number_of_symbols);

    while(getline(encoding_file, line)) {
        stringstream(line) >> current_weight;

        //convert "current_symbol" from int to string -- process finishes at symbol_rep.str()
        ostringstream symbol_rep;
        symbol_rep << current_symbol;

        symbol * new_symbol = new (nothrow) symbol;
        * new_symbol = { symbol_rep.str(), current_weight, nullptr, nullptr };
        _insert(huffman_tree, new_symbol);

        ++current_symbol;
    }

    encoding_file.close();

    symbol * first, * second;

    //Main loop ends when single tree formed.
    while(huffman_tree.size() != 1) {
        //pluck off two smallest sub-trees and merge
        first = extract_min(huffman_tree);
        second = extract_min(huffman_tree);

        //create a dummy node and append 'first' and 'second'
        //as its children
        symbol * dummy = new (nothrow) symbol;
        dummy->weight = first->weight + second->weight;
        dummy->left_child = first;
        dummy->right_child = second;

        //insert dummy node into the heap
        _insert(huffman_tree, dummy);
    }

    vector<int> lengths;
    encoding_length(huffman_tree[0], lengths, 0);
    sort(lengths.begin(), lengths.end());

    cout << "Smallest encoding length: " << lengths.front() << endl
         << "Largest encoding length: " << lengths.back() << endl;

    delete_tree(huffman_tree[0]);
    return 0;
}

void _insert(vector<symbol *> &heap, symbol * new_symbol) {
    heap.push_back(new_symbol);
    _bubble_up(heap);
}

symbol * extract_min(vector<symbol *> &heap) {
    symbol * minimum = heap[0];

    heap[0] = heap.back();
    heap.pop_back();

    if(heap.size() != 0) _bubble_down(heap, 0);
    return minimum;
}

void _bubble_up(vector<symbol *> &heap) {
    int index = heap.size() - 1, parent_index;
    while(index != 0) {
        parent_index = (index - 1)/2;
        if(heap[index]->weight < heap[parent_index]->weight) {
            symbol * swap_symbol = heap[index];
            heap[index] = heap[parent_index];
            heap[parent_index] = swap_symbol;
        }
        index = parent_index;
    }
}

void _bubble_down(vector<symbol *> &heap, int index) {
    if((2*index + 1) > (heap.size() - 1)) return;
    if((2*index + 1) == (heap.size() - 1)) {
        if(heap[index]->weight > heap[2*index + 1]->weight) {
            symbol * swap_symbol = heap[index];
            heap[index] = heap[2*index + 1];
            heap[2*index + 1] = swap_symbol;
        }
        return;
    }
    if(heap[index]->weight <= heap[2*index + 1]->weight &&
        heap[index]->weight <= heap[2*index + 2]->weight) return;

    int index_of_smaller = (heap[2*index + 1]->weight <= heap[2*index + 2]->weight) ? (2*index + 1) : (2*index + 2);
    symbol * swap_symbol = heap[index];
    heap[index] = heap[index_of_smaller];
    heap[index_of_smaller] = swap_symbol;

    _bubble_down(heap, index_of_smaller);
}

//Use of recursion to compute all the encoding lengths. will be duplicates in vector 'lengths'
void encoding_length(symbol * root, vector<int> &lengths, int current_level) {

    if(root->left_child == nullptr && root->right_child == nullptr) {
        lengths.push_back(current_level);
        return;
    }

    encoding_length(root->left_child, lengths, current_level + 1);
    encoding_length(root->right_child, lengths, current_level + 1);
}

//Delete the tree by freeing memory for all allocated symbols
void delete_tree(symbol * root) {
    if(root == nullptr) return;

    delete_tree(root->left_child);
    delete_tree(root->right_child);

    root->left_child = nullptr;
    root->right_child = nullptr;
    delete root;
    root = nullptr;
}
