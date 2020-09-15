#include <stdio.h> 
#include <fstream>
#include <iostream>
#include <stdlib.h> 
#include <unistd.h> 
#include <string>
#include <stdlib.h>
#include <sys/wait.h>
#include <vector>
#include <sstream>

using namespace std;


/********************* READ SNAKE AND LADDER INFORMATION********************************/

bool readfile(string filename,int snake_ladder[])
{
	string line;
	ifstream myfile (filename);
  	if (myfile.is_open()) // open file in read mode
  	{
    		 while(getline (myfile,line)) // read complete line one by one
    		{
			istringstream f(line); 
			int start,end,i=1;
			string info;
			while(f)
			{
				string st;
				f>>st;
				if(i==1) //read the 1st character form file to distinguish between shake and ladder
				info=st;
				else if(i==2) // read 2nd character as the start info 
				start=stoi(st);
				else if(i==3) //read 3rd character as the end info
				end=stoi(st);
				i++;
			}
		if(start<=0 || start>50 || end<=0 || end>50 || snake_ladder[start-1]!=0 || (info=="L" && start>=end) || (info=="S" && start<=end)) //board validation ladder start should be less than end and snake start should be greater than its end 
		return false;
		
		snake_ladder[start-1]=end;
    		}
    	myfile.close();

	return true;
  }

  else cout << "Unable to open file"; 

}

/************************* DRIVER FUNCTION ****************************/
int main(int argc, char *argv[])
{

	if(argc<4)
	{
		cout<<"Provide correct input";
		exit(1);
	}
	int players,squares; //number of players and number of squares in the board
	string filename; // name of file containing snake and ladder information
	squares=atoi(argv[1]);
	players=atoi(argv[2]);
	filename=argv[3];
	int pid,loc; // pid will contain the id of the process obtained during fork operation and loc contains the value read by parent child i.e number obtained when dice is rolled by any child
	vector<int> child_id(players); //child_id will store id of all the child processes created
	int pipes[2*players][2]; // pipes is a 2-D arrar where 2 pipes are connected with one child process
	int location[players]={0}; // location array will maintain the current postion of every player
	int parent_id=getpid(); //parent_id will store ID of parent process
	srand(time(0)+7);
	int turn=rand()%players; // generate random number to obtain the intial turn among the players
	int status=-1; // status will be 0 when one of the player reaches to a location with is greater than the number of squares, indicating end of the game
	int snake_ladder[squares+20]={0}; // array to maintain provided snake and ladder information

	bool check = readfile(filename,snake_ladder); // read snake and ladder info and upate snake_ladder array

	if(check==false) // if readfile function return false, this indicate that board is invalid
	{
		cout<<"Board information is incorrect";
		exit(1); // exit form parent child
	}	
	for(int i=0;i<2*players;i++) //establishing connection between the parent and its child process using pipes
	{
		pipe(pipes[i]);
		pipe(pipes[i+1]);
	}
	
	for(int i = 0; i < players; i++) 
	{
	    		pid = fork();	// create child process
	    		if(pid < 0) {
				printf("Error");
				exit(1);
	   		} 
			else if (pid == 0)  // if it is child process
			{
				if(getppid()!=parent_id) // exit from unwanted child process
				exit(1);
				else
				{
					while(1) // all child process keep on checking if there is content in the corresponding pipe
					{	
						int process_id;
						close(pipes[2*i][1]); // close write end parent to child connection
						close(pipes[2*i+1][0]); // close read end child to parent connection
						read(pipes[2*i][0],&process_id,sizeof(process_id)); // read from parent to child pipe
						if(process_id==-1) // it the content is -1 this indicate that status=0, end game and exit form child process
						{
							cout<<"Child process "<<i<<" ends"<<endl;
							exit(1);
						}

						cout<<"Child Read : "<<process_id<<endl; 

						if(getpid()==process_id) // if child process reads its ID from the pipe
						{
							srand(time(0)+i+7);
							int val = rand()%6 +1; // the child process generate a random number between 1-6 
							write(pipes[2*i+1][1],&val,sizeof(val)); // write the random number on its pipe
							cout<<"Child write : " <<val<<endl;
						}
					}
				}			 
	   		}
			else{ // if its parent process
				child_id[i]=pid; // store its required child ID in the child_id array
	   		} 
	}

if(getpid()==parent_id) // if it is parent 
{
while(status==-1) // iterate till the game ends
{
	cout<<"Turn : "<<turn<<endl;
	close(pipes[2*turn][0]); // close read end of parent to the child who have to roll the dice
	close(pipes[2*turn+1][1]); // close write end of the child who have to roll the dice to parent process
	write(pipes[2*turn][1],&child_id[turn],sizeof(child_id[turn])); // write the ID of the child process in its pipe
	cout<<"Parent write : " <<child_id[turn]<<endl;
	read(pipes[2*turn+1][0],&loc,sizeof(loc)); // read the dice value 
	
	
	cout<<"Parent read : "<<loc<<endl;
	location[turn]+=loc; // update the location of the child process

		if(location[turn]>squares) // if location of that child is greater than number of squares
		{
			status=0; // set status=0 indication end of game and move out of loop
			int x=-1;
			for(int i=0;i<players;i++)
			write(pipes[2*i][1],&x,sizeof(x)); // send -1 in all parent to child pipes
			
			wait(NULL); // wait till the child ends
			cout<<"Game ends and the winner is : " <<turn+1<<endl;
			
			for(int i=0;i<2*players;i++) // close pipes when child process ends
			{
				close(pipes[i][0]);
				close(pipes[i][1]);
			}
			cout<<"Ending main process"<<endl;
			exit(1); // end parent process
		}

	while(snake_ladder[location[turn]-1]!=0) //if there is snake or ladder at the current location of the player 
	location[turn]=snake_ladder[location[turn]-1]; // update location accordingly

	for(int i=0;i<players;i++)
	cout<<"Player "<<i+1<<" at location :" <<location[i]<<endl; // show the current location of every player
	cout<<endl;
	
	if(loc!=6) // if a player rolled 6 then turn be the same otherwise turn will move in round robin
	{
		turn++;
		if(turn>=players)
		turn=0;
	}
}
}                                            	
}
