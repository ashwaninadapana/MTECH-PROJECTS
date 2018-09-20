#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <pthread.h>
#include <fstream>
#include <unistd.h>
#include<bits/stdc++.h>
#include <map>
#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )
#define MAX_HOUR 24//exclude 24
#define MAX_MIN 60//exclude 60
#define MAX_MONTH 12//check month
#define MAX_DATE 32//exclude 32
#define MAX_DAYS_WEEK 7

using namespace std;

//Function declarations
void parseCronFile();
void* runCurrentCommandsCopy(void*);
string makeItbackgroundCommand(string command);
void fileChangeListener();
int getMonth(string month);
bool isNum(string str);
tm getCurrentTime();
bool checkHourValidity(int);
bool checkDayWeekValidity(int);
bool checkMinValidity(int);
bool checkMonthValidity(string month);
void runCurrentCommands();
void* scheduleChecker();
bool checkIfCurrentTimeMatches(struct conf conf_obj);
void killThreads();
void processCronFile();
long getDelayForTheStartOfVeryNextMin();
void terminate(int signum);

//global
struct conf
{
	string min;
	string hour;
	string date;
	string month;
	string day_week;
	string command;
};


string configurationFile = "/home/ashwani/Downloads/174101006/Que1/conf.txt";
bool isCommaPresent = false;
vector<struct conf> conf_list;
bool isExit = false;
int line_no = 0;
vector<pthread_t> thread_ids;
int fileWatchDescriptor, fileDescriptor;
pthread_t tid;

int main()
{
	//signal(SIGINT, terminate);
	tm currentTime = getCurrentTime();
	processCronFile();
	fileChangeListener();
}

void terminate(int signum)
{
	if(thread_ids.size() == 0)
	{
		inotify_rm_watch( fileDescriptor, fileWatchDescriptor );
		close( fileDescriptor );
		 
		exit(0);
	}
}

void processCronFile()
{
	killThreads();
	parseCronFile();
	pthread_create(&tid, NULL, runCurrentCommandsCopy,  NULL);
	//runCurrentCommands();	
}

//returns the integer value of the day of week
int getDayOfWeek(string dayofWeekstr)
{
	int dayOfWeek = -1;
	transform(dayofWeekstr.begin(), dayofWeekstr.end(), dayofWeekstr.begin(), ::tolower);
    	const char* day = dayofWeekstr.c_str();
	if(isNum(dayofWeekstr))
	{
		return atoi(day);
	}
	else
	{
		if(strcmp(day,"sun") == 0)
		{
			dayOfWeek = 0;
		}
		else if(strcmp(day,"mon") == 0)
		{
			dayOfWeek = 1;
		}
		else if(strcmp(day,"tue") == 0)
		{
			dayOfWeek = 2;
		}
		else if(strcmp(day,"wed") == 0)
		{
			dayOfWeek = 3;
		}
		else if(strcmp(day,"thu") == 0)
		{
			dayOfWeek = 4;
		}
		else if(strcmp(day,"fri") == 0)
		{
			dayOfWeek = 5;
		}
		else if(strcmp(day,"sat") == 0)
		{
			dayOfWeek = 6;
		}
	}
	return dayOfWeek;
}

//returns the integer value of the month
int getMonth(string monthStr)
{
	int month_no = -1;
	transform(monthStr.begin(), monthStr.end(), monthStr.begin(), ::tolower);
    	const char* month = monthStr.c_str();
	if(isNum(monthStr))
	{
		return atoi(month);
	}
	else
	{
		if(strcmp(month,"jan") == 0)
		{
			month_no = 1;
		}
		else if(strcmp(month,"feb") == 0)
		{
			month_no = 2;
		}
		else if(strcmp(month,"mar") == 0)
		{
			month_no = 3;
		}
		else if(strcmp(month,"apr") == 0)
		{
			month_no = 4;
		}
		else if(strcmp(month,"may") == 0)
		{
			month_no = 5;
		}
		else if(strcmp(month,"jun") == 0)
		{
			month_no = 6;
		}
		else if(strcmp(month,"jul") == 0)
		{
			month_no = 7;
		}
		else if(strcmp(month,"aug") == 0)
		{
			month_no = 8;
		}
		else if(strcmp(month,"sep") == 0)
		{
			month_no = 9;
		}
		else if(strcmp(month,"oct") == 0)
		{
			month_no = 10;
		}
		else if(strcmp(month,"nov") == 0)
		{
			month_no = 11;
		}
		else if(strcmp(month,"dec") == 0)
		{
			month_no = 12;
		}
	}
	return month_no;
}

