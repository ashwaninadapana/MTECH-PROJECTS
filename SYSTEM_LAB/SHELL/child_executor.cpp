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
#include "map"
#include <wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include  <dirent.h>
#include <asm-generic/errno-base.h>
#include <errno.h>

using namespace std;

void loadConfiguration();
void startShell();
void executeCommand();
void terminateShell();
void validateCommand();
bool checkCommandValidity();
void endCommand();
bool check_if_history();
void addToHistory();
void showHistory();
void showContentsOfFile();
bool isCatCommandValid();
void clearCommandCache();
bool isExitCommandValid();
void terminateShell();
bool checkFileExistence(string filename);
void getHelp();
void parseCommand();
bool changeDirectory();
bool createDirectory(char* directoryname);
bool mkdirCall();
bool calculateRedirectionIndicesIfPresent();
void redirectConsoleOutput();
bool isredirectionOperator(char* operatr);
void generateTop();

int  duplicate_stdout;
int file_desc;
string command = "";	
string commandsArray[] = {"cat: cat_help.txt" , "mkdir: mkdir_help.txt" , "exit: exit_help.txt", "top: top_help.txt",  "help: help_help.txt", "cd: cd_help.txt"};
vector<char*> commandArgs;
vector<string> validCommands; 
vector<string> history;
int history_index = 0;
map<string,string> help;
char* username;
char currentDir[1024];
char hostname[1024];
char currentPath[1024];
bool redirectionPresent = false;
int  final_redirection_index = -1;
int  initial_redirection_index = -1;

int main(int argc, char *argv[], char** env)
{
	for(int i = 0 ; argv[i] != NULL; i++)
	{
		commandArgs.push_back(argv[i]);
	}
	if(calculateRedirectionIndicesIfPresent())
	{
		executeCommand();
	}
}

bool calculateRedirectionIndicesIfPresent()
{
	int count = 0;
	for(int i = 0; i < commandArgs.size(); i++)
	{	
		if(isredirectionOperator(commandArgs[i]))
		{
			count++;
			redirectionPresent = true;
			initial_redirection_index = (initial_redirection_index < 0)? i: initial_redirection_index;
			if(i > final_redirection_index)
			{
				final_redirection_index = i;
			}
		}
		else
		{
			if(redirectionPresent)
			{
				if(count == 1)
				{
					FILE* file=fopen(commandArgs[i],"r");
				   	if (file == NULL)  
					{
						file=fopen(commandArgs[i],"w");
					}
					fclose(file);
					count = 0;
				}
				else
				{
					cout << "Invalid command " << endl;
					return false;
				}
			}
			
		}
	}
	return true;
}

void parseCommand()
{
	string token;
	string delimiter = " ";
	char* cstr=const_cast<char*>(command.c_str());
	char* current;
	current = strtok(cstr,delimiter.c_str());
	while(current!=NULL)
	{
		commandArgs.push_back(current);
		current=strtok(NULL,delimiter.c_str());
	}

}

bool createDirectory(char* directory_name)
{
	int length_directory_name = strlen(directory_name);
	int value = 0;
	if(directory_name[length_directory_name - 1] == '/')
	{	
		directory_name[length_directory_name - 1] = '\0';
	}
	for(int i = 0; i < length_directory_name; i++)
	{
		if(directory_name[i] == '/')
		{	
			directory_name[i] = '\0';
			DIR* dir = opendir(directory_name);
			if (dir)
			{
			    closedir(dir);
			}
			else if (ENOENT == errno)
			{
			    if(value != -1)
			   {
				value = mkdir(directory_name, 0700);
				if(value == -1)
				{
					cout << "failed to create directory " << commandArgs[i] << endl;
					break;
				}
			   }
			}
			else
			{
			    cout << "Unable to open the directory: " << commandArgs[i]<< endl;
			}
			directory_name[i] = '/';
		}
	}
	if(value != -1)
	{
		value = mkdir(directory_name, 0700);
		if(value == -1)
		{
			cout << "failed to create directory " << directory_name << endl;
			return false;
		}
		return true;
	}
	return true;
}

bool mkdirCall()
{
	for(int i = 1; i < commandArgs.size(); i++)
	{

		DIR* dir = opendir(commandArgs[i]);
		if (dir)
		{
		    cout << "mkdir: cannot create directory ‘"<<commandArgs[i] <<"': File exists" << endl;
		    closedir(dir);
		}
		else if (ENOENT == errno)
		{	
		    createDirectory(commandArgs[i]);
		}
		else
		{
		    cout << "Unable to open the directory: " << commandArgs[i]<< endl;
		}
	}
	return false;
	
}

bool isredirectionOperator(char* operatr)
{
	if(strcmp(operatr,">") == 0 || strcmp(operatr,">>") == 0)
		{
			return true;
		}
	return false;
}

