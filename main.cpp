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
	string comment = "#";

	//deleting all comments
	if (command.find(comment) != string::npos)
	{
		int com = command.find(comment);
		command = command.substr(0, com);
	}

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

		//checks to see which index of the different connectors are first
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
void token(vector< vector<string> > cmdl, vector< vector<string> > &cmdl2)
{
	string f;
	for (int i = 0; i < cmdl.size(); ++i)
	{
		int j = 0;
		char_separator<char> sep(" ");
		tokenizer< char_separator<char> > tok(cmdl.at(i).at(0), sep);
		
		//cout << cmdl.at(i).at(0) << endl;
		cmdl2.push_back(vector<string>());

		//must seperate vectors or the tokenizer will BREAK
		for (tokenizer< char_separator<char> >::iterator iter = tok.begin(); iter != tok.end(); ++iter)
		{
			//cout << "deref iters: " << *iter << endl; 
			f = *iter;
			cmdl2.at(i).push_back(f);
			++j;
		}
		
	}
}


void startline()
{ 
    char hostname[128];
	//passes in an array named hostname & it basically makes a copy of the hostname stored 
    int hostnameStatus = gethostname(hostname, sizeof(hostname));
    //somewhere else and passes it back by reference (default b/c its an array). 
    if (hostnameStatus == -1) 
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
			
			//for (int i = 0; i < cmdline.size(); ++i)
			//{
			//	cout << "cmd before tokenizing: " << cmdline.at(i).at(0) << endl;
			//}
			vector< vector<string> > cmdline2;
            token(cmdline, cmdline2); 
            vector< vector<char*> > commands;

			//changes all strings to char pointers
			for (int i = 0; i < cmdline2.size(); ++i)
			{
				//cout << cmdline.size() << endl;
				commands.push_back( vector<char*>() );
				for (int j = 0; j < cmdline2.at(i).size(); ++j)
				{
					//cout << endl;
					//cout << "before conversion to char*: " << cmdline2.at(i).at(j) << endl;
					//cout << "during conversion to char*: " << cmdline2.at(i).at(j).c_str() << endl;
					commands.at(i).push_back(const_cast<char*>(cmdline2.at(i).at(j).c_str()));
				}
				char* temp = NULL;
				commands.at(i).push_back(temp);
			}

			//calls process
			int i = 0;
			int j = 0;
         	bool sentinel = true; 
          	while (sentinel == true) 
           	{
				if (commands.size() == 1)
				{
					pid_t pid = fork();
					if (pid == 0)
					{
						if (execvp(commands.at(i).at(0), &(commands.at(i).at(0))) == -1)
						{
							perror("exec");
						}
					}
					if (pid > 0)
					{
						if (wait(0) == -1)
						{
							perror("wait");
						}
					}
					sentinel = false;
				}
				else
				{	
					if (j == connectors.size())
					{
						--j;
					}
					//checks for connector logic
					string temp = commands.at(i).at(0);
					string tempconnectors = connectors.at(j);
					if (temp.compare("exit") == 0)
					{
						exit(0);
					}
					//forking process to child
					pid_t pid = fork();
					if (pid == 0)
					{
						if (execvp(commands.at(i).at(0), &(commands.at(i).at(0))) == -1)
						{
							if (tempconnectors.compare("&&") == 0)
							{
								sentinel = false;
							}
							else
							{
								sentinel = true;
							}
							perror("exec");
						}
						else
						{
							if (tempconnectors.compare("||") == 0)
							{
								sentinel = false;
							}
						}
					}
					if (pid > 0)
					{
						if (wait(0) == -1)
						{
							perror("wait");
						}
					}
					if (i >= connectors.size())
					{
						sentinel = false;
					}
					++i;
					++j;
				}			
           	}       
		}
	}
}

//main function, will contain test cases 
int main()
{
/*
	string command = "ls -a ; echo asdfkasdfjasdf ; echo asdfjjenenc || aasdfaf";
	vector<string> connectors;
	vector< vector<string> > cmdline;
	vector< vector<string> > cmdline2;
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
	token(cmdline, cmdline2);
	for (int i = 0; i < cmdline2.size(); ++i)
	{
		for (int j = 0; j < cmdline2.at(i).size(); ++j)
		{
			cout << "<" << cmdline2.at(i).at(j) << ">" << endl;
		}
	}
*/
	run();
	return 0;
}
