// CSci114_P3.cpp : This file contains the 'main' function. Program execution begins and ends there.
/* HOW TO EXECUTE PROGRAM
1. Type in command: ./a.exe n n n
Where n is some number

Example: ./a.exe 4 5 6
*/
#include <iostream>
#include <array>
#include <thread>
#include <string>
#include <fstream>
#include <mutex>
using namespace std;
mutex mtx;

/*
x = get # of columns for A and rows for B
Doing matrix mult for one element
*/
int a_b(int **A, int **B, int row, int col, int x) {
    int result = 0;
    for (int i = 0; i < x; i++) {
        //cout << row << " " << col << " " << i << endl;
        result = result + (A[row][i] * B[i][col]);
    }
    return result;
}

/*
Performing A x B = C
For each row in c, calculate value for each element
*/
void c(int **A, int **B, int **C, int row, int x, int ROW) {
    int result = 0;
    for (int i = 0; i < ROW; i++) {
        result = a_b(A,B,row,i,x);
        C[row][i] = result;
    }
}

int main(int argc, char** argv) {
    
    int m = stoi(argv[1]), n = stoi(argv[2]), k = stoi(argv[3]); 
    int** a, ** b;
    
    a = new int* [m];
    for (int d = 0; d < m; d++) a[m] = new int[n];
    
    b = new int* [n];
    for (int e = 0; e < n; e++) b[n] = new int[k];

    //Reading in the text files
    ifstream testFile;
    string file = "A.txt";
    testFile.open(file,ios::in);
    
    if (!testFile.is_open()) {
        cout << "Can't open file\n";
        return -1;
    }

    int x,row = 0,col = 0, ROW = m, COL = n;

    while (testFile >> x) {
        if (col == 0) a[row] = new int[col];
        
        //Matrix A
        a[row][col] = x;
        col++;
        if (col == COL) {
            col = 0;
            row++;
        }
        if (row == ROW) break;  
    }

    //Array A
    for (int p = 0; p < m; p++) {
        for (int q = 0; q < n; q++) {
            cout << a[p][q] << " ";
        }
        cout << endl;
    }

    testFile.close();

    //Working with B.txt
    file = "B.txt";
    testFile.open(file, ios::in);

    if (!testFile.is_open()) {
        cout << "Can't open file\n";
        return -1;
    }

    row = 0, col = 0, ROW = n, COL = k;

    while (testFile >> x) {
        if (col == 0) b[row] = new int[col];

        //Matrix B
        b[row][col] = x;
        col++;
        if (col == COL) {
            col = 0;
            row++;
        }
        if (row == ROW) break;
    }

    //Utilizing multi-threading

    //Array B
    for (int p = 0; p < n; p++) {
        //cout << arr[m][p] << " ";
        for (int q = 0; q < k; q++) {
            cout << b[p][q] << " ";
        }
        cout << endl;
    }

    testFile.close();
    
    thread thread1[m];
    int** arr = new int* [m];
    
    for (int i = 0; i < m; i++) arr[i] = new int[k];

    for (int j = 0; j < m; j++) {
        thread1[j] = thread(c, a, b, arr, j, m, k);
    }
    for (int l = 0; l < m; l++) thread1[l].join();

    //Array B
    for (int p = 0; p < m; p++) {
        for (int q = 0; q < k; q++) {
            cout << arr[p][q] << " ";
        }
        cout << endl;
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu