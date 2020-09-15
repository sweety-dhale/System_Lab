/*
*                       ELEVATOR CONTROL SYSTEM
*/

#include <fstream> 
#include <iostream>
#include <vector> 
#include <stack> 
#include <queue> 
#include <set> 
#include <iterator> 
#include <bits/stdc++.h>

#define MAXF 164
#define MINF 0

using namespace std;

vector<int> requestServed;
vector<vector<int>> floorRequests;
set<int> floorInQueue;

int findUpNext(int currFloor)
{
    int nextFloor=MAXF;
    for(auto it:floorInQueue)
        if(it<nextFloor && it>currFloor)
            nextFloor=it;
    return nextFloor;
}

int findDownNext(int currFloor)
{
    int nextFloor=MINF;
    for(auto it:floorInQueue)
        if(it>=nextFloor && it<currFloor)
            nextFloor=it;
    return nextFloor;
}

void addFloorToQueue(int floor)
{
    for(int i=0;i<floorRequests.size();i++)
    {
        if(floorRequests[i].size()>1 && floorRequests[i][0]==floor)
            for(int j=floorRequests[i].size()-1;j>0;j--)
                floorInQueue.insert(floorRequests[i][j]);
    }
}

int moveUpward(int currFloor)
{
    int nextFloor=findUpNext(currFloor);
    requestServed.push_back(nextFloor);
    floorInQueue.erase(nextFloor);
    addFloorToQueue(nextFloor);
    return nextFloor;
}

int moveDownward(int currFloor)
{
    int nextFloor=findDownNext(currFloor);
    requestServed.push_back(nextFloor);
    floorInQueue.erase(nextFloor);
    addFloorToQueue(nextFloor);
    return nextFloor;
}

int floorDiff(int curr, int nxt)
{
    int carry=0;
    if(curr==MAXF || curr==MINF || nxt==MAXF || nxt==MINF)
        carry=-1;
    if(nxt>curr)
        return nxt-curr+carry;
    else
        return curr-nxt+carry;
}

int totalMovements()
{
    int totMoves=0,reqSize=requestServed.size(),curr,nxt;
    for(int i=0;i<reqSize-1;i++)
    {
        curr=requestServed[i];
        nxt=requestServed[i+1];
        totMoves+=floorDiff(curr,nxt);
    }
    return totMoves;
}

int main(int argc, char *argv[])
{
    // input file containing page reference sequence
    // is not provided through command line
    if(argc!=3) 
    {
        cout<<"\n Improper argument count, please provide input file"<<endl;
        return -1;
    }

    ifstream fin;
    fin.open(argv[1]);
    string direction=argv[2];

    // if unable to open input file
    if(!fin)                                                    
    {
        cout<<"\n Unable to open input file"<<endl;
        return -1;
    }

    int req=0,dir;
    if(direction.compare("up")==0)
        dir=0;
    else
    if(direction.compare("down")==0)
        dir=1;
    else
    {
        cout<<" Invalid direction value"<<endl;
        return -1;
    }

    string str;
    while(getline(fin,str))
    {
        req++;
    }
    fin.close();

    char ch;
    int idx=0,f;
    floorRequests=vector<vector<int>> (req);
    fin.open(argv[1]);
    while(getline(fin,str))
    {
		istringstream line(str);                                // to split the input line into strings
        string floor,reqString;
        while(line>>floor)
        {
            f=stoi(floor);
            floorRequests[idx].push_back(f);
        }
        idx++;
    }

    // if(validateFile(floorRequests,req))
    //     return -1;

    idx=0;
    while(idx<req)
    {
        for(auto it:floorRequests[idx])
            cout<<" "<<it;
        cout<<endl;
        idx++;
    }

    int currFloor=floorRequests[0][0];
    requestServed.push_back(currFloor);
    cout<<"\n Initial Elevator floor position: "<<currFloor<<endl;
    cout<<" Request Queue at Floor "<<currFloor<<": ";
    while(floorRequests[0].size()>1)
    {
        floorInQueue.insert(floorRequests[0].back());
        floorRequests[0].pop_back();
    } 
    for(auto it:floorInQueue)
        cout<<" "<<it;
    cout<<endl<<endl;   
    int flag=0;
    while (!floorInQueue.empty())
    {
        switch (dir)
        {
            case 0:
                currFloor=moveUpward(currFloor);
                if(currFloor==MAXF)
                    dir=1;
                break;
            
            case 1:
                currFloor=moveDownward(currFloor);
                if(currFloor==MINF)
                    dir=0;
                break;
        }
        if(currFloor==MINF || currFloor==MAXF)
            continue;
        else
        {
            cout<<" Reached Floor: "<<currFloor<<endl;
            cout<<" Request Queue at Floor "<<currFloor<<": ";
            for(auto it:floorInQueue)
                cout<<" "<<it;
            cout<<endl<<endl; 
        }
    }

    int totalMoves=totalMovements();
    cout<<" Total movement of the Elevator: "<<totalMoves<<endl;
    cout<<" Order in which the floor requests are serviced: "<<endl;
    for(auto it:requestServed)
        if(it==MINF || it==MAXF)
            continue;
        else
            cout<<" "<<it;
    cout<<endl;
    return 0;
}