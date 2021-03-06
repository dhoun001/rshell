#include <iostream>
#include <boost/tokenizer.hpp>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <vector>
#include <stack> 
#include <cstring>

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
	int size = cmdl.size();
	for (int i = 0; i < size; ++i)
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

//-----------------------------------------------------------------------------
void test_func(vector <char*> to_test)
{
	//cout << "detects [] or test" << endl;
	string e_delim = "-e";
	char* ed = const_cast<char*>(e_delim.c_str());
	string f_delim = "-f";
	char* fd = const_cast<char*>(f_delim.c_str());
	string d_delim = "-d";
	char* dd = const_cast<char*>(d_delim.c_str());
	if (strcmp(to_test.at(1), ed) == 0)
	{
		struct stat buffer;
		//cout << "there we go E" << endl;
		int statusstat = stat(to_test.at(2), &buffer);
		//cout << "Status of stat: " << statusstat << endl;
		if (statusstat == 0)
		{
			cout << "(true)" << endl;
			cout << "Path: Exists." << endl;
		}
		else
		{
			cout << "(false)" << endl;
			cout << "Path: Does not exist." << endl;
		}
	}
	else if (strcmp(to_test.at(1), fd) == 0)
	{
		//cout << "there we go F" << endl;
		struct stat buffer;
		stat(to_test.at(2), &buffer);
		//cout << "Status of stat: " << statusstat << endl;
		bool tempbool = S_ISREG(buffer.st_mode);
		if (tempbool)
		{
			cout << "(true)" << endl;
			cout << "Filename: Is regular file." << endl;
		}
		else
		{
			cout << "(false)" << endl;
			cout << "Filename: Is not regular file." << endl;
		}
	}
	else if (strcmp(to_test.at(1), dd) == 0)
	{
		//cout << "there we go D" << endl;
		struct stat buffer;
		stat(to_test.at(2), &buffer);
		//cout << "Status of stat: " << statusstat << endl;
		bool tempbool = S_ISDIR(buffer.st_mode);
		if (tempbool)
		{
			cout << "(true)" << endl;
			cout << "Filename: Is a directory." << endl;
		}
		else
		{
			cout << "(false)" << endl;
			cout << "Filename: Is not a directory." << endl;
		}
	}
	else if (to_test.size() == 2)
	{
		//cout << "auto set: E" << endl;
		struct stat buffer;
		int statusstat = stat(to_test.at(1), &buffer);
		//cout << "Status of stat: " << statusstat << endl;
		if (statusstat == 0)
		{
			cout << "(true)" << endl;
			cout << "Path: Exists." << endl;
		}
		else
		{
			cout << "(false)" << endl;
			cout << "Path: Does not exist." << endl;
		}
	}
	else if (to_test.size() == 1)
	{
		cout << "Error: invalid use of test" << endl;
	}
	else
	{
		//cout << "auto set: E" << endl;
		struct stat buffer;
		int statusstat = stat(to_test.at(2), &buffer);
		//cout << "Status of stat: " << statusstat << endl;
		if (statusstat == 0)
		{
			cout << "(true)" << endl;
			cout << "Path: Exists." << endl;
		}
		else
		{
			cout << "(false)" << endl;
			cout << "Path: Does not exist." << endl;
		}
	}



}
//-------------------------------------------------------------------------------

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
			//-------------------------------------------------------------------------
			//changes
			string test1 = "test";
			char* test2 = const_cast<char*>(test1.c_str());
			string test_openb = "[";
			string test_closedb = "]";
			char* test3 = const_cast<char*>(test_openb.c_str());
			char* test4 = const_cast<char*>(test_closedb.c_str());
			//-------------------------------------------------------------------------
	
			//changes all strings to char pointers
			int size = cmdline2.size();
			for (int i = 0; i < size; ++i)
			{
				//cout << cmdline.size() << endl;
				commands.push_back( vector<char*>() );
				for (unsigned int j = 0; j < cmdline2.at(i).size(); ++j)
				{
					//cout << endl;
					//cout << "before conversion to char*: " << cmdline2.at(i).at(j) << endl;
					//cout << "during conversion to char*: " << cmdline2.at(i).at(j).c_str() << endl;
					commands.at(i).push_back(const_cast<char*>(cmdline2.at(i).at(j).c_str()));
					/*--------------------------------------------------------------------------
					if (strcmp(commands.at(i).at(0), test2) == 0)
					{
						cout << "Hello there!" << " " << commands.at(i).at(0) << endl;
					}
					else if (strcmp(commands.at(i).at(0), test3) == 0)
					{
						cout << "Hello there!" << " " << commands.at(i).at(0) << endl;
					}
					*///-------------------------------------------------------------------------
				}
				char* temp = NULL;
				commands.at(i).push_back(temp);
			}

			//calls process
			unsigned int i = 0;
			unsigned int j = 0;
         	bool sentinel = true; 
          	while (sentinel == true) 
           	{
				if (strcmp(commands.at(i).at(0), test3) == 0)
				{
					char* tempcharp = commands.at(i).at(commands.at(i).size() - 1);
					if (strcmp(tempcharp, test4) == 0)
					{
						test_func(commands.at(i));
					}
					++i;
					++j;
					if (i == commands.size())
					{
						break;
					}
				}
				else if (strcmp(commands.at(i).at(0), test2) == 0)
				{
					test_func(commands.at(i));
					++i;
					++j;
					if (i == commands.size())
					{
						break;
					}
				}
				else
				{
					if (commands.size() == 1)
					{
						pid_t pid = fork();
						if (pid == 0)
						{
							//--------------------------------------------------------------------------
							int tempexec = execvp(commands.at(i).at(0), &(commands.at(i).at(0)));
							if (tempexec == -1)
							{	
								perror("exec");
							}
							//-----------------------
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
									++i;
									++j;
									if (i < commands.size())
									{
										continue;
									}
									//else
									//{
									//	sentinel = false;
									//}
								}
								perror("exec");
							}
						}
						if (pid > 0)
						{
							int stats;
							if (wait(&stats) == -1)
							{
								perror("wait");
							}
							else if (WEXITSTATUS(stats) == 0)
							{
								if ( (tempconnectors.compare("||") == 0) )
								{
									++i;
									++i;
									++j;
									++j;
									if (j < connectors.size())
									{
										tempconnectors = connectors.at(j);
										if (tempconnectors.compare("||") == 0)
										{
											++i;
											++j;
										}
									}
									if (i >= commands.size())
									{
										sentinel = false;
									}
									else
									{
										continue;
									}
								}
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
}

void parser(string command, vector<string> &cmd) 
{ 
    vector<int> parenthesis; 
    string firstParent = "(";
    string closeParent = ")";
    int firstIndex = -1; 
    int secondIndex = -1; 
    int numFirstParent = 0; 
    int numCloseParent = 0;  
    string subCommand = command;    //initialized subCommand to command.  
    int size = command.size(); 

    if(command.find(firstParent) != string::npos)
    {
       for(unsigned i = 0; i < command.size(); i++)
        {
            if(command.compare(i, 1, firstParent) == 0) 
            {
                numFirstParent++;
                parenthesis.push_back(i);    
            } 
            else if(command.compare(i, 1, closeParent) == 0) 
            {
                numCloseParent++;  
                parenthesis.push_back(i);  
            }
        }
        if(numFirstParent != numCloseParent)
        {
            cout << "Error: incorrect numbers of parethensis!" << endl; 
        }
        else
        {
            if(command.find(firstParent) != 0) 
             {
                secondIndex = command.find(firstParent);
                subCommand = command.substr(0, secondIndex); 
                if(secondIndex != 0)
                   {
                       cmd.push_back(subCommand);
                   }
             }
             for(unsigned i = 0; i < parenthesis.size()-1; i++)
             {
                firstIndex = parenthesis.at(i) + 1;
                secondIndex = parenthesis.at(i+1) - firstIndex; 
                subCommand = command.substr(firstIndex, secondIndex);
                if(secondIndex != 0)
                   {
                       cmd.push_back(subCommand);
                   }
             }
             if(parenthesis.at(parenthesis.size()-1) != size -1)
             {
                 firstIndex = parenthesis.at(parenthesis.size()-1)+ 1; 
                 secondIndex = command.size() - firstIndex; 
                 subCommand = command.substr(firstIndex, secondIndex);
                 if(secondIndex != 0) 
                 {
                     cmd.push_back(subCommand); 
                 }
             }
        }           
    }//end if no firstParent

}//end of parser function

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
