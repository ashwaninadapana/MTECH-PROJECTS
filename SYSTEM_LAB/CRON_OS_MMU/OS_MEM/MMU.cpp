#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <signal.h>
#include <unistd.h>
#include <fstream>
#include <math.h>

using namespace std;

struct pageTable
{
	bool valid;
	int frame;
	bool dirty;
	int requested;
};

struct pageTable* sharedMemory;

int pages = 0;  //total no of pages
int offset = 0;
int page_num_bits = 0;
const char* traceFile = "";
int os_pid = -1;
int page_hits = 0;
int page_faults = 0;
int mmu_pid = -1;
int shmid = -1;
bool isSleepEnabled = true;

void detachSharedMemory()
{
    shmdt(sharedMemory);
}

void attachSharedMemory()
{
	key_t key;
	
	cout << "creating shared memory " << endl;
	key = ftok("pageTableSimulation",65);// create an unique key
	int sharedMemorySize = pages* sizeof(struct pageTable);
	 
        shmid = shmget(key, sharedMemorySize, 0666|IPC_CREAT);
 	if (shmid < 0)
 	{
        	perror("shmget");
        	exit(1);
        }
        sharedMemory = (struct pageTable*) shmat(shmid,(void*)0,0);
	cout << "MMU is attached with Page Table" << endl;
	for(int i = 0; i < pages ; i++)
	{
		printf("valid: %d frame: %d dirty:%d requested: %d\n",sharedMemory[i].valid, sharedMemory[i].frame,sharedMemory[i].dirty, sharedMemory[i].requested);
	}
}


//trim the string		
string trim(string s)
{
   size_t p = s.find_first_not_of(" \t");
   s.erase(0, p);

   p = s.find_last_not_of(" \t");
   if (string::npos != p)
      s.erase(p+1);
   return s;
}

bool checkIfAddressValid(string binaryAddr)
{
	if(!(strlen(binaryAddr.c_str()) == page_num_bits + offset))
	{
		return false;
	}
	return true;
}

string convertHexToBinaryAddress(string virtualAddress)
{	
	if(strcmp((virtualAddress.substr(0,2)).c_str() , "0x") == 0 || strcmp((virtualAddress.substr(0,2)).c_str() , "0X") == 0)//remove 0x if present in the start
	{
		virtualAddress = virtualAddress.substr(2,strlen(virtualAddress.c_str()));
	}
	char* binary = (char*)malloc(4 * strlen(virtualAddress.c_str()) * sizeof(char));
	cout << "virtualAddress: " << virtualAddress << endl;
	//TODO: handle extra bits 
	for(int i = 0; i < strlen(virtualAddress.c_str()); i++)
	{
		switch(virtualAddress[i])
		{
			case '0': binary = strcat(binary, "0000"); break;
			case '1': binary = strcat(binary, "0001"); break;
			case '2': binary = strcat(binary, "0010"); break;
			case '3': binary = strcat(binary, "0011"); break;
			case '4': binary = strcat(binary, "0100"); break;
			case '5': binary = strcat(binary, "0101"); break;
			case '6': binary = strcat(binary, "0110"); break;
			case '7': binary = strcat(binary, "0111"); break;
			case '8': binary = strcat(binary, "1000"); break;
			case '9': binary = strcat(binary, "1001"); break;
			case 'a':
			case 'A': binary = strcat(binary, "1010"); break;
			case 'b':
			case 'B': binary = strcat(binary, "1011"); break;
			case 'c':			
			case 'C': binary = strcat(binary, "1100"); break;
			case 'd':			
			case 'D': binary = strcat(binary, "1101"); break;
			case 'e':
			case 'E': binary = strcat(binary, "1110"); break;
			case 'f':
			case 'F': binary = strcat(binary, "1111"); break;
		}
	}
	if(!checkIfAddressValid(binary))
	{
		cout << "INVALID MEMORY ADDRESS: " << virtualAddress << endl;
	}
	cout << "Binary number: " << binary << endl;
	return binary; 
}

int convertBinaryToDecimal(string pageNumber)
{
	int decimalNumber = 0;
	cout << "page number: " << pageNumber << endl;
	for(int i =  0 ; i <= strlen(pageNumber.c_str()) -1 ; i++)
	{
		decimalNumber = (2 * decimalNumber) + (int(pageNumber[i]) - 48) ;
		
	cout << "decimal number: " << decimalNumber << endl;
	}
	cout << "decimal number: " << decimalNumber << endl;
	return decimalNumber; 
}

int getPageNumber(string virtualAddress)
{
	string binaryAddress = convertHexToBinaryAddress(virtualAddress);
	string pageNumber = binaryAddress.substr(0,page_num_bits);
	cout << "Page number: " << pageNumber << endl;
	int pageNo = convertBinaryToDecimal(pageNumber);
	return pageNo;
}

