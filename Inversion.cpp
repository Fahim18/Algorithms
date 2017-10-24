#include <iostream>
#include <fstream>

/* The following program reads integers from the file
   "array.txt" and calculates the number of inversions
   in the file. As is apparent, the program was tested
   on a file with 100,000 integers, with range
   [1, 100000]. So you can directly use it with a text
   file with the same name with less than or equal to
   100,000 integers, with the integers being in the same
   range as I used, without any problems.*/

using namespace std;

void mergesort(long int unsorted[], long int first, long int last);
void merge_step(long int sort_this[], long int l, long int m, long int r);

unsigned long int count_inversions = 0;

int main()
{
    long int size_of_array = 100000;

    long int arr[size_of_array];

    ifstream infile("array.txt");
    long int a, index_of_array = 0;
    while(infile >> a) {
        arr[index_of_array] = a;
        index_of_array++;
    }

    mergesort(arr, 0, size_of_array - 1);

    cout << count_inversions << endl;

    return 0;
}

void mergesort(long int unsorted[], long int first, long int last) {

    if(first == last) {
        return;
    }

    long int middle = (first + last)/2;

    mergesort(unsorted, first, middle);
    mergesort(unsorted, middle + 1, last);

    merge_step(unsorted, first, middle, last);

    return;
}

void merge_step(long int sort_this[], long int l, long int m, long int r) {
    long int left[m - l + 1];
    long int right[r - m];

    for(int i = l; i < (m+1); i++) {
        left[i - l] = sort_this[i];
    }

    for(int i = (m + 1); i < (r + 1); i++) {
        right[i - (m + 1)] = sort_this[i];
    }

    long int i = 0, j = 0;

    long int k = r + 1;

    for(long int b = l; b < k; b++) {

        if( i == (m - l + 1) ) {
            for(long int c = b; c < k; c++) {
                sort_this[c] = right[j];
                j++;
                if( j == (r - m) ) return;
            }
        }

        if ( j == (r - m) ) {
            for (long int c = b; c < k; c++) {
                sort_this[c] = left[i];
                i++;
                if( i == (m - l + 1) ) return;
            }
        }

        if(left[i] <= right[j]) {
            sort_this[b] = left[i];
            i++;
        }

        else {
            sort_this[b] = right[j];
            j++;
            count_inversions += (m - l + 1) - i;
        }
    }

    return;
}

