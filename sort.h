#include <iostream>
#include <vector>
#include <string>
using namespace std;

void mergepairs(vector<pair<string, int>>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<pair<string, int>> L(n1), R(n2);
    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
    int i = 0; // Initial index of first subvector
    int j = 0; // Initial index of second subvector
    int k = l; // Initial index of merged subvector
    while (i < n1 && j < n2) {
        if (L[i].first <= R[j].first) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Merge sort function
void mergeSortpairs(vector<pair<string, int>>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSortpairs(arr, l, m);
        mergeSortpairs(arr, m + 1, r);
        mergepairs(arr, l, m, r);
    }
}