//checks if the token is integer
bool isNum(string str)
{
	return str.find_first_not_of("0123456789") == string::npos;
}

//checks if the str is *
bool isWildCardCharacter(string str)
{
	return (strcmp(str.c_str(),"*") == 0);
}


bool isComment(string command)
{
	if(!command.empty() && command.find("#") == 0)//starts with #
	{
		return true;
	}
	return false;
}

//checks if / is present
bool isSlashPresent(string str)
{
	if(!str.empty())
	{
		if(str.find("/") != string::npos)
		{
			//cout << "Slash is present" << endl;
			return true;
		}
	}
	return false;
}

//checks if - is present
bool isHiphenPresent(string field)
{
	if(field.find("-") != string::npos && field.find("-") != 0)
	{
		return true;
	}
	return false;
}

//checks if val is in between min and max
bool checkRange(int val, int min, int max)
{
	if(val >= min && val < max)
	{
		return true;
	}
	return false;
}

int getActualValue(string token,int isMonth,int isDayOfWeek)
{		
	if(isMonth)
	{	
		return getMonth(token);
	}
	else if(isDayOfWeek)
	{	
		cout << "Day of week" << token << getDayOfWeek(token) << endl;
		return getDayOfWeek(token);				
	}	
	else if(isNum(token))
	{
		return atoi(token.c_str());
	}
	return -1;
}

//checks if the field is valid
bool checkAllPossibilitiesForToken(string token, int min, int max,int isMonth,int isDayOfWeek)
{
	if(isNum(token))
	{
		int number = atoi(token.c_str());
		return checkRange(number, min, max);
	}
	else if(isSlashPresent(token))
	{
		string lower = token.substr(0,token.find("/"));
		string upper = token.substr(token.find("/") + 1);
		//cout << "Lower: " << lower << " upper: " << upper << endl;
		bool isValid = false;
		int high = getActualValue(upper,isMonth,isDayOfWeek);
		if(isWildCardCharacter(lower))	
		{
		    return checkRange(high, min, max);
		}		
		else 
		{			
			int low = getActualValue(lower,isMonth,isDayOfWeek);
			isValid = checkRange(low, min, max)?checkRange(high, min, max): false;
		}
		return isValid;
	}
	else if(isHiphenPresent(token))
	{
		string lower = token.substr(0,token.find("-"));
		string upper = token.substr(token.find("-") + 1);
		int low = getActualValue(lower,isMonth,isDayOfWeek);
		int high = getActualValue(upper,isMonth,isDayOfWeek);
		bool isValid = checkRange(low, min, max)?checkRange(high, min, max): false;
		return isValid;
	}
	else if(isWildCardCharacter(token))
	{
		return true;
	}
	else if(getActualValue(token,isMonth,isDayOfWeek) != -1)
	{
		return true;
	}
}

//parses the token in each field separated by , and validates the token
bool validateToken(string token, int min, int max,int isMonth,int isDayOfWeek )
{
	if(!token.empty())
	{
		char* const_line = const_cast<char*>(token.c_str());
		const char* token = strtok(const_line,",");
		while(token != NULL)
		{
			//cout << "----->Validating token: " << token << endl;
			if(checkAllPossibilitiesForToken(token, min, max,isMonth,isDayOfWeek ))
			{
				token = strtok(NULL,",");
			}			
			else
				return false;
		}
		if(token == NULL)
		{
			return true;
		}
		return false;
	}
}

