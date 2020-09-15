/*
*          Memory Management: Implementation of Page Replacement Algorithms
*/

#include<fstream> 
#include<iostream>
#include<vector> 
#include <stack> 
#include<bits/stdc++.h>

using namespace std;

// function to search if required page is present in frames or not
// and return -1 if page is absent
int pageSearch(int reqPage, vector<int> &frames,int start, int end)
{
    for(int i=start;i<end;i++)
    {
        if(reqPage==frames[i])
        return i;
    }
    return -1;
}

// function to return the index of which frame to replace with new page
// whenever there is page fault
int findPlaceOpt(vector<int> &frames, vector<int> &refSeq, int start, int rSize)
{
    int fSize=frames.size();
    int pos1=0,pos2=0,idx=0;
    for(int i=0;i<fSize;i++)
    {
        // check whether page is there in frames or not
        pos1=pageSearch(frames[i],refSeq,start,rSize);
        if(pos1==(-1)) 
        {
            // if some page is not required further then 
            // the required page is replaced with that frame
            return i;
        }
        else
        if((pos1>pos2))
        {
            // that frames which is not required for long
            // is replaced with the required one
            pos2=pos1;
            idx=i;
        }   
    }
    return idx;
}

// function to implement OPTIMAL page replacement algorithm
int optimal(vector<int> &refSeq, int refNum, int fNum)
{
    vector<int> frames= vector<int>(fNum,-1);
    int idx, pos, pageFaults=0;
    for(int i=0;i<refNum;i++)
    {
        // prints which page to search and contents
        // of the frames
        cout<<"   Page: "<<refSeq[i]<<"\t Frames:";
        for(auto it:frames)
            cout<<"   "<<it;

        // check if required page is present in frames
        idx=pageSearch(refSeq[i],frames,0,fNum);
        if(idx==(-1))
        {
            cout<<"   *"<<endl;
            // increment page faults if page not found
            pageFaults++;
            // to get the index of frame to be replaced
            pos=findPlaceOpt(frames,refSeq,i+1,refNum);
            frames[pos]=refSeq[i];
        }
        else
        {
            cout<<endl;
        }
        
    }
    return pageFaults;
}

// function to find the frame to be replaced with new one
void findPlaceLRU(vector<int> &refSeq, int idx, vector<int> &frames, int fNum, int flag)
{
    int page=refSeq[idx];
    stack<int> fstack;
    if(flag)
        for(int i=fNum-1;i>=0;i--)
        {
            // push the contents of frame to stack until
            // required page found and pop back the contents
            // of stack to frame and keep recently used page
            // on top of frame
            if(page==frames[i])
            {
                while(!fstack.empty())
                {
                    frames[i]=fstack.top();
                    fstack.pop();
                    i++;
                }
                frames[i]=page;
                return;
            }
            else
            {
                fstack.push(frames[i]);
            }      
        }
    else
    {
        int i;
        for(i=fNum-1;i>0;i--)
        {
            fstack.push(frames[i]);
        }
        while(!fstack.empty())
        {
            frames[i]=fstack.top();
            fstack.pop();
            i++;
        }
        frames[i]=page;
    }
}

// function to implement LRU page replacement algorithm
int LRU(vector<int> &refSeq, int refNum, int fNum)
{
    vector<int> frames=vector<int>(fNum,-1);
    int idx,pos,pageFaults=0;

    // to print the required page and contents of the frames
    // until frames get filled
    for(int i=0;i<fNum;i++)
    {
        cout<<"   Page: "<<refSeq[i]<<"\t Frames:";
        for(auto it:frames)
            cout<<"   "<<it;
        cout<<"   *"<<endl;
        frames[i]=refSeq[i];
        pageFaults++;
    }

    // to print the required page and contents of the frames
    // and also it search for the required page in the frames 
    // if page is found then check for next else replace with
    // the least recently used page
    for(int i=fNum;i<refNum;i++)
    {
        // search for required page
        idx=pageSearch(refSeq[i],frames,0,fNum);
        // check if page is found
        if(idx==(-1))
        {
            cout<<"   Page: "<<refSeq[i]<<"\t Frames:";
            for(auto it:frames)
                cout<<"   "<<it;
            cout<<"   *"<<endl;
            // increment page fault if page is not there
            pageFaults++;
            // choose frame which needs to be replaced
            findPlaceLRU(refSeq,i,frames,fNum,0);
        }
        else
        {
            cout<<"   Page: "<<refSeq[i]<<"\t Frames:";
            for(auto it:frames)
                cout<<"   "<<it;
            cout<<endl;
            // choose frame which needs to be replaced
            findPlaceLRU(refSeq,i,frames,fNum,1);
        }   
    }
    // return number of page faults occured in case of LRU 
    return pageFaults;
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

    int refNum=0, page;
    vector<int> refSeq;
    // read input file which contains page requests
    // and store it in vector refSeq 
    while(fin>>page)
    {
        refSeq.push_back(page);
        refNum++;
    }
    fin.close();
    
    // prints page requests
    cout<<" Page request sequence: "<<endl;
    for(int i=0;i<refNum;i++)
    cout<<" "<<refSeq[i];
    cout<<endl;

    // take input for number of frames
    cout<<" Enter number of frames: ";
    int fNum, pageFaults=0;
    cin>>fNum;

    // for optimal approach
    pageFaults=optimal(refSeq,refNum,fNum);
    cout<<" Page faults for Optimal are: "<<pageFaults<<endl;
    cout<<" Page fault ratio in case of optimal algorithm: "<<((float)pageFaults/refNum)<<endl;

    // for LRU approach
    pageFaults=LRU(refSeq,refNum,fNum);
    cout<<" Page faults for LRU are: "<<pageFaults<<endl;
    cout<<" Page fault ratio in case of LRU algorithm: "<<((float)pageFaults/refNum)<<endl;

    return 0;
}