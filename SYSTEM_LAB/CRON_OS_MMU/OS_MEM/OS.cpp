#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <algorithm>


using namespace std;

struct pageTable
{
	bool valid;
	int frame;
	bool dirty;
	int requested;
};
struct pageTable* sharedMemory;
void traverseLFUList();

int pages = 0;  //total no of pages
int frames = 0; //total no of frames
vector<int> freeFrames;
int disk_access = 0;
int disk_access_time = 2;//sleep for 2 sec if there is a disk access
vector<int> fifoList;
vector<int> lfuList;
int shmid, lfu_shmid;

int getFreeFrame()
{
	cout << "Checking for free frames" << endl;
	for(int i = 0; i < frames; i++)
	{
		if(freeFrames[i] == 1)
		{
			cout << "Free frame: " << i << endl; 
			return i;
		}
	}
	return -1;
}

int fifo_victim()
{
	int victim = fifoList[0];
	fifoList.erase(fifoList.begin());
	cout << "VICTIM FRAME: " << victim << endl;
	return victim;
}

int lfu_victim()
{
	int victimFrame = -1;
	int min = lfuList[0];
	vector<int> minFrameList;
	minFrameList.push_back(0);
	int frame = 0;
	for(int i = 1; i < frames; i++)
	{
		if(lfuList[i] < min)
		{
			min = lfuList[i];
			frame = i;
			minFrameList.clear();
			minFrameList.push_back(i);
		}
		else if(lfuList[i] == min)
		{	
			minFrameList.push_back(i);
		}
	}
	cout << "MIN FRAME LIST: ";
	for(int i = 0; i < minFrameList.size(); i++)
	{
		cout << minFrameList[i] <<" ";
	}
	cout << endl;
	//CHOOSE RANDOM ELEMENT
	//int random = rand() % minFrameList.size();
	//cout << "random victim:  " << minFrameList[random] << endl;
	//return minFrameList[random];
	for(int k = 0; k < fifoList.size(); k++)
	{
		cout << "FIFO element" << fifoList[k] << endl;
		if(std::find(minFrameList.begin(), minFrameList.end(), fifoList[k]) != minFrameList.end()) 
		{
			victimFrame = fifoList[k];
			fifoList.erase(fifoList.begin() + k);
			cout << "selected victim frame is " << victimFrame << endl;
		   	break;
		}
	}
	cout << "FIFO elements after removing the victimFrame: " << endl;
	for(int k = 0; k < fifoList.size(); k++)
	{
		cout << fifoList[k];	
	}
	cout << endl;
	return victimFrame;
}

int getVictimFrame()
{
	int victimFrame = fifo_victim();//TODO: choose the frame using replacement policies
	for(int j = 0; j < pages; j++)//make the entry in the page table as invalid for the entry whose frame is the current victim frame
	{
		if(sharedMemory[j].frame == victimFrame)
		{
			sharedMemory[j].valid = 0;
			sharedMemory[j].frame = -1;
			if(sharedMemory[j].dirty == 1)//if dirty
			{
				cout << "The victim frame: " << victimFrame <<" is dirty. So writing back to the disk" << endl;
				cout << " %%%%%%%%%%%%%%%%%%%%%% Writing back ...%%%%%%%%%%%%%%%%%%%%" << endl;
				sleep(disk_access_time);
				cout << " %%%%%%%%%%%%%%%%%%%%%% COMPLETED %%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
				disk_access++;	
				sharedMemory[j].dirty = 0;			
			}
			break;
		}
	}
	return victimFrame;
}


void destroySharedMemory(int signum)
{
	// destroy the shared memory
    	shmdt(sharedMemory); //detach from shared memory 
        shmctl(shmid,IPC_RMID,NULL);
	cout << " DISK ACCESSES: " << disk_access << endl;
	exit(1);
}

void destroySharedMemory()
{
    	shmdt(sharedMemory); //detach from shared memory 
	// destroy the shared memory
        shmctl(shmid,IPC_RMID,NULL);	
	cout << " DISK ACCESSES: " << disk_access << endl;
}


