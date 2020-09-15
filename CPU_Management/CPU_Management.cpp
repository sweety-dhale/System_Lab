/*
*          CPU Management: Implementation of CPU Scheduling Algorithms
*/

#include <fstream> 
#include <iostream>
#include <vector> 
#include <stack> 
#include <queue> 
#include <bits/stdc++.h>

using namespace std;

int processes;
vector<int> burstTime,arrivalTime;

void addToQueue(int timer, vector<int> &executed, queue<int> &w_queue)
{
    for(int i=0;i<processes;i++)
        if(!executed[i] && arrivalTime[i]<=timer)
            w_queue.push(i);
}

int jobToSchedule(queue<int> &w_queue)
{
    int p_idx,b_time,process,burst;
    p_idx=w_queue.front();
    w_queue.pop();
    b_time=burstTime[p_idx];
    while(!w_queue.empty())
    {
        process=w_queue.front();
        w_queue.pop();
        burst=burstTime[process];
        if(burst<b_time)
        {
            b_time=burst;
            p_idx=process;
        }
    }
    return p_idx;
}

void NonPreemptiveSJF(float *avg_w, float *avg_t)
{
    int timer=0, p_index, p_Num=processes;
    queue<int> waitingQueue;
    vector<int> waitingTime,turnaroundTime,executed;
    waitingTime=vector<int>(processes,0);
    turnaroundTime=vector<int>(processes,0);
    executed=vector<int>(processes,0);


    while(p_Num>0)
    {
        addToQueue(timer,executed,waitingQueue);
        p_index=jobToSchedule(waitingQueue);
        executed[p_index]=1;
        waitingTime[p_index]=timer-arrivalTime[p_index];
        turnaroundTime[p_index]=burstTime[p_index]+waitingTime[p_index];
        cout<<" P"<<p_index+1<<":\t"<<timer;
        timer+=burstTime[p_index];
        cout<<" to "<<timer<<endl;
        p_Num--;
    }

    for(int i=0;i<processes;i++)
    {
        *avg_w+=waitingTime[i];
        *avg_t+=turnaroundTime[i];
    }
    *avg_w=((float)*avg_w)/processes;
    *avg_t=((float)*avg_t)/processes;
}

void  PreemptiveSJF(float *avg_w, float *avg_t)
{
    int timer=0, p_index, p_Num=processes, prevJob=-1,prevTime=0;
    queue<int> waitingQueue;
    vector<int> waitingTime,turnaroundTime,executed;
    waitingTime=vector<int>(processes,0);
    turnaroundTime=vector<int>(processes,0);
    executed=vector<int>(processes,0);

    for(int i=0;i<p_Num;i++)
        waitingTime[i]+=(-1)*burstTime[i];

    while(p_Num>0)
    {
        addToQueue(timer,executed,waitingQueue);
        p_index=jobToSchedule(waitingQueue);
        if(prevJob!=p_index)
        {
            if(prevJob!=(-1))
            {
                cout<<" P"<<prevJob+1<<":\t"<<prevTime<<" to "<<timer<<endl;
                prevTime=timer++;
                prevJob=p_index;
                burstTime[p_index]--;
                if(burstTime[p_index]==0)
                {
                    executed[p_index]=1;
                    turnaroundTime[p_index]=timer-arrivalTime[p_index];
                    waitingTime[p_index]+=turnaroundTime[p_index];
                    p_Num--;
                }
            }
            else
            {
                prevJob=p_index;
            }
            
        }
        else
        {
            timer++;
            burstTime[p_index]--;
            if(burstTime[p_index]==0)
            {
                executed[p_index]=1;
                turnaroundTime[p_index]=timer-arrivalTime[p_index];
                waitingTime[p_index]+=turnaroundTime[p_index];
                p_Num--;
            }
        }
    }

    cout<<" P"<<prevJob+1<<":\t"<<prevTime<<" to "<<timer<<endl;
    for(int i=0;i<processes;i++)
    {
        *avg_w+=waitingTime[i];
        *avg_t+=turnaroundTime[i];
    }
    *avg_w=((float)*avg_w)/processes;
    *avg_t=((float)*avg_t)/processes;
}

int main(int argc, char *argv[])
{
    // input file containing page reference sequence
    // is not provided through command line
    if(argc!=2) 
    {
        cout<<"\n Improper argument count, please provide input file"<<endl;
        return -1;
    }

    ifstream fin;
    fin.open(argv[1]);

    // if unable to open input file
    if(!fin)                                                    
    {
        cout<<"\n Unable to open input file"<<endl;
        return -1;
    }

    int b_Time,arr_Time;
    while(fin>>b_Time && fin>>arr_Time)
    {
        burstTime.push_back(b_Time);
        arrivalTime.push_back(arr_Time);
        processes++;
    }
    fin.close();
    
    for(int i=0;i<processes;i++)
    cout<<" P"<<i+1<<"\t"<<burstTime[i]<<"\t"<<arrivalTime[i]<<endl;

    float avgWaitTime_n,avgTurnTime_n;
    cout<<"\n Non-Preemptive SJF: "<<endl;
    NonPreemptiveSJF(&avgWaitTime_n,&avgTurnTime_n);
    cout<<" Average waiting time: "<<avgWaitTime_n<<endl;
    cout<<" Average turn around time: "<<avgTurnTime_n<<endl;

    float avgWaitTime_p,avgTurnTime_p;
    cout<<"\n Preemptive SJF: "<<endl;
    PreemptiveSJF(&avgWaitTime_p,&avgTurnTime_p);
    cout<<" Average waiting time: "<<avgWaitTime_p<<endl;
    cout<<" Average turn around time: "<<avgTurnTime_p<<endl;

    cout<<"\n Improvement of performance of preemptive over non-preemptive SJF scheduling:"<<endl;
    cout<<" In terms of waiting time = "<<100*(avgWaitTime_n-avgWaitTime_p)/avgWaitTime_n<<"%"<<endl;
    cout<<" In terms of turnaround time = "<<100*(avgTurnTime_n-avgTurnTime_p)/avgTurnTime_n<<"%"<<endl;

    return 0;
}