//returns whether the line is valid by validating each token
bool validateFields(char* min,char* hour,char* date_month,char* month,char* day_week)
{
	bool isValid = false;
	bool isMonth = false;
	bool isDayOfWeek = false;
	if(validateToken(min, 0, MAX_MIN, isMonth,isDayOfWeek ) && validateToken(hour, 0, MAX_HOUR,isMonth,isDayOfWeek) && validateToken(date_month, 1, MAX_DATE,isMonth,isDayOfWeek))
	{
			isMonth = true;
			if( validateToken(month, 1, MAX_MONTH, isMonth,isDayOfWeek))
			{
				isDayOfWeek = true;
				isMonth = false;
				isValid = validateToken(day_week, 0, MAX_DAYS_WEEK,isMonth,isDayOfWeek);
				isDayOfWeek = false;
			}
			
	}		
	return isValid;
}

//trim space at the ends
string trim(string str)
{
    str.erase(0, str.find_first_not_of(' '));//remove the first spaces
    str.erase(str.find_last_not_of(' ')+1);//remove the last spaces
    return str;
}

//parses each field in the line and if the line is valid, create conf object with the fields and push into the vector
void processTheLine(string line)
{
		   cout << "Command: " << line << endl;
		   line = trim(line);
		   line_no++;
		   if(!isComment(line))
		   {
			  /* get the first token */
			    char* const_line = const_cast<char*>(line.c_str());
			    cout << "------> Validate: " << const_line << endl;
			    char* min = strtok(const_line, " ");
			   
			    if( min != NULL ) 
			    {
				      char* hour = strtok(NULL, " "); 
				      char* date_month = strtok(NULL, " ");
				      char* month = strtok(NULL, " ");
				      char* day_week = strtok(NULL, " ");
				      char* command = strtok(NULL, "\n"); 
				      string cmd = makeItbackgroundCommand(command);
				      if(validateFields(min, hour, date_month, month, day_week))
				      {
						struct conf newObj = {string(min), string(hour), string(date_month), string(month), string(day_week) , cmd};
						conf_list.push_back(newObj);
						cout << "Added new object" << endl;
				      }
				      else
				      {
						cout << "The instruction at line: " << line_no << " is not valid" << endl;
				      }
				}
	            }
		    else
		    {
			   cout << "----Comment: The instruction at line " << line_no << " is a comment "<< endl << endl;
		    }		  
}

//parses the configuration file
void parseCronFile()
{
	cout << "Parsing the configuration file" << endl;
	//erase the conf list 
	conf_list.clear();
	ifstream infile(configurationFile.c_str());
	string line;
	line_no = 0;
	while(getline(infile, line))
	{
		if(!line.empty());
			processTheLine(line);     
	}
	infile.close();
}	

//get current time
struct tm getCurrentTime()
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	//printf("Current Time: %d-%d-%d %d:%d:%d %d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec , tm.tm_wday);
	return tm;
}



bool checkIfValuesMatches(string token, int current, int min, int max, int isMonth, int isDayOfWeek)
{
	//cout << "current Value: " << current << " token: " << token << endl; 
	if(isNum(token))
	{
		//cout << "HI" << endl;
		if(current == atoi(token.c_str()))
			return true;
		return false;
	}
	else if(isSlashPresent(token))
	{
		string lower = token.substr(0,token.find("/"));
		string upper = token.substr(token.find("/") + 1);
		//cout << "Lower: " << lower << " upper: " << upper << endl;
		bool isValid = false;
		int high = getActualValue(upper,isMonth,isDayOfWeek);
		int low = min;
		if(!isWildCardCharacter(lower))	
		{
			low = getActualValue(lower,isMonth,isDayOfWeek);
		}		
		for(int i = low; i < max && current >= i; i = i + high)
		{
			//cout << "The token: " << current << " trying to match with " << i << endl;
			if(i == current)
			{
				cout << "The token: " << current << " matches with " << i << endl;
				return true;
			}
		} 		
		return false;
	}
	else if(isHiphenPresent(token))
	{
		string lower = token.substr(0,token.find("-"));
		string upper = token.substr(token.find("-") + 1);
		int low = getActualValue(lower,isMonth,isDayOfWeek);
		int high = getActualValue(upper,isMonth,isDayOfWeek);
		if(current >= low && current <= high)
			return true;
		return false;
	}
	else if(isWildCardCharacter(token))
	{
		
		//cout << "Wild card character" << endl;
		return true;
	}
	else if(getActualValue(token,isMonth,isDayOfWeek) != -1)
	{
		return true;
	}
	else
	{
		return false;
	}
	return true;	
}