bool isCatCommandValid()
{
	int end_index = redirectionPresent?initial_redirection_index - 1: commandArgs.size()-1;
	string content = "";
	string temp =  "";
	if(commandArgs.size() == 1)//only cat is given as the command
	{
		string line;
		while(getline(cin, line))
		{
			cout << line << endl;
		}
		return true;
	}
	else if(commandArgs.size() > 1)//if there are more than 1 argument 
	{
		if(end_index == 0)//if > or >> is present just after cat
		{
			string line;
			while(getline(cin, line))
			{
				cout << line << endl;
			}
		}
		else//if filename is present after cat
		{
			for(int i = 1; i <= end_index ; i++)
			{
				if(checkFileExistence(commandArgs[i]))
					{
						 string str;
						 fstream f(commandArgs[i], fstream::in );
						 getline( f, str, '\0');
			 			 cout << str ;
						 if(i < end_index)
						 {
							cout << endl;
						 }
						 f.close();
					}
					else
					{
						dup2(duplicate_stdout,1);
						cout << "cat: " << commandArgs[i] << ": No such file or directory" << endl << endl;
						dup2(file_desc,1);
					}
			}
		}	
		return true;
	}
}


// Check if the file exists or not
bool checkFileExistence(string filename)
{
	ifstream file(filename.c_str());
	if (!file) 
	{
		return false;
	}
	file.close();
	return true;
}

bool changeDirectory()
{
    if(commandArgs.size() > 2)//greater than 2 arguments
	{
		cout << "Invalid number of Arguments" << endl;
	}
    else
	{
		char* path = '\0';
		if(commandArgs.size() == 1)
		{
			path = getenv("HOME");
		}
		else
		{
			path = commandArgs[1];
		}
		int value = chdir(path);
	    	if(value == 0)
		{
	    		printf(" Current path is %s\n", realpath(".", currentPath));
			return true;
		}
		else
		{
			cout << "No such file or Directory" << endl;
		}
	}
	return false;
}

void terminateShell()
{
	cout << "exiting the terminal "<< endl;
	exit(1);
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

void redirectConsoleOutput()
{
	char* fileName = commandArgs[final_redirection_index+1];
	char* operatr = commandArgs[final_redirection_index];
	FILE* myfile;
	if(strcmp(operatr,">") == 0) //overwrite
	{
		myfile = fopen(fileName,"w");
		file_desc = open(fileName, O_WRONLY|O_APPEND);	 
	}
	else if(strcmp(operatr, ">>")==0) //append to a file
	{	
		myfile = fopen(fileName,"a");
		file_desc = open(fileName,  O_APPEND | O_WRONLY);
	}
	if(myfile)
	{
		if(strcmp(commandArgs[0],"cat") == 0)
		{
			duplicate_stdout = dup(1);
   			dup2(file_desc, 1);
			isCatCommandValid();
			close(file_desc);//file_desc is no longer needed
		}
		else if(strcmp(commandArgs[0],"top") == 0)
		{
			dup2(file_desc,1);
			generateTop();
			close(file_desc);//file_desc is no longer needed
		}
		else
		{
			//tokens of the new command are available in char* new_command.
			//concatenate them so that we could execute it as a system command
			int buf_size = 1024;
			char buffer[buf_size];
			FILE* popenptr;
			string cmd = (string)commandArgs[0];
			for(int i = 1; i < initial_redirection_index ; i++)
			{
				cmd =  cmd + " " +(string)commandArgs[i];
			}
			//printf(" %s ", cmd.c_str());
			if((popenptr = popen(cmd.c_str(), "r")) != NULL)
			{
				while (fgets(buffer, buf_size, popenptr) != NULL)
                           		fprintf(myfile, "%s", buffer);
                   		pclose(popenptr);
			}
			else
			{
				 perror( "popen" );
			}
		}
	}
	fclose(myfile);
}

void generateTop()
{
	//system("ps -axelf");	//ps -axelf --sort=-pcpu,+pmem: show the top process which are using CPU the most
	DIR* dir;
	struct dirent *directory_entries;
	if((dir = opendir("/proc/")) != NULL)
	{
		while((directory_entries = readdir(dir)) != NULL)
		{
			char* folder_name = directory_entries->d_name;				
			char* string_part;
			long converted = strtol(folder_name, &string_part, 10);
			if(*string_part)
			{
			}
			else
			{
				string str;
				string path = "/proc/" + (string)folder_name + "/status";
				fstream f(path.c_str(), fstream::in );
				getline( f, str, '\0');
	 			cout << "-------------------------PROCESS ID: " <<converted <<"----------------------------"<< endl<< str << endl << endl;
				f.close();
			}	
		}
		closedir(dir);
	}	
	else
	{
		perror("");
	}
}

void executeCommand()
{
	if(redirectionPresent)// either > or >> is present. So redirect the output to the last file
	{
		redirectConsoleOutput();
	}
	else
	{
		if(strcmp(commandArgs[0], "cat") == 0)
		{
			isCatCommandValid();
		}
		else if(strcmp(commandArgs[0], "cd") == 0)
		{
			changeDirectory();
		}
		else if(strcmp(commandArgs[0], "mkdir") == 0)
		{
			mkdirCall();
		}
		else if(strcmp(commandArgs[0], "top") == 0)
		{
			generateTop();
		}
	}
}

void clearCommandCache()
{
	command = "";
	commandArgs.clear();
}

void getHelp()
{
	for(int i = 0; i < validCommands.size(); i++)
	{
		if(validCommands[i] == commandArgs[1])
		{
			string showHelpCommand = help[commandArgs[1]];
			clearCommandCache();
			command = "cat "+showHelpCommand;
			parseCommand();
			executeCommand();
			return;
		}
	}
		cout << "Command doesn't exist" << endl;
}
