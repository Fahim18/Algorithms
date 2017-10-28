#include <iostream>
#include <fstream>

using namespace std;

/*This is the QuickSort Algorithm, applied to
  an array of 10000 numbers in the range
  [1, 10000]. The array was loaded from a text
  file named "array2.txt" where each number
  was in a seperate line. The long integer
  'compare' computes the number of comparisons made.
  */

void QuickSort(int x[], int l, int r);
int Partition_array(int y[], int b, int e);

//Variable to count number of comparisons
long int compare = 0;

int main() {
    int sizeofarray = 10000;

    int A[sizeofarray];

    ifstream infile("array2.txt");
    int a, index_of_array = 0;
    while(infile >> a) {
        A[index_of_array] = a;
        index_of_array++;
    }

    QuickSort(A, 0, sizeofarray - 1);

    cout << compare << endl;

    return 0;
}

void QuickSort(int x[], int l, int r) {

    //Handle base case
    if(l >= r) {
        return;
    }

    //Increment by number of comparisons per call
    compare += r - l;

    //Partition around pivot A[l]
    int index = Partition_array(x, l, r);

    QuickSort(x, l, index - 1);
    QuickSort(x, index + 1, r);
    return;
}

int Partition_array(int y[], int b, int e) {
    int p = y[b];

    int i = b + 1;

    for(int j = b + 1; j < (e + 1); j++) {
        if(y[j] < p) {
            int q = y[j];
            y[j] = y[i];
            y[i] = q;
            i++;
        }
    }

    int swapp = y[i - 1];
    y[i - 1] = y[b];
    y[b] = swapp;

    int index_of_pivot = i - 1;

    return index_of_pivot;
}