bool validate(string field, int current, int min, int max,int isMonth, int isDayOfWeek)
{
	//cout << "field: " << field << endl;
	if(!field.empty())
	{
		
		char* const_line = const_cast<char*>(field.c_str());
		//cout << "field: " << const_line << endl;
		const char* token = strtok(const_line,",");
		while(token != NULL)
		{
			//cout << "Token ashwani: " << token << endl;
			if(checkIfValuesMatches(token,current, min, max,isMonth,isDayOfWeek))
			{
				//cout << "Matched" << endl;
				return true;
			}			
			else
			{
				//cout << "Not Matched" << endl;
				token = strtok(NULL,",");
			}
		}
		return false;
	}
	return false;
}

//0: match, 1: current date exceeds, -1: current date is yet to come
int checkIfValuesexceeds(string token, int current, int min, int max, int isMonth, int isDayOfWeek)
{
	int val = -1;
	cout << "current: " << current << " token: " << token  << endl; 
	if(isNum(token))
	{
		if(current > atoi(token.c_str()))
			return 1;
		else if(current == atoi(token.c_str()))
			return 0;
		return -1;
	}
	else if(isSlashPresent(token))
	{
		string lower = token.substr(0,token.find("/"));
		string upper = token.substr(token.find("/") + 1);
		cout << "Lower: " << lower << " upper: " << upper << endl;
		int high = getActualValue(upper,isMonth,isDayOfWeek);
		int low = min;
		if(!isWildCardCharacter(lower))	
		{
			low = getActualValue(lower,isMonth,isDayOfWeek);
		}
		int i = 0;		
		for(i = low; i < max; i = i + high)
		{
			if(i == current)
			{
				cout << "Slash present: The token: " << current << " matches with " << i << endl;
				return 0;
			}
		} 		
		if(current > (i - high))
			return 1;
		else
			return -1;
	}
	else if(isHiphenPresent(token))
	{
		string lower = token.substr(0,token.find("-"));
		string upper = token.substr(token.find("-") + 1);
		int low = getActualValue(lower,isMonth,isDayOfWeek);
		int high = getActualValue(upper,isMonth,isDayOfWeek);
		cout << "Low: " << low << " high: " << high << endl;
		if(current >= low && current <= high)
			return 0;
		else if(current < low)
			return -1;
		else if(current > high)
			return 1;
	}
	else if(isWildCardCharacter(token))
	{
		return 0;
	}
	else if((val = getActualValue(token,isMonth,isDayOfWeek)) != -1)
	{
		if(current > val)
		{
			cout << "Current : " << current << " value: " << val << endl;
			return 1;
		}		
		else if(current == val)
			return 0;
		return -1;
	}
	return 1;//current exceeded	
}

int checkIfSchedulable(string field, int current, int min, int max, int isMonth, int isDayOfWeek)
{
	if(!field.empty())
	{
		char* const_line = const_cast<char*>(field.c_str());
		const char* token = strtok(const_line,",");
		while(token != NULL)
		{
			int val = checkIfValuesexceeds(token,current, min, max, isMonth,isDayOfWeek);
			if(val == 0 || val == -1)//can schedule if there is a match or yet to come in any one token
			{
				return val;
			}			
			else
				token = strtok(NULL,",");
		}
		return 1;
	}
}

