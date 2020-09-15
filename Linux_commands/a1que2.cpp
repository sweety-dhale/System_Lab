#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include <fstream> 
#include <iostream>
#include <sstream>
#include <vector> 
#include<dirent.h>
#include <sys/stat.h> 
#include<sys/types.h>
#include <bits/stdc++.h>
#include<readline/readline.h>
#include<readline/history.h>

#define RED     "\033[31m" 
#define RESET   "\033[0m"
#define GREEN   "\033[32m" 
#define BLUE    "\033[34m"
#define CYAN    "\033[36m" 
#define BOLDGREEN   "\033[1m\033[32m"                       // color codes


using namespace std; 

char cwd[100];
string final_cwd;                                          // global current working directory for command prompt
string ss = "$";
char *username;

int startShell()                                            // function to print welcome message
{
	username=getenv("USER");
	cout<<"\n \tWELCOME "<<username<<endl;
	cout<<"\n \tBELOW COMMANDS ARE IMPLEMENTED"<<endl;
	cout<<"     \t1. ls (-a)"<<endl;
	cout<<"     \t2. cd"<<endl;
	cout<<"     \t3. mkdir (-m)"<<endl;
	cout<<"     \t4. cp (-u)"<<endl;
	cout<<"     \t5. grep (-n)"<<endl;
	cout<<"     \t6. sort (-r)"<<endl;
	cout<<"     \t7. cat (-n)"<<endl;
	cout<<"     \t8. exit"<<endl;
	return 0;
} 

void createShellPrompt()                                  // function to create and print command prompt
{
	//system("clear");
	chdir("/path/to/change/directory/to");
	getcwd(cwd,sizeof(cwd));
}

void ls()                                                // function to implement 'ls' command
{
	struct dirent **fileName;
	int n=scandir(".",&fileName,NULL,alphasort);         // to fetch files and sub-directories in current directory

	while(n--)
	{
		if(fileName[n]->d_name[0]!='.')                  // print present files except "." and ".."
		cout<<fileName[n]->d_name<<"  ";
		free(fileName[n]);
	}
	free(fileName);                                     // free memory allocated to fileName
}

void ls_a()                                            // function to implement 'ls -a' command
{
	struct dirent **fileName;
	int n=scandir(".",&fileName,NULL,alphasort);       // to fetch files and sub-directories in current directory

	while(n--)
	{
		cout<<fileName[n]->d_name<<"  ";              // print all the files present  
		free(fileName[n]);
	}
	free(fileName);                                   // free memory allocated to fileName
}

void cd(vector<string> files,int fcnt)                // function to implement 'cd' command
{
	if(fcnt==1)
	{
		string homedir=getenv("HOME");                // home directory
		const char *f=homedir.c_str();
		chdir(f);
		getcwd(cwd,sizeof(cwd));
	}
	else if(fcnt==2)                                  // if path path is provided
	{
		const char *f=files[1].c_str();
		if(chdir(f)!=0)
		cout<<"bash: cd: \'"<< files[1]<<"\' : No such file or directory";
	}
	else                                             
	{
		cout<<"bash: cd: too many arguments";
	}
}

void cat(vector<string> files,int fcnt)                // function to print file contents
{
	ifstream fin;                                      // to read input file contents
	string line;
	int i=1;

	while(i<fcnt-1)                                    // to handle if more than one file provided
	{
		fin.open(files[i]);
		if(!fin)
		{
			cout<<"\ncat: "<<files[i]<<": No such file or directory"<<endl;   // if file doesn't exist
			fin.close();
			i++;
			continue;
		}
		while(getline(fin,line))
		{
			cout<<line<<endl;
		}
		fin.close();
		i++;
	}
}