void printStatistics()
{
	cout << endl << "***********************STATISTICS**************************" << endl;
	cout << "Page Hits: " << page_hits << endl;
	cout << "Page Faults: " << page_faults << endl;
	cout << endl << "------------------------PAGE TABLE-------------------------" << endl;
	cout << "valid\t\tFrameno\t\tdirty\t\trequested" << endl;	
	for(int i = 0; i < pages; i++)
	{
		cout << i <<": " << sharedMemory[i].valid << "\t" << sharedMemory[i].frame << "\t" << sharedMemory[i].dirty << "\t" << sharedMemory[i].requested << endl;
	}
}

void processMemTraceFile()
{
	ifstream inFile;
	inFile.open(traceFile);
	string memoryTrace;
        while(inFile.good()) // To get you all the lines.
        {
	        getline(inFile,memoryTrace); 
		cout << endl << endl << "_________________________" << memoryTrace << "_____________________" << endl;
	        trim(memoryTrace);
		if(strlen(memoryTrace.c_str()) > 0)
		{
			char* const_line = const_cast<char*>(memoryTrace.c_str());
			string virtualAddress = strtok(const_line, " ");
			string mode = strtok(NULL, " ");
			mode = trim(mode);
				
		     	int pageNumber = getPageNumber(virtualAddress);
		    	cout << "Requested for page " << pageNumber << " in mode " << mode<< endl;
		     	if(!sharedMemory[pageNumber].valid)
		     	{
		     		cout << "Not present in the page table. Page fault" << endl;
		     		sharedMemory[pageNumber].requested = mmu_pid;
		     		page_faults++;
		     		kill(os_pid, SIGUSR1);
		     		while(isSleepEnabled)
				{
					sleep(3);//sleep for 15 secs
				}
				isSleepEnabled = true;
			}
		     	else //if the page is valid, increment the page hit count
		     	{
				cout << "Incrementing the page hits" << endl;
		     		page_hits++;
				sharedMemory[pageNumber].requested = mmu_pid;				
				kill(os_pid, SIGUSR2);
		     		while(isSleepEnabled)
				{
					sleep(3);//sleep for 15 secs
				}
				isSleepEnabled = true;
			}
			//set the page to dirty is Write mode
			if( strcmp(mode.c_str(),"W") == 0 || strcmp(mode.c_str(),"w") == 0 )
			{
				sharedMemory[pageNumber].dirty = true;
			}
		}
		printStatistics();
        }
	cout << "Closing the file" << endl;
	inFile.close();
}
//checks if it is a number
bool is_digit(string str)
{
	return str.find_first_not_of("0123456789") == string::npos;
}

//exits if the page bits are invalid
void checkIfPageBitsValid()
{
	int no_pages = pow(2,page_num_bits);
	if(pages != no_pages)
	{
		cout << "Page bits are invalid" << endl;
		exit(1);
	}	
}

//checks the arguments
bool validateArgs(int argc, char** args)
{
	if(argc < 6)
	{
		cout << "SORRY!! invalid no of arguments " << endl;
	}
	else if(argc == 6)
	{
		if(is_digit(args[1]) && is_digit(args[3]) && is_digit(args[4]) && is_digit(args[5]))
		{
			
			pages = atoi(args[1]);  //total no of pages
			traceFile = args[2];		
			page_num_bits = atoi(args[3]);
			offset = atoi(args[4]);
			os_pid = atoi(args[5]);
			checkIfPageBitsValid();
			cout << "Pages: " << pages << ", trace: " << traceFile << ", page_num_bits: " << page_num_bits << ", offset: " << offset << ", OS_PID: " << os_pid << endl;
			return true;
		}
		else
		{
			cout << "Please enter the valid integer arguments" << endl;
		}
	}
	else
	{
		cout << " Please enter only 2 arguments.\n Usage: ./mmu.out <pages_count> <mem_trace_file> <page number bits> <offset> <os_pid> " << endl;
	}
	return false;
}

void sigcont_handler(int signum)
{
	cout << "Received SIGCONT signal" << endl;
	isSleepEnabled = false;
}

//registers signals
void registerSignals()
{
	signal(SIGCONT, sigcont_handler);
}

int main(int argc, char **argv)
{
    mmu_pid = getpid();
    registerSignals();
    cout << " MMU ID: " << mmu_pid << endl; 
    if(validateArgs(argc, argv))
    {
		attachSharedMemory();
		processMemTraceFile();
		detachSharedMemory();//destroy the shared memory and signal the OS process for the last time
		kill(os_pid, SIGUSR1);
    }
    return 1;
}
