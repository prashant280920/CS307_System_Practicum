#include <iostream>
#include <thread>
#include <semaphore.h>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>
#include <unistd.h>
#include <unordered_map>
using namespace std;

mutex spoons[5];
mutex printPer;
mutex draw;
mutex printState;

int eats[5]={0};
unordered_map<int, vector<int>> count_state;

void isEating(int student_i){
    printPer.lock();
    cout<<"S"<<student_i+1<<": Both spoons acquired."<<endl;
    printPer.unlock();
    this_thread::sleep_for(chrono::milliseconds(20*1000));
    count_state[student_i][0]++;
}

void isThinking(int student_i){
    printPer.lock();
    cout<<"S"<<student_i+1<<": Thinking"<<endl;
    printPer.unlock();
    // unsigned int ms = rand()%2000;
    this_thread::sleep_for(chrono::milliseconds(1500));
    count_state[student_i][1]++;
}
void isWaiting(int student_i){
    printPer.lock();
    cout<<"S"<<student_i+1<<": Waiting for spoons"<<endl;
    count_state[student_i][2]++;
    printPer.unlock();
}
//DeadLock is possible in below function. 
void diningTable_1(int student_i){

    while(true){
        int left = student_i, right = (student_i+1)%5;

        isWaiting(student_i);
        //draw.lock();
        spoons[right].lock();
        spoons[left].lock();
        isEating(student_i);
        eats[student_i]++;
        spoons[right].unlock();
        spoons[left].unlock();
        isThinking(student_i);
        //draw.unlock();

        printState.lock();
        cout << "------------------------------"<<endl;
        cout << "         Eat "<< " Think "<<" Wait "<<endl; 
        for(auto i: count_state){


            cout << "S"<< i.first + 1 <<": "
            <<"      " << i.second[0] << "     " 
            << i.second[1] << "     "<<i.second[2] << endl;
        }
        cout << "------------------------------"<<endl;
        printState.unlock();
    }
}
//This Function Prevents Deadlock;
void diningTable_2(int student_i){

    while(true){
        int left = student_i, right = (student_i+1)%5;

        isWaiting(student_i);
        draw.lock();
        spoons[right].lock();
        spoons[left].lock();
        draw.unlock();
        isEating(student_i);
        eats[student_i]++;
        spoons[right].unlock();
        spoons[left].unlock();
        isThinking(student_i);


        printState.lock();
        cout << "------------------------------"<<endl;
        cout << "         Eat "<< " Think "<<" Wait "<<endl; 
        for(auto i: count_state){


            cout << "S"<< i.first + 1 <<": "
            <<"      " << i.second[0] << "     " 
            << i.second[1] << "     "<<i.second[2] << endl;
        }
        cout << "------------------------------"<<endl;
        printState.unlock();
    }
}
//Here Also Deadlock is not possible.
void diningTable_3(int student_i){

    while(true){
        int left = student_i, right = (student_i+1)%5;

        isWaiting(student_i);
        if(left == 4){
            spoons[left].lock();
            spoons[right].lock();
        }
        else{
            spoons[right].lock();
            spoons[left].lock();
        }

        isEating(student_i);
        eats[student_i]++;
        spoons[right].unlock();
        spoons[left].unlock();
        isThinking(student_i);
      

        printState.lock();
        cout << "------------------------------"<<endl;
        cout << "         Eat "<< " Think "<<" Wait "<<endl; 
        for(auto i: count_state){


            cout << "S"<< i.first + 1 <<": "
            <<"      " << i.second[0] << "     " 
            << i.second[1] << "     "<<i.second[2] << endl;
        }
        cout << "------------------------------"<<endl;
        printState.unlock();
    }
}
int main()
{       

    for(int i=0; i< 5; i++){
        vector<int> temp = {0,0,0};
        count_state[i]= temp;
    }
    thread threads[5];
        for(int i=0; i<5; i++){
        threads[i]=thread(diningTable_1,i);
    }
    for(int i=0; i<5; i++){
        threads[i].join();
    }
    cout<<endl;
    // for(int i=0; i<5; i++){
    //     cout<<eats[i]<<" ";
    // }
    // cout<<endl;
    
    return 0;
}