bool isAnyExecutionLeft(struct conf confObj, struct tm current_time, int isMonth, int isDayOfWeek)
{
	isMonth = true;
	cout << "HI"<< endl;
	int isMonthSchedulable = checkIfSchedulable(confObj.month, current_time.tm_mon + 1, 1, MAX_MONTH, isMonth,isDayOfWeek);
	if(isMonthSchedulable == 1)// current month is greater than given
	{
		isMonth = false;
		cout << "Current month crossed given month" << endl;
		return false;
	}
	else if(isMonthSchedulable == 0)// current month 
	{
		if(isWildCardCharacter(confObj.day_week))
		{
			isMonth = false;
			cout << "Current month is the given month" << endl;		
			int isDateSchedulable = checkIfSchedulable(confObj.date, current_time.tm_mday, 1, MAX_DATE, isMonth,isDayOfWeek);
			if( isDateSchedulable == -1)
			{
				cout << "Given date is not over yet. So it is executable" << endl;
				return true;
			}
			else if(isDateSchedulable == 0)
			{
				cout << "Today" << endl;
				int isHourSchedulable = checkIfSchedulable(confObj.hour, current_time.tm_hour , 0, MAX_HOUR, isMonth,isDayOfWeek);
				if(isHourSchedulable == 1)
				{
					cout << "Current hour already passed given hour" << endl;
					return false;
				}
				else if(isHourSchedulable == 0)
				{
					cout << "Current hour" << endl;
					int isMInSchedulable = checkIfSchedulable(confObj.min, current_time.tm_min , 0, MAX_MIN, isMonth,isDayOfWeek);
					if(isMInSchedulable == 1)
					{
						cout << "Current min already passed given min" << endl;
						return false;
					}
					else if(isMInSchedulable == 0)
					{
						cout << "Current min. So it is executable" << endl;
						return true;
					}
					else
					{
						cout << " Yet to get the desired min. So it is executable" << endl;
						return true;
					}	
				}
				else
				{
					cout << " Yet to get the desired hour. So it is executable" << endl;
					return true;
				}
			}
			//TODO: day of the week check
			else
			{
				cout << "given date in this month is yet to come. So it is executable" << endl;
				return true;
			}
		}
		else//if day of week is not *
		{
			return true;
		}
	}
	else// if current month is less than given month
	{
		cout << "Current month yet to come" << endl;
		return true;
	}
}

bool checkIfCurrentTimeMatches(struct conf conf_obj)
{
	bool isSchedulable = true;
	bool isMonth = false;
	bool isDayOfWeek = false;
	struct tm current_time = getCurrentTime();
	if( validate(conf_obj.min, current_time.tm_min, 0, MAX_MIN, isMonth,isDayOfWeek ) &&  
		validate(conf_obj.hour, current_time.tm_hour, 0, MAX_HOUR, isMonth,isDayOfWeek ) && 
			((isMonth = true) && validate( conf_obj.month, current_time.tm_mon + 1, 1, MAX_MONTH, isMonth,isDayOfWeek)))
	{
			isMonth = false;
			if( validate(conf_obj.date, current_time.tm_mday, 1, MAX_DATE, isMonth,isDayOfWeek))
			{
				cout << "----->Scheduling the command: " << conf_obj.command << endl;
				system(conf_obj.command.c_str());
			}
			else
			{
				isDayOfWeek = true;
				if(validate(conf_obj.day_week, current_time.tm_wday, 0, MAX_DAYS_WEEK, isMonth,isDayOfWeek))
				{
					cout << "----->Scheduling the command: " << conf_obj.command << endl;
					system(conf_obj.command.c_str());
				}
			}
	}
	isDayOfWeek = false;
	isMonth = false;
	return isSchedulable;
}
/*
void* scheduleChecker(void* confObj)
{
	struct conf* confItem = (struct conf*) confObj;	
	cout << confItem->min << " " << confItem->hour << " " << confItem->date << endl;
	thread_ids.push_back(pthread_self());
        bool isSchedulable = true;//checkIfAnyRunLeft(*confItem);
	while(1)//!isExit || (isExit && isSchedulable))//If feature is not complete, then reemove the condition and keep it 1
	{
		cout << "Checking whether to schedule the command: " << confItem->command << endl;
		isSchedulable = checkIfCurrentTimeMatches(*confItem);
		//if(isSchedulable)
			sleep(60);
	}
	cout << "Thread is killed" << endl;
	if(thread_ids.size() == 0)	
		exit(0);
}*/

