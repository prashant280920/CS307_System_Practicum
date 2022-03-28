#include <bits/stdc++.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <unistd.h>
#include <ctime>
#define vi vector<int>
#define vvi vector<vi>
using namespace std;
mutex lck;
mutex qlk;
int currThreads=0;

class List{
    public:
        int val;
        List * next = NULL;

        List(int num){
            val=num;
        }
};

List* mergeList(List* head1 , List* head2);

List* addNode(List* head, int num){
    auto newHead = new List(num);
    newHead->next = head;
    return newHead;
}

List* sortList2(List* head){
    if(head->next==NULL){
        return head;
    }
    List* first=head;
    List* second=head;
    while(second!=NULL and second->next!=NULL and second->next->next!=NULL){
        first=first->next;
        second=second->next->next;
    }
    auto temp=first->next;
    first->next=NULL;
    first=sortList2(head);
    second=sortList2(temp);
    auto newHead = mergeList(first,second);
    return newHead;
}

void sortList(List* head, vector<List*>&v, int i){
    // vi temp;
    // auto savehead = head;
    // while(head){
    //     temp.push_back(head->val);
    //     head=head->next;
    // }
    // sort(temp.begin(),temp.end());
    // head=savehead;
    // for(auto x: temp){
    //     head->val = x;
    //     head=head->next;
    // }
    v[i]=sortList2(head);
}

List* mergeList(List* head1 , List* head2){
    if(head1==NULL){
        return head2;
    }

    if(head2==NULL){
        return head1;
    }

    if(head1==NULL && head2==NULL){
        return NULL;
    }

    List* head = new List(0);
    List* temp = head;

    while(head1!=NULL && head2!=NULL){
        if(head1->val <= head2->val){
            temp->next = head1;
            head1=head1->next;
        }
        else{
            temp->next = head2;
            head2=head2->next; 
        }
        temp=temp->next;
    }

    if(head1!=NULL){
        temp->next = head1;
    }

    if(head2!=NULL){
        temp->next = head2;
    }
    return head->next;
}

List * toList(vi &v){
    auto head = new List(v[0]);
    for(int i=1; i<v.size(); i++){
        head = addNode(head,v[i]);
    }
    return head;
}

void print(List * head){
    ofstream output;
    output.open("output.txt", ios::app);
    while(head){
        output <<head->val<<" ";
        head=head->next;
    }
    output.close();
}

int giveVal(){
    lck.lock();
    int x = currThreads;
    lck.unlock();
    return x;
}

vector<List *> give(string &inPath, int nThreads){
    vector<List *>ans(nThreads,NULL);
    int i=0;
    ifstream iFile(inPath);
    while(!iFile.eof()){
        int x;
        iFile>>x;
        if(iFile.eof()) break;
        ans[i]=addNode(ans[i],x);
        i++;
        if(i==nThreads){
            i=0;
        }
    }
    return ans;
}

int main(int argc, char** argv){

    if(argc !=4){
        cout<<"Please check the assignment :("<<endl;
        return 0;
    }
    int nThreads = stoi(argv[1]);
    string inPath = argv[2];
    string ouPath = argv[3];
    ifstream iFile(inPath);
    vector<List *>temp = give(inPath,nThreads);
    // for(auto head: temp){
    //     print(head);
    //     cout<<endl;
    // }
    // sortList(temp[0]);
    // sortList(temp[1]);
    // auto head = merge(temp[0],temp[1]);
    // print(head);
    const clock_t time = clock();
    thread threads[nThreads];
    for(int i=0; i<nThreads; i++){
        // threads[i]=thread(sortList,temp[i],ref(temp),i);
        sortList(temp[i],temp,i);
    }
    // for(int i=0; i<nThreads; i++){
    //     threads[i].join();
    // }
    queue<List*>q;
    queue<thread>qt;
    for(auto &x: temp){
        q.push(x);
    }
    do{
        auto head1=q.front();
        q.pop();
        List * head2;
        if(!q.empty()){
            head2 = q.front();
            q.pop();
            q.push(mergeList(head1,head2));
        }
        else{
            q.push(head1);
        }
    }
    while( q.size()!=1);
    cout<<"Execution Time is "<<float(clock()-time)/CLOCKS_PER_SEC<<endl;
    //print(q.front());
    
    auto head = q.front();
    
    print(head) ;
    



    return 0;
}