void cat_n(vector<string> files,int fcnt)                 // function to print file contents with line numbers
{
	ifstream fin;
	string line;
	int i=2,ln=1;

	while(i<fcnt-1)
	{
		fin.open(files[i]);

		if(!fin)
		{
			cout<<"\n"<<"  cat: "<<files[i]<<": No such file or directory"<<endl;
			fin.close();
			i++;
			continue;
		}

		while(getline(fin,line))
		{
			cout<<ln<<"  "<<line<<endl;
			ln++;
		}
		fin.close();
		i++;
	}
}


void _mkdir(vector<string> files,int fcnt)                   // function to create directoty in present directory
{
	int i=1;

	while(i<fcnt-1)
	{
		const char *f=files[i].c_str();
		if(mkdir(f,0777)==-1)                               // if trouble occurs while creating directoty    
			cerr<<"mkdir: cannot create directory \'"<<files[i]<<"\':"<<strerror(errno)<<endl;  
		i++;
	}
}

void _mkdir_m(vector<string> files,int fcnt)               // function to create directoty in present directory with 
{                                                          // given mode
	int i=1;

	if(fcnt<4)                                             // when one of argument for 'mkdir -m' is not provided
	{
		cout<<"\nmkdir: missing operand";
		cout<<"\nTry \'mkdir --help\' for more information.";
	}
	else if(fcnt>=4)                                        // to create first directory with given mode
	{
		int mode=stoi(files[2]);
		i=3;
		const char *f=files[i].c_str();
		if(mkdir(f,mode)==-1)
			cerr<<"mkdir: cannot create directory \'"<<files[i]<<"\':"<<strerror(errno)<<endl;
		i++;
	}

	while(i<fcnt)                                           // to create first directory with default mode
	{
		const char *f=files[i].c_str();
		//cout<<"\n "<<f;
		if(mkdir(f,0777)==-1)
			cerr<<"mkdir: cannot create directory \'"<<files[i]<<"\':"<<strerror(errno)<<endl;
		i++;
	}
}

void cp(vector<string> files,int fcnt)                                        // function to copy source file to destination file
{
	if(fcnt==3)
		{
			string line;
			string source(files[1]);
			string destination(files[2]);
			ifstream myfile (source);
	  		if (myfile.is_open())
	  		{
				ofstream outfile(destination);
				while(getline(myfile,line))
				{	
					outfile<<line<<endl;	
				}
				outfile.close();
				myfile.close();
			}
			else
				cout<<"cp: cannot stat " <<source<<" : No such file or directory";	
		}
		
}

void cp_u(vector<string> files,int fcnt)                                        // function to copy source file to destination file
{
	if(fcnt==4)                                                                 // depending on modified time of source and destination file
	{
		string line;
		string source(files[2]);
		string destination(files[3]);
		ifstream myfile (source);
  		if (myfile.is_open())
  		{
			ifstream destfile(destination);
			if(!destfile.is_open())
			{
				ofstream outfile(destination);
				while(getline(myfile,line))
				{	
					outfile<<line<<endl;	
				}
				outfile.close();
			}
			else
			{
				struct stat s,d;
				stat(source.c_str(),&s);
				stat(destination.c_str(),&d);
				

				if(s.st_mtime > d.st_mtime)
				{
					ofstream outfile1(destination);
					while(getline(myfile,line))
					{	
						outfile1<<line<<endl;	
						//cout<<line<<endl;
					}
					outfile1.close();
				}
			}
			destfile.close();
			myfile.close();
		}
		else
			cout<<"cp: cannot stat " <<source<<" : No such file or directory";		
	}
}

bool cmpStrCaseIn(const string& start,const string& end)             // function to sort contents of filedata vector
{

   string::size_type len = std::min(start.length(),end.length());

   for(string::size_type i=0;i<len;++i)
   {
      if(tolower(start[i]) < tolower(end[i]))return true;
      if(tolower(start[i]) > tolower(end[i]))return false;
   }

   if(start.length()<end.length())return true;
   if(start.length()>end.length())return false;                      // can ignore

   return false;                                                     // equal should return false
}