void scheduleCheckerCopy(struct conf confObj)
{
	bool isSchedulable = true;//checkIfAnyRunLeft(*confItem);
	while(1)
	{
		cout << "Checking whether to schedule the command: " << confObj.command << endl;
		checkIfCurrentTimeMatches(confObj);
		sleep(60);
	}
}
 
/*
void runCurrentCommands()
{
	long delay = getDelayForTheStartOfVeryNextMin();
	sleep(delay);
	for(int i = 0; i < conf_list.size(); i++)
	{
		scheduleCheckerCopy(conf_list[i]);
		//pthread_t tid;
		//pthread_create(&tid, NULL, scheduleChecker, &conf_list[i]);			
	}
}*/

void* runCurrentCommandsCopy(void*)
{
	long delay = getDelayForTheStartOfVeryNextMin();
	sleep(delay);
	while(1)
	{
		for(int i = 0; i < conf_list.size(); i++)
		{
			checkIfCurrentTimeMatches(conf_list[i]);		
		}
		sleep(60);
	}
}

//load the file when ever it is modified 
void fileChangeListener()
{
	cout << "Starting file Change listener" << endl;
	int length, i = 0;
	struct inotify_event* event = NULL;
        char buffer[BUF_LEN];
	
	fileDescriptor = inotify_init();
	if(fileDescriptor == -1)
	{
		perror("inotify_init");
	}
	else// created file descriptor successfully
	{
		fileWatchDescriptor = inotify_add_watch(fileDescriptor, configurationFile.c_str() , IN_MODIFY);
		cout << "Succesfully added watch descriptor for file " << configurationFile << endl;
		while (i < BUF_LEN) 
		{
		    length = read( fileDescriptor, buffer, BUF_LEN );  
		    cout << "Event occured." << endl;
	 	    if ( length < 0 ) 
		    {
			perror( "read" );
		    } 
		    struct inotify_event *event = ( struct inotify_event * ) &buffer[i];
		    printf( "The file %s was modified. So, restart processing....\n", event->name );
		    pthread_cancel(tid);
		    processCronFile();

		    i += EVENT_SIZE + event->len;
		    fileWatchDescriptor = inotify_add_watch(fileDescriptor, configurationFile.c_str() , IN_MODIFY);// add the watch again as gedit and some other editors remove the file over which we added watch and creates a new one. So add it once again.
		}
 	
		inotify_rm_watch( fileDescriptor, fileWatchDescriptor );
		close( fileDescriptor );
		 
		exit(0);
	}
}

void killThreads()
{
	for(int i = 0; i < thread_ids.size(); i++)
	{
		cout << " Cancelling the thread: " << thread_ids[i] << endl; 
		pthread_cancel(thread_ids[i]);		
	}
	thread_ids.clear();
}

long getDelayForTheStartOfVeryNextMin()
{
	time_t nxtTime;
	time_t currentTime = time(NULL);
	struct tm tm = *localtime(&currentTime);
	printf("Current Time: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	
	struct tm startTime;    
	startTime.tm_year = tm.tm_year;
	startTime.tm_mon = tm.tm_mon;
	startTime.tm_mday = tm.tm_mday;
	startTime.tm_hour = tm.tm_hour;
	startTime.tm_min = tm.tm_min;
	startTime.tm_sec = 0;
	startTime.tm_isdst = 0;
    	nxtTime = mktime(&startTime);
	printf("Previous Time: %d-%d-%d %d:%d:%d\n", startTime.tm_year + 1900, startTime.tm_mon + 1, startTime.tm_mday, startTime.tm_hour, startTime.tm_min, startTime.tm_sec);
	long diffTime = difftime(currentTime, nxtTime);
	long delay = diffTime > 0 ? 60 - diffTime: 0;
        cout << "Delay for next run time: " << delay << "sec" << endl;
	
        return delay;
}

string makeItbackgroundCommand(string command)
{
	//TODO : check if the command doesnt end with & and add & at the end
	cout << "Command end: " << command[strlen(command.c_str()) - 1]<< endl;
	if(command[strlen(command.c_str()) - 1] != '&')
	{
		command = command.append(" &");		
	}
	cout << "Command: " << command << endl;
	return command;
}

