#include "std_lib_facilities.h"
using namespace std;

/*
 *      Median Maintenance:
 *      Maintain two heaps, one that supports extract_min,
 *      and another that supports extract_max. Balance
 *      them out whenever they are not equal in size. This
 *      makes sure that the median is always accessible with
 *      a single query. Median maintenance allows median
 *      finding in O(log n) time.
 */

constexpr int how_many_numbers = 10000;     //total number of numbers for assignment
constexpr int modulo = 10000;               //modulo for assignment

int compute_median(int number, vector<int> &low, vector<int> &high);
void _insert(int number, vector<int> &low, vector<int> &high);
void _balance_out(vector<int> &low, vector<int> &high);
void _bubble_up_max(vector<int> &low);
void _bubble_up_min(vector<int> &high);
void _bubble_down_max(vector<int> &low, int index);
void _bubble_down_min(vector<int> &high, int index);
int magnitude(int a);

int main()
{
    int sum_of_medians = 0;

    //initialize heaps. lower supports extract-max and higher supports extract-min
    vector<int> lower, higher;
    lower.reserve(how_many_numbers/2);
    higher.reserve(how_many_numbers/2);

    //Read input and compute median on the go
    string filename = "median.txt";        //Initialize it to name of file that contains input
    ifstream number_file(filename);
    string line;
    int next_number;

    while(getline(number_file, line)) {
        stringstream(line) >> next_number;

        //function compute_median gives k'th median after adding x_k
        sum_of_medians += compute_median(next_number, lower, higher);
    }

    number_file.close();

    cout << "Sum of medians (modulo " << modulo << ") is: " << sum_of_medians % modulo << endl;
    return 0;
}

int compute_median(int number, vector<int> &low, vector<int> &high) {
    _insert(number, low, high);

    //balance out heaps if they are not equal in size
    if(low.size() != high.size())
        if(magnitude(low.size() - high.size()) != 1) _balance_out(low, high);

    //determine what the median is depending on number of inputs so far
    int combined_heap_size = low.size() + high.size();
    if(combined_heap_size % 2 == 0) return low[0];
    else {
        if(low.size() > high.size()) return low[0];
        else return high[0];
    }
}

void _insert(int number, vector<int> &low, vector<int> &high) {

    //base cases for when both heaps are empty
    if(low.size() == 0 && high.size() == 0) {
        low.push_back(number);
        return;
    }

    if(number <= low[0]) {
        low.push_back(number);
        _bubble_up_max(low);
        return;
    }
    else {
        high.push_back(number);
        if(high.size() > 1) _bubble_up_min(high);
        return;
    }
}

//keeps heaps roughly equal in size, i.e. size difference no more than 1
void _balance_out(vector<int> &low, vector<int> &high) {

    //find out which heap is bigger
    string bigger = (low.size() > high.size()) ? "low" : "high";

    if(bigger == "low") {
        int transfer_val = low[0];
        low[0] = low.back();
        low.pop_back();
        high.push_back(transfer_val);
        _bubble_down_max(low, 0);
        _bubble_up_min(high);
    }

    if(bigger == "high") {
        int transfer_val = high[0];
        high[0] = high.back();
        high.pop_back();
        low.push_back(transfer_val);
        _bubble_down_min(high, 0);
        _bubble_up_max(low);
    }
}

void _bubble_up_max(vector<int> &low) {

    int index = low.size() - 1;
    int parent_index = (index - 1)/2;

    while(low[index] > low[parent_index]) {
        int swap_val = low[index];
        low[index] = low[parent_index];
        low[parent_index] = swap_val;

        //make changes for next iteration of loop
        if(parent_index == 0) return;
        index = parent_index;
        parent_index = (index - 1)/2;
    }
}

void _bubble_up_min(vector<int> &high) {

    int index = high.size() - 1;
    int parent_index = (index - 1)/2;

    while(high[index] < high[parent_index]) {
        int swap_val = high[index];
        high[index] = high[parent_index];
        high[parent_index] = swap_val;

        //make changes for next iteration of loop
        if(parent_index == 0) return;
        index = parent_index;
        parent_index = (index - 1)/2;
    }
}

void _bubble_down_max(vector<int> &low, int index) {

    if((2*index + 1) > (low.size() - 1)) return;    //2i+1 being out automatically implies 2i+2 being out
    if((2*index + 1) == (low.size() - 1)) {
        //if heap property maintained, nothing to do
        if(low[index] >= low[2*index + 1]) return;

        //else, swap
        int swap_val = low[index];
        low[index] = low[2*index + 1];
        low[2*index + 1] = swap_val;
        return;
    }
    if(low[index] >= low[2*index + 1] && low[index] >= low[2*index + 2]) return;

    int index_of_bigger = (low[2*index + 1] > low[2*index + 2]) ? 2*index + 1 : 2*index + 2;
    int swap_val = low[index];
    low[index] = low[index_of_bigger];
    low[index_of_bigger] = swap_val;

    _bubble_down_max(low, index_of_bigger);
}

void _bubble_down_min(vector<int> &high, int index) {

    if((2*index + 1) > (high.size() - 1)) return;
    if((2*index + 1) == (high.size() - 1)) {
        if(high[index] <= high[2*index + 1]) return;

        int swap_val = high[index];
        high[index] = high[2*index + 1];
        high[2*index + 1] = swap_val;
        return;
    }
    if(high[index] <= high[2*index + 1] && high[index] <= high[2*index + 2]) return;

    int index_of_smaller = (high[2*index + 1] < high[2*index + 2]) ? (2*index + 1) : (2*index + 2);
    int swap_val = high[index];
    high[index] = high[index_of_smaller];
    high[index_of_smaller] = swap_val;

    _bubble_down_min(high, index_of_smaller);
}

int magnitude(int a) {
    if(a > 0) return a;
    return -a;
}
