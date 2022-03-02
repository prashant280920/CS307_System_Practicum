#include <iostream>
#include<bits/stdc++.h>
#include <thread>
#include <semaphore.h>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>
#include <unistd.h>
#include <unordered_map>
using namespace std;

int n;
int** Matrix1;
int** Matrix2;
int** MatrixNormal;
int** MatrixThread;

void InitialiseMatrix(int **Matrix){
    for (int i = 0; i < n; i++) {
        Matrix[i] = new int[n];
    }
    for(int i=0; i< n; i++){
        for(int j=0; j< n; j++){
            int val = rand()%10;
            Matrix[i][j] = val;
        }
    }
}
void PrintMatrix(int ** Matrix){
    for(int i=0; i< n; i++){
        for(int j=0; j< n; j++){
            cout << Matrix[i][j] <<" ";
        }
        cout << endl;
    }
    cout << endl;
}

void thread_Multiplication(int row1, int row2 ){
    for(int i = row1; i< row2; i++){
        for(int j=0; j< n; j++){
            for(int k = 0; k< n; k++){
                MatrixThread[i][j] += (Matrix1[i][k] * Matrix2[k][j]);
            }
        }
    }
}

void MatMulNormal(){
    for(int i=0; i< n; i++){
        for(int j=0; j< n; j++){
            for(int k = 0; k< n; k++){
                MatrixNormal[i][j]+= Matrix1[i][k] * Matrix2[k][j];
            }
        }
    }
}

void MatMulThread(int n_thread){
    int i=0; int j = n/n_thread;
    thread threads[n_thread];
    for(int k=0; k<n_thread; k++){
        threads[k]=thread(thread_Multiplication,i,j);
        j += n/n_thread;
        i += n/n_thread;
    }
    for(int k=0; k<n_thread; k++){
        threads[k].join();
    }
}


int main(int argc, char *argv[]){
    if(argc == 1){
        cout <<"Please provide the size of the matrix"<<endl;
        return 0;
    }
    if(argc > 2 ){
        cout <<"Please provide only one positive integer"<<endl;
        return 0;
    }
    
    n = atoi(argv[1]);
    //Initializing Matrix 1.
    Matrix1 = new int*[n];
    InitialiseMatrix(Matrix1);

    //Initializing Matrix 2.
    Matrix2 = new int*[n];
    InitialiseMatrix(Matrix2);

    //Initializing Matrix Normal, which is calculated using sequentially.
    MatrixNormal = new int*[n];
    for (int i = 0; i < n; i++) {
        MatrixNormal[i] = new int[n];
    }

    //Initializing Matrix Thread, which is calculated using thread.
    MatrixThread = new int*[n];
    for (int i = 0; i < n; i++) {
            MatrixThread[i] = new int[n];
    }
    // Calling function to multiply matrix sequentially.
    // time_t startSequential, endSequential;
    // time_t startThreading, endThreading;
    struct timespec startSequential, endSequential;
    clock_gettime(CLOCK_MONOTONIC, &startSequential);
    MatMulNormal();
    clock_gettime(CLOCK_MONOTONIC, &endSequential);

    // Calling function to multiply using thread.
    struct timespec startThreading, endThreading;
    clock_gettime(CLOCK_MONOTONIC, &startThreading);
    MatMulThread(4);
    clock_gettime(CLOCK_MONOTONIC, &endThreading);

    double timeSequential = (endSequential.tv_sec  - startSequential.tv_sec)* 1e9;
    timeSequential = (timeSequential + (endSequential.tv_nsec  - startSequential.tv_nsec))* 1e-9;
    double timeThreading = (endThreading.tv_sec  - startThreading.tv_sec)* 1e9;
    timeThreading = (timeThreading + (endThreading.tv_nsec - startThreading.tv_nsec))* 1e-9;
    cout << "Time taken by sequential Multiplication : "
         << fixed << timeSequential << setprecision(9)<<" seconds"<<endl;
    cout << "Time taken by parallel Multiplication : "
         << fixed << timeThreading << setprecision(9)<<" seconds"<<endl;

    // PrintMatrix(Matrix1);
    // PrintMatrix(Matrix2);
    // PrintMatrix(MatrixNormal);
    // PrintMatrix(MatrixThread);

    return 0;
}