void sigusr1_handler(int signum)
{
	bool requestedPage = false;
	int mmu_pid = -1;
	for(int i = 0 ; i < pages; i++)//only 1 non zero request at a time. Hence once the request is processed, it breaks the loop
	{
		if(sharedMemory[i].requested != 0)
		{
			cout << endl << endl << "********************Servicing Page fault of Page: "<< i << "************************" << endl;
			disk_access++;	
			requestedPage = true;
			cout << "simulated page fault" << endl;	
			int frameno = getFreeFrame();
			if(frameno == -1)
			{
				int victimFrame = getVictimFrame();
				if(victimFrame < 0 || victimFrame >= frames)
				{
					cout << "Error in selecting the victim frame" << endl;
				}
				else
				{
					frameno = victimFrame;
				}
			}
			if(frameno >=0 && frameno < frames)
			{
				fifoList.push_back(frameno);
				lfuList[frameno] = 1;
				cout << "$$$$    Frame no    :" << frameno << "           $$$$$$$$" << endl; 
				traverseLFUList();
				mmu_pid = sharedMemory[i].requested;
				sharedMemory[i].requested = 0;
				sharedMemory[i].valid = 1;
				sharedMemory[i].dirty = 0;
				sharedMemory[i].frame = frameno;
				freeFrames[frameno] = 0;//make it as not free	
			}
			cout << "sending Continue signal to mmu id: " << mmu_pid << endl;
			kill(mmu_pid,SIGCONT);
			break;
		}
	}
	if(!requestedPage)
	{
		cout << "There are no page whose page fault has to be serviced" << endl;
		raise(SIGINT);
	}
}

void createLFUList()
{
    	for(int i = 0; i < frames; i++)
	{
		lfuList.push_back(0);
	}
}


void createSharedMemory()
{
	shmid = -1;
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
    	// shmat to attach to shared memory
    	sharedMemory = (struct pageTable*) shmat(shmid,(void*)0,0);
 	for(int i = 0; i < pages ; i++)
	{
		struct pageTable newEntry = {false,-1,false,0};
		sharedMemory[i] = newEntry;
	}
	cout << "PageTable initialised" << endl;	
}

bool is_digit(string str)
{
    return str.find_first_not_of("0123456789") == string::npos;
}

bool validateArgs(int argc, char** args)
{
	if(argc < 3)
	{
		cout << "SORRY!! invalid no of arguments " << endl;
	}
	else if(argc == 3)
	{
		if(is_digit(args[1]) && is_digit(args[2]))
		{
			pages = atoi(args[1]);
			frames = atoi(args[2]);
			if(pages > frames)			
			{			
				return true;
			}
			else
			{
				cout << "Please enter the first argument greater than second argument" << endl;
				return false;
			}
		}
		else
		{
			cout << "Please enter the valid integer arguments" << endl;
		}
	}
	else
	{
		cout << " Please enter only 2 arguments.\n Usage: ./os.out <pages_count> <frame_count> " << endl;
	}
	return false;
}

void traverseLFUList()
{
	cout << "LFU counter list: "; 
	for(int i = 0; i < frames; i++)
	{
		cout << lfuList[i] << ", ";
	}
	cout << endl;
}

void updateLFUList(int signum)
{
	int mmu_pid = 0;
	for(int i = 0; i < pages; i++)
	{
		if(sharedMemory[i].requested != 0)
		{
			cout << endl << endl << "********************Servicing Page HIT of Page: "<< i << "************************" << endl;
			mmu_pid = sharedMemory[i].requested; 
			sharedMemory[i].requested = 0;
			lfuList[sharedMemory[i].frame]++;
			break;
		}
	}
	traverseLFUList();
	cout << "sending Continue signal to mmu id: " << mmu_pid << endl;
	kill(mmu_pid,SIGCONT);
}


void registerSignals()
{
	signal(SIGUSR1, sigusr1_handler);
	signal(SIGINT, destroySharedMemory);
	signal(SIGUSR2, updateLFUList);
}

void initialiseFreeFrames()
{
	for(int i = 0; i < frames; i++)
	{
		freeFrames.push_back(1);// 1- if free
	}
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    registerSignals();
    if(validateArgs(argc, argv))
    {	
    	initialiseFreeFrames();
	cout << "pages: " << pages << " frames: " <<  frames << endl;
	cout << "Process id: " << getpid() << endl;
    	createSharedMemory();
	createLFUList();
	while(1);
	destroySharedMemory();
    }
    return 1;
}
