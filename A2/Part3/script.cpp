#include<stdio.h>
#include<time.h>
#include <bits/stdc++.h>
#include<fstream>
using namespace std;

int main(){
    int n = 1e3;
    int *arr;
    arr = new int[n];
    ofstream myfile;
    myfile.open("input.txt", ios::app);
    for(int i=0; i< n; i++){
        myfile << rand()%1000<<" ";

    }
    myfile.close();
}