void _sort(vector<string> files,int fcnt,int op)                    //  sort file contents         
{
		vector<string> filedata;
		ifstream fin;
	 	string line;
	    int i;

	    if(op==1)
	    	i=2;
	    else
	    	i=1;

	   while(i<fcnt-1)
	   {
	      fin.open(files[i]);
	      if(!fin)
	      {
	         cout<<"\nsort: cannot read: "<<files[i]<<": No such file or directory"<<endl;
	         fin.close();
	         i++;
	         return;
	      }
	      while(getline(fin,line))
	      {
	         
	         filedata.push_back(line);
	      }
	      fin.close();
	      i++;
	   }

	   sort(filedata.begin(),filedata.end(),cmpStrCaseIn);

	   if(op==1)
	   	for(auto it=filedata.rbegin();it!=filedata.rend();it++)              // print sorted output in reverse order
	   		cout <<*it<<endl;
	   else
	   for(const auto& s : filedata)                                         // print sorted output
	   		cout <<s<<endl;

}

void grep(vector<string> files,int fcnt)                                    // function to match 'pattern' from the given files
{
	if(fcnt>=3)
	{
		string line;
		string pattern=files[1];
		string filename;

		for(int i=2;i<fcnt;i++)                                           // if multiple files present
		{	
			filename = files[i];

			ifstream myfile (filename);
	  		if (myfile.is_open())
	  		{
				while(getline(myfile,line))
				{	 
					int pos=line.find(pattern);
					if(pos!=string::npos)
					{
						if(fcnt==3)						
							cout<<line.substr(0,pos)<<RED<<line.substr(pos,pattern.size())<<RESET<<line.substr(pos+pattern.size())<<endl;
						else
						{
							cout<<CYAN<< filename<<RESET<<BLUE<<" : "<<RESET<<line.substr(0,pos)<<RED<<line.substr(pos,pattern.size())<<RESET<<line.substr(pos+pattern.size())<<endl;
						}			
					}
				}
			}
			else
				cout<<"grep: "<< filename<<": No such file or directory"<<endl;
		}		
	}
}

void grep_n(vector<string> files,int fcnt)                                   // function to match 'pattern' from the given files
{                                                                            // provided line numbers from files
	if(fcnt>=3)
	{
		string line;
		string pattern=files[2];

		string filename;

		for(int i=3;i<fcnt;i++)                                            // if multiple files present
		{	
			filename = files[i];
			int count=0;
			ifstream myfile (filename);
	  		if (myfile.is_open())
	  		{
				while(getline(myfile,line))
				{	 
					count++;
					int pos=line.find(pattern);
					if(pos!=string::npos)
					{
						if(fcnt==4)		
							cout<<GREEN<<count<<RESET<<BLUE<<":"<<RESET<<line.substr(0,pos)<<RED<<line.substr(pos,pattern.size())<<RESET<<line.substr(pos+pattern.size())<<endl;	
						else
						{
	                    	cout<<CYAN<< filename<<RESET<<BLUE<<":"<<RESET<<GREEN<<count<<RESET<<BLUE<<":"<<RESET<<line.substr(0,pos)<<RED<<line.substr(pos,pattern.size())<<RESET<<line.substr(pos+pattern.size())<<endl;
						}
									
					}
				}
			}
			else
				cout<<"grep: "<< filename<<": No such file or directory"<<endl;
		}		
	}
}

