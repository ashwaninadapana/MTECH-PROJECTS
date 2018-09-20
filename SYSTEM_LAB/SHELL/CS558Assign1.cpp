// CS558Assign1.cpp :Defines the entry point for the console application.
//
#include <unistd.h> 
#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include "vector"
#include "algorithm"
#include <stdlib.h>
#include "map"
#include <wait.h>
#include <fstream>
#include <readline/readline.h>
#include <readline/history.h>

using namespace std;

void loadConfiguration();
void startShell();
void executeCommand();
void terminateShell();
void validateCommand();
bool checkCommandValidity();
void clearCommandCache();
void validate();
bool isExitCommandValid();
void getHelp();
void parseCommand();
void execute();
bool isredirection_present();

const char* command;	
string commandsArray[] = {"cat: cat_help.txt" , "mkdir: mkdir_help.txt" , "exit: exit_help.txt", "top: top_help.txt",  "help: help_help.txt", "cd: cd_help.txt",">: overwrite.txt",">>: append.txt"};
vector<char*> commandArgs;
vector<string> validCommands; 
vector<string> history;
int history_index = 0;
map<string,string> help;
char* username;
char currentDir[1024];
char hostname[1024];
char currentPath[1024];
char message[1000];

extern char **environ;

int main(int argc, char** argv)
{
	loadConfiguration();
	startShell();
	terminateShell();
	return 0;
}

void startShell()
{
	while(1)
	{
		//read the command from shell
		try
		{	
		         char* buf;
			 buf = readline(message) ;
			 if (strlen(buf) > 0) 
			 {
				add_history(buf);
			 }
			 command = buf;
			 // readline malloc's a new buffer every time.
			 validate();
			 executeCommand();
			 free(buf);
		}
		catch(exception &e)
		{
			cout << "error occured. Cause: " << e.what() << endl;
		}
		clearCommandCache();
	}
}

void  execute()
{
     pid_t  pid = vfork();
     int    status;
     if (pid < 0)
     {     // fork a child process           
          cout << "ERROR: forking child process failed" << endl;
     }
     else if (pid == 0)
    {          // for the child process:  
	  char *args[]={"./exec"};
           
	  char ** arr = new char*[commandArgs.size()+1];
	  for(size_t i = 0; i < commandArgs.size(); i++)
	  {
		arr[i] = commandArgs[i];
	  }
	  arr[commandArgs.size()] = NULL;
          if ( execvp(args[0], arr)  < 0) 
	  {       
              cout << "*** ERROR: exec failed" << endl;
          }
	  else
	  {
	      cout << " completed child execution " << endl;
	  }
     }
     else
     {                                  // for the parent:      
          while (wait(&status) != pid)       // wait for completion  
               ;
     }
}

void loadConfiguration()
{
	//insert valid commands
	validCommands.clear();
	for(int i = 0; i < sizeof(commandsArray)/sizeof(commandsArray[0]) ; i++)
	{
		char* cstr=const_cast<char*>(commandsArray[i].c_str());
		char* current;
		current = strtok(cstr,":");
		while(current!=NULL)
		{
			validCommands.push_back(current);
			string helpStr=strtok(NULL,":");
			help.insert(pair<string, string>(current,helpStr));
			current = NULL;
		}
	}
	username = getenv("USER");
        gethostname(hostname, 1024);
	realpath(".", currentPath); 	
    	sprintf(message,"%s@%s:%s$  ", username,hostname, currentPath); 
}

void validate()
{
	switch(strlen(command))
	{
		case 0:
			   startShell();
			   break;
		default: validateCommand();
			 break;
	}
}

void parseCommand()
{
	string token;
	string delimiter = " ";
	char* cstr=const_cast<char*>(command);
	char* current;
	current = strtok(cstr, delimiter.c_str());
	while(current!=NULL)
	{
		commandArgs.push_back(current);
		current=strtok(NULL,delimiter.c_str());
	}

}

bool isredirection_present()
{
	for(int i = 0; i < commandArgs.size(); i++)
	{	
		if(strcmp(commandArgs[i],">") == 0 || strcmp(commandArgs[i],">>") == 0)
		{
			return true;
		}
	}
	return false;
}

void validateCommand()
{
	parseCommand();
	checkCommandValidity();
	if(commandArgs.size() > 11)
	{
		cout << "The number of arguments are greater than 10 "<< endl;
		return;
	}
}


void terminateShell()
{
	cout << "exiting the terminal "<< endl;
	exit(1);
}

bool checkCommandValidity()
{
	if (std::find(validCommands.begin(), validCommands.end(), commandArgs[0]) != validCommands.end() || isredirection_present())
	{
		return true;
	}	
	cout << "Unrecognised command" << endl;
	return false;
}

void clearCommandCache()
{
	command = '\0';
	commandArgs.clear();
}

bool isExitCommandValid()
{
	if(commandArgs.size() == 1)
	{
		terminateShell();
		return true;
	}
	return false;
}

void getHelp()
{
	string showHelpCommand = "";
	for(int i = 0; i < validCommands.size(); i++)
	{
		if(validCommands[i] == commandArgs[1])
		{
			showHelpCommand = help[commandArgs[1]];
			for(int i = 2; i < commandArgs.size(); i++)
				showHelpCommand += " " + (string)commandArgs[i]; 
			//cout << showHelpCommand << endl;
			clearCommandCache();
			showHelpCommand = "cat "+showHelpCommand;
			command = showHelpCommand.c_str();
			parseCommand();
			executeCommand();
			return;
		}
	}
		cout << "Command doesn't exist" << endl;
}

void executeCommand()
{
	if(strcmp(commandArgs[0], "exit") == 0)
	{
		isExitCommandValid();
	}
	else if(strcmp(commandArgs[0], "help") == 0)
	{
		getHelp();
	}
	else
	{
		execute();
	}
}

