#include <iostream>
#include <boost/tokenizer.hpp>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <vector>

using namespace std;
using namespace boost;

//function used to seperate commands
void parseconnect(vector< vector<string> > &cmd, vector<string> &connect, string command)
{
	//different connectors
	string semicolon = ";";
	string needfirst = "&&";
	string needfirstfail = "||";
	int i = 0;
	
	//finding connectors
	while (command.size() != 0)
	{
		int semi = 0;//command.find(semicolon);
		int needf = 0;//command.find(needfirst);
		int needff = 0;//command.find(needfirstfail);
		
		//test for cases where connectors are gone
		if (command.find(semicolon) != string::npos)
		{
			semi = command.find(semicolon);
		}
		else
		{
			semi = 10000;
		}
		if (command.find(needfirst) != string::npos)
		{
			needf = command.find(needfirst);
		}
		else
		{
			needf = 10000;
		}
		if (command.find(needfirstfail) != string::npos)
		{
			needff = command.find(needfirstfail);
		}
		else
		{
			needff = 10000;
		}

		vector<string> temp;
		if (semi > needf)
		{
			if (needf > needff)
			{
				connect.push_back(needfirstfail);
				temp.push_back(command.substr(0, needff));
				command = command.substr(needff + 2, command.size() - needff - 2);
				++i;
			}
			else
			{
				connect.push_back(needfirst);
				temp.push_back(command.substr(0, needf));
				command = command.substr(needf + 2, command.size() - needf - 2);
				++i;
			}
		}
		else if (needf > semi)
		{
			if (semi > needff)
			{
				connect.push_back(needfirstfail);
				temp.push_back(command.substr(0, needff));
				command = command.substr(needff + 2, command.size() - needff - 2);
				++i;
			}
			else
			{
				connect.push_back(semicolon);
				temp.push_back(command.substr(0, semi));
				command = command.substr(semi + 1, command.size() - semi - 1);
				++i;
			}
		}
		else if ( (needf == semi) && (needff != 10000) )
		{
			if (needf > needff)
			{
				connect.push_back(needfirstfail);
				temp.push_back(command.substr(0, needff));
				command = command.substr(needff + 2, command.size() - needff - 2);
				++i;
			}
		}
		else
		{
			//used when out of connectors 
			temp.push_back(command);
			cmd.push_back(temp);
			return;
		}
		cmd.push_back(temp);
	}
}

//tokenizer function
void token(vector< vector<string> > &cmdl)
{
	string f;
	for (int i = 0; i < cmdl.size(); ++i)
	{
		int j = 0;
		typedef tokenizer< char_separator<char> > tokenizer;
		char_separator<char> sep(" ");
		tokenizer tokens(cmdl.at(i).at(0), sep);
		cmdl.at(i).pop_back();
		for (tokenizer::iterator iter = tokens.begin(); iter != tokens.end(); ++iter)
		{
			f = *iter;
			cmdl.at(i).push_back(f);
			++j;
		}
	}
}


void startline()
{ 
    char hostname[128];
    int hostnameStatus = gethostname(hostname, sizeof(hostname)); //passes in an array named hostname & it basically makes a copy of the hostname stored  
                                                                  //somewhere else and passes it back by reference (default b/c its an array). 
    if(hostnameStatus == -1) 
    {
        perror(hostname);           
    }
    else
    {
         char* login = getlogin();       
         cout << login << "@" << hostname << " $ ";
    }   
}

//main run function, will prepare the command line
void run()
{
	//sentinel for while loop
	while (true)
	{
		//take in a command into a variable
		string command;
		startline(); 
		getline(cin, command);
		cout << endl;

		//exit command
		if (command == "exit")
		{
			exit(0);
		}
		else
		{
		    //call to parse
			vector< vector<string> > cmdline;
			vector<string> connectors;
			parseconnect(cmdline, connectors, command);
            token(cmdline); 
            vector< vector<char*> > commands; 

            //changes all strings to char pointers
            for(int i = 0; i < cmdline.size(); i++) 
           {
               commands.push_back(vector<char*>() ); 
               for(int j = 0; j < cmdline.size(); j++)
               {
                   commands.at(i).push_back(const_cast<char*>(cmdline.at(i).at(j).c_str()));
               } 
                char* temp = NULL; 
                commands.at(i).push_back(const_cast<char*>(temp)); 
           }
           //cmdline.size() add null at very end 

           int i = 0; 
           bool sentinel = true; 
           while(sentinel == true)
           {
               //checks for connector logic
               if(i >= connectors.size())
               {
                   sentinel = false; 
               }
               else if(connectors.at(i) == ";") 
               {
                   sentinel == true;
               }
               pid_t pid = fork();
               if(pid == 0) 
               {
                   if(execvp(commands.at(i).at(0), &(commands.at(i).at(0))) == -1)
                   {
                       if(connectors.at(i) == "&&") 
                       {
                           sentinel == false; 
                       }
                       else
                       {
                           sentinel == true; 
                       }
                       perror("exec"); 
                   }
                   ++i; 
               }
               else if (pid > 0) 
               {
                   if (wait(0) == -1) 
                   {
                       perror("wait"); 
                   }
               }
            }           
		}
	}
}

//main function, will contain test cases 
int main()
{
	string command = "ls -a && echo asdfkasdfjasdf ; echo asdfjjenenc || aasdfaf";
	vector< vector<string> > cmdline;
	vector<string> connectors;
	parseconnect(cmdline, connectors, command);
	
	for (int i = 0; i < cmdline.size() - 1; ++i)
	{
		cout << cmdline.at(i).at(0);
		for (int j = i; j < i + 1; ++j)
		{
			cout << connectors.at(j);
		}
	}
	cout << cmdline.at(cmdline.size() - 1).at(0) << endl;
	token(cmdline);
	for (int i = 0; i < cmdline.size(); ++i)
	{
		for (int j = 0; j < cmdline.at(i).size(); ++j)
		{
			cout << "<" << cmdline.at(i).at(j) << ">" << endl;
		}
	}
	return 0;
}