int main()                                                           // function to run driver code
{
	startShell();                                                    
	string user(username);
	char *buffer;

	while(1)                                                        // to take commands from user
	{
		cout<<endl;
		createShellPrompt();                                        
		final_cwd=cwd+ss;
		vector<string> args;
		int acnt=0;
		char inp[100];
		buffer = readline((BOLDGREEN+user+":~"+RESET+BLUE+final_cwd+RESET).c_str());     // to print command prompt
		
        if (strlen(buffer) != 0)  
		{
       		add_history(buffer);                                 // to account the commands used
			strcpy(inp,buffer);

    	} 
		string inln(inp);
		istringstream line(inln);                                // to split the input line into strings

		while(line)
		{
			string str;
			line>>str;
			args.push_back(str);
			acnt++;
		}
		
		string cmd;
		cmd.assign(args[0]);                                   // to copy first string from input string into cmd

		if(cmd.compare("ls")==0)
		{
			string op;
			op.assign(args[1]);                               // to implement 'ls' command
			if(args[1][0]=='-')
			{
				if(args[1][1]=='a' && args[1].length()==2)
				{
					ls_a();
				}
				else
				{
					cout<<"\nls: invalid option -- "<<"\'"<<op<<"\'";
					cout<<"\nTry \'ls --help\' for more information.";
				}
			}
			else
			{
				ls();
			}
		}
		else if(cmd.compare("cd")==0)
		{
			cd(args,acnt-1);                                          // to implement 'cd' command
		}
		else if(cmd.compare("cat")==0)
		{
			string op;                                                // to implement 'cat' command
			op.assign(args[1]);
			if(args[1][0]=='-')
			{
				if(args[1][1]=='n' && args[1].length()==2)
				{
					cat_n(args,acnt);
				}
				else
				{
					cout<<"\ncat: invalid option -- "<<"\'"<<op<<"\'";
					cout<<"\nTry \'cat --help\' for more information.";
				}
			}
			else
			{
				cat(args,acnt);
			}
		}
		else if(cmd.compare("mkdir")==0)
		{
			string op;                                                       // to implement 'mkdir' command
			op.assign(args[1]);
			if(args[1][0]=='-')
			{
				if(args[1][1]=='m' && args[1].length()==2)
				{
					_mkdir_m(args,acnt-1);
				}
				else
				{
					cout<<"\nmkdir: invalid option -- "<<"\'"<<op<<"\'";
					cout<<"\nTry \'mkdir --help\' for more information.";
				}
			}
			else if(acnt>=2)
			{
				_mkdir(args,acnt);
			}
			else
			{
				cout<<"\nmkdir: missing operand";
				cout<<"\nTry \'mkdir --help\' for more information.";
			}
		}
		else if(cmd.compare("cp")==0)
		{
			string op;                                                         // to implement 'cp' command
			op.assign(args[1]);
			if(args[1][0]=='-')
			{
				if(args[1][1]=='u' && args[1].length()==2)
				{
					cp_u(args,acnt-1);
				}
				else
				{
					cout<<"\'cp: invalid option -- "<<"\'"<<op<<"\'";
					cout<<"\nTry \'cp --help\' for more information.";
				}
			}
			else
			{
				cp(args,acnt-1);
			}
		}
		else if(cmd.compare("sort")==0)
		{
			string op;                                                          // to implement 'sort' command
			op.assign(args[1]);
			if(args[1][0]=='-')
			{
				if(args[1][1]=='r' && args[1].length()==2)
				{
					_sort(args,acnt,1);
				}
				else
				{
					cout<<"\nsort: invalid option -- "<<"\'"<<op<<"\'";
					cout<<"\nTry \'sort --help\' for more information.";
				}
			}
			else
			{
				_sort(args,acnt,0);
			}
		}
		else if(cmd.compare("grep")==0)
		{
			string op;                                                            // to implement 'grep' command
			op.assign(args[1]);
			if(args[1][0]=='-')
			{
				if(args[1][1]=='n' && args[1].length()==2)
				{
					grep_n(args,acnt-1);
				}
				else
				{
					cout<<"\'grep: invalid option -- "<<"\'"<<op<<"\'";
					cout<<"\nTry \'grep --help\' for more information.";
				}
			}
			else
			{
				grep(args,acnt-1);
			}
		}
		else if(cmd.compare("exit")==0)                                       // to exit from running program
		{
			exit(0);
		}
		else 
			cout<<"\n"<<cmd<<": command not found";                           // if invalid command typed

	}

	return 0;
}
