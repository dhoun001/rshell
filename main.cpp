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
/*
//main run function, will prepare the command line
void run()
{
	//sentinel for while loop
	while (true)
	{
		//take in a command into a variable
		string command;
		cout << "$ ";
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

		}
	}
}
*/
//main function, will contain test cases

int main()
{
    int gethostname(char* name, size_t len);
    int sethostname(const char* name, size_t len); 

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
	return 0;
}
