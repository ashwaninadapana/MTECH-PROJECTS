#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <arpa/inet.h>
#include <vector>
#include <map>
#include "Utility.h"
#define MAX_INITIAL_SEQUENCE_NUMBER 5000
#define CMSS  3

using namespace std;

void handleRetransmissions();

char data_sent[3] = "AB";
int SSTHRESH = 16;
struct message msg, ack_msg, emptyMsg = {0,0,0,0,0,'\0'};
int sock, client_sock;
int server_portno, client_portno;
struct sockaddr_in server, client;
unsigned int sockAddr_in_length = sizeof(struct sockaddr_in);
int client_nextSeq_no = 0;
int server_nextSeq_no = 0;
int server_max_seg_size = 0;
int max_seg_size = 0;
int initial_window_seq_no = 0;

int sequence_no_to_be_retransmitted = -1;
bool CONNECTION_ESTABLISHED = false;
bool CONNECTION_RESET = true;
bool FAST_RETRANSMIT_MODE = false;
bool IS_SYN_ACK_RECEIVED = false;
bool IS_SYN_ACK_TIMEOUT = false;
bool ACK_TIMEOUT = false;
int windowSize = -1;
vector<struct message> send_buffer;//packets to be transmitted
long timeout_timer = 3;
int receivedAckNumber = 0;
vector<pthread_t> thread_ids;
map<int,pthread_t> seq_thread_ids;
pthread_t syn_ack_thread;
pthread_t syn_ack_timer_thread;
pthread_t receiveThread;
vector<struct support> supportBuffer;
//slow start parameters
int cwnd = 0;
int AWS = -1;
int no_packets_sent = 0;//in the cwnd
int ack_counter = 0;
int client_process_id = -1;
int dup_ack = 0; 

//CUBIC parameters
float beta = 0.3;
int Wmax = -1;
float cubicParameter = 0.6; //K


void createClientSocket()
{
	  //socket: create the client socket 
	  client_sock = socket(AF_INET, SOCK_DGRAM, 0);
	  
	  //setting the timeout for receive
          struct timeval timeout;      
	  timeout.tv_sec = 2;//3sec
	  timeout.tv_usec = 0;

	  if (setsockopt (client_sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
	  {
 	  	cout << "setsockopt failed" << endl;
	  }


	  if (client_sock < 0) 
	    	cout << "ERROR opening client socket";
	  bzero((char *) &client, sizeof(client));//clearing the client variable
	  client.sin_family = AF_INET;
	  client.sin_port = htons((unsigned short)client_portno);
	  
	      
    // Convert IPv4 and IPv6 addresses from text to binary form
	  if(inet_pton(AF_INET, "127.0.0.1", &client.sin_addr)<=0) 
	  {
	       cout << "Invalid address/ Address not supported " << endl;
	       exit(0);
	  }
	 
	  if (bind(client_sock, (struct sockaddr *) &client, sockAddr_in_length) < 0) 
	    	cout << "ERROR in binding";
  	
	  //populating the server sockaddr_in variable
	  bzero((char *) &server, sizeof(server));//clearing the client variable
	  server.sin_family = AF_INET;
	  server.sin_addr.s_addr = htonl(INADDR_ANY);//htonl(inet_addr("127.0.0.1"));
	  server.sin_port = htons((unsigned short)server_portno);
	  
}

int generateInitialSequenceNum()
{
	int random = (rand() % (MAX_INITIAL_SEQUENCE_NUMBER + 1 - 1)) + 1;
	client_nextSeq_no = 0;//random;
	cout << "Client Initial sequence no: " << client_nextSeq_no << endl;
	return random;
}

void printMsg(string message, struct message obj)
{
	printf("%s ---------------->seq No: %d Acknow no: %d\n", message.c_str(), obj.sequence_no, obj.acknowledgement_no);
}

int sendSegment(struct message obj)
{
	printMsg("Sent: ", obj);
	int size = sendto(client_sock, &obj, sizeof(obj), 0, (struct sockaddr *) &server, sockAddr_in_length);
	return size;
}

int receiveSegment()
{
	int size = recvfrom(client_sock, &msg, sizeof(msg), 0, (struct sockaddr *) &server, &sockAddr_in_length);
	printf("Received <----------------seq No: %d Acknow no: %d\n",  msg.sequence_no, msg.acknowledgement_no);
	return size;
}

int receiveAckSegment()
{
	//cout << "Waiting for acknowledgement" << endl;
	int size = recvfrom(client_sock, &ack_msg, sizeof(ack_msg), 0, (struct sockaddr *) &server, &sockAddr_in_length);
	if(size > 0)
	{
		receivedAckNumber = ack_msg.acknowledgement_no;
		server_nextSeq_no += strlen(ack_msg.data);
		printf("Received <----------------seq No: %d Acknow no: %d\n",  ack_msg.sequence_no, ack_msg.acknowledgement_no);
	}
	return size;
}

void* startTimerForHandShake(void* temp)
{
	syn_ack_timer_thread = pthread_self();
	cout << "---Start the timer for SYN ACK receival "<< syn_ack_timer_thread << endl;
	sleep(timeout_timer);
	if(IS_SYN_ACK_RECEIVED)
	{
		cout << "Killing the Thread waiting for SYN ACK RECEIVAL"  << endl;
		pthread_cancel(pthread_self());		
	}
	else
	{
		cout << "TIMEOUT!!!!!!!!!!!!!!!!!!!" << endl;
		pthread_cancel(syn_ack_thread);
		IS_SYN_ACK_TIMEOUT = true;
		pthread_cancel(pthread_self());	
	}
}

void* waitForSynACK(void * temp)
{
	syn_ack_thread = pthread_self();
	cout << "Waiting for SYN ACK: " <<syn_ack_thread<< endl;
	while(!IS_SYN_ACK_RECEIVED)
	{
		int msg_size = receiveSegment();
	
		if (msg_size < 0)
		{
		      	cout << "ERROR in receiving from server. Waiting for one more time" << endl;
		}
		else
		{
			if(msg.syn == 1 && msg.ack == 1 && msg.fin == 0)//Receive SYN = 1 and ack = 1 as response for 1st request
			{
				server_max_seg_size = msg.MSS;
				max_seg_size = minimum(server_max_seg_size, CMSS);
				AWS = msg.windowSize;
				SSTHRESH = AWS/2;
				cout << "Max seg size: "<< max_seg_size << " AWS: " << AWS << " SSthresh: " << SSTHRESH << endl;
				IS_SYN_ACK_RECEIVED = true;
				//Send final acknowledgement
				client_nextSeq_no = msg.acknowledgement_no;					
				server_nextSeq_no = msg.sequence_no+1;
				msg.sequence_no = client_nextSeq_no;
				msg.acknowledgement_no = server_nextSeq_no;
				msg.syn = 0;
				msg.ack = 1;
				msg.fin = 0;
				msg_size = sendSegment(msg);
				if (msg_size < 0)
				{
				      	cout << "ERROR in sending to server.";
				}
				else
				{
					cout << "The connection is established." << endl;
					CONNECTION_ESTABLISHED = true;
					cout << "Killing the timeout thread " << endl;
					pthread_cancel(syn_ack_timer_thread);	
				}	
			}
			else
			{
				cout << "Error!!! Connection failed." << endl;
			}
		}
	}
}

void initialiseFlags()
{
	IS_SYN_ACK_RECEIVED = false;
	CONNECTION_ESTABLISHED = false;
	IS_SYN_ACK_TIMEOUT = false;
	CONNECTION_RESET = false;
}

void way3handShake()
{
	initialiseFlags();
	msg = emptyMsg;
	msg.syn = 1;
	msg.sequence_no = client_nextSeq_no;
	msg.MSS = CMSS;
	
	//send 1st Handshake signal
	cout << "--------> Sent SYN packet" << endl;
	int msg_size = sendSegment(msg);
	if (msg_size < 0)
	{
	      	cout << "ERROR in sending to server.";
	}	
	pthread_create(&syn_ack_timer_thread, NULL, startTimerForHandShake, NULL);
	pthread_create(&syn_ack_thread, NULL, waitForSynACK, NULL);
	//wait until eithere SYn ack timeout donot occur or connection is not established
	while(!(IS_SYN_ACK_TIMEOUT || CONNECTION_ESTABLISHED));
	cout << "Came out of the while loop" << endl;
}

void terminateConnection()
{
	cout << " TRYING TO END CONNECTION " << endl;
	msg = emptyMsg;
	msg.fin = 1;
	msg.sequence_no = client_nextSeq_no++;
	msg.acknowledgement_no = server_nextSeq_no;
	int msg_size = sendSegment(msg);//send FIN
	if (msg_size < 0)
	{
	      	cout << "ERROR in sending to server.";
	}

	msg_size = receiveSegment();	
	if (msg_size < 0)
	{
	      	cout << "ERROR in receiving from server.";
	}
	else
	{
		if(msg.syn == 0 && msg.ack == 1 && msg.fin == 0)//Receive ACK
		{

			msg_size = receiveSegment();
			if (msg_size < 0)
			{
			      	cout << "ERROR in receiving from server.";
			}
			else
			{
				if(msg.syn == 0 && msg.ack == 1 && msg.fin == 1)//Receive FIN + ACK
				{
					//Send final acknowledgement
					server_nextSeq_no = msg.sequence_no+1;
					client_nextSeq_no = msg.acknowledgement_no;
					msg.sequence_no = client_nextSeq_no;
					msg.acknowledgement_no = server_nextSeq_no;
					msg.syn = 0;
					msg.ack = 1;
					msg.fin = 0;
					msg_size = sendSegment(msg);
					if (msg_size < 0)
					{
					      	cout << "ERROR in sending to server.";
					}
					else
					{
						cout << "The connection is terminated." << endl;
						CONNECTION_RESET = true;
					}	
				}	
				else
				{
				}
			}
		}
		else
		{
			cout << "Error!!! Connection failed." << endl;
		}

	}
}

void parseArguments(int argc, char** argv)
{
  	//check command line arguments
	if (argc != 3) 
	{
		cout << "Arguments are less than 2." << endl << " Usage: ./client.out <server_port_no> <client_port_no> " << endl;
		exit(1);
  	}
	if(is_digit(argv[1]) && is_digit(argv[2]))
	{
  		server_portno = atoi(argv[1]);
		client_portno = atoi(argv[2]);
	}
	else
	{
		cout << " The argument passed is not a number." << endl;
		exit(0);
	}
}

int getIndex(int difference)
{
	int index = ((difference/max_seg_size) % windowSize);
	return index;
}

//returns index to save the information regarding the received packet
int getSupportBufferIndex(int packetSeqNo)
{
	int diff_seq = packetSeqNo - initial_window_seq_no;
	cout << "Difference between " << packetSeqNo << " and " << initial_window_seq_no << " is " << diff_seq << endl;
	if(diff_seq < 0)
	{
		cout << "The packet with seq no " << packetSeqNo << " is already received" << endl;
		return -1;
	}
	else
	{
		int index = getIndex(diff_seq);
		if(index != -1)
		{
			cout << " index for seqno: " << packetSeqNo << " is " << index << endl;
			if(supportBuffer[index].sequence_no == packetSeqNo)
			{
				cout << "Matched" << endl;
				return index;
			}
		}
	}
	return -2;
}

bool isAckReceived(int sequence_no)
{
	int index = getSupportBufferIndex(sequence_no);
	cout << index << endl;
	if(index != -1)
	{
		if(supportBuffer[index].isPacketReceived)
		{
			cout << "Sequence no: " << sequence_no << " received ack number: "<<  supportBuffer[index].acknowledgement_no << " already received" << endl;
			return true;
		}
	}
	else if(index == -1)
	{
		return true;//packet already received
	}
return false;
}

void* startDataTimer(void* temp)
{
	struct support* dup_msg = (struct support*) temp;
	int seqNo = dup_msg->sequence_no;
	cout << "Creating timer for " << seqNo << " sleep: " << dup_msg->sleepTime << endl;
	sleep(dup_msg->sleepTime);
	cout << "Checking if ack is received for seq no: " << seqNo  << endl;	
	if(isAckReceived(seqNo))
	{
		cout << "Killing the thread with seq no: " << seqNo << endl;
		pthread_cancel(seq_thread_ids[seqNo]);
	}
	else
	{
		cout << "tImeout" << endl << flush;
		ACK_TIMEOUT = true;
		sequence_no_to_be_retransmitted = seqNo;
	}
}

bool markAckReceived(int ack_no)
{
	
	for(int i = 0; i < supportBuffer.size(); i++)
	{		
		//if previous packets are not acknowledged and current acknowledgement number is greater than earlier acknowledgements, mark them as received
		if(supportBuffer[i].acknowledgement_no <= ack_no && !supportBuffer[i].isPacketReceived && !ACK_TIMEOUT)
		{	
			printf("Marking the packet with seq no %d ack: %d for ack: %d\n",supportBuffer[i].sequence_no, supportBuffer[i].acknowledgement_no, ack_no);
			ack_counter++;
			supportBuffer[i].isPacketReceived = true;
			printf("Cancelling thread for seq no: %d tid: %lu \n", supportBuffer[i].sequence_no,seq_thread_ids[supportBuffer[i].sequence_no]);
			pthread_cancel(seq_thread_ids[supportBuffer[i].sequence_no]);	
			if(supportBuffer[i].acknowledgement_no == ack_no)
				return true;	
		}
		else if(ACK_TIMEOUT)
		{
			printf("\nfailed to mark ack as timeout occured for seq:no %d \n",supportBuffer[i].sequence_no);
		}
	}
	return false;
}

struct message createMessagePacket()
{
	struct message newMsg = emptyMsg;
	strcpy(newMsg.data,data_sent);
	newMsg.sequence_no = client_nextSeq_no;			
	newMsg.acknowledgement_no = server_nextSeq_no;

	struct support entry;
	entry.sequence_no = client_nextSeq_no;
	client_nextSeq_no += max_seg_size;
	entry.acknowledgement_no = client_nextSeq_no;
	entry.sleepTime = timeout_timer + no_packets_sent;
	entry.isPacketReceived = false;

	supportBuffer.push_back(entry);
	send_buffer.push_back(newMsg);
	return newMsg;
}

void removeThreadEntry(pthread_t id)
{
	for(int i = 0 ; i < thread_ids.size(); i++)
	{
		if(id == thread_ids[i])
		{	
			cout << "Erasing entry " << i << " from the thread ids. id: "<< id << " thread_id: " << thread_ids[i] << endl;
			thread_ids.erase(thread_ids.begin()+i);	
			break;
		}
	}	
}

//kill all the threads
void killTimerThreads()
{
	cout << "Killing all threads" << endl;
	for(int i = 0 ; i < thread_ids.size(); i++)
	{
		pthread_cancel(thread_ids[i]);
	}
	thread_ids.clear();
}

//continuously receive acknowledgements and mark them as received
void* receiveAck(void* temp)
{
	//struct message acknow;
	int prev_ack = -1;
	int current_ack = -1;
	ack_counter = 0;
	dup_ack = 0;
	while(1)
	{
		printf("Waiting for acknowledgment");
		int size = receiveAckSegment();		
		if( size > 0 )
		{
			int current_ack = ack_msg.acknowledgement_no;
			if(markAckReceived(current_ack))
			{
				cout << "Ack counter: " << ack_counter << endl;
			}
			if(prev_ack == current_ack)
			{
				dup_ack++;
			}
			else
			{
				dup_ack = 0;
			}
			if(dup_ack >= 3)
			{
				printf("FAST RETRANSMIT MODE\n");
				sequence_no_to_be_retransmitted = current_ack;
				FAST_RETRANSMIT_MODE = true;
			}
			prev_ack = current_ack;
		}			
	}		
}

void killTimerForRetransmitted(int seq_no)
{
	cout << " cancelling the timer whose seq no: " << seq_no << " is "<< seq_thread_ids[seq_no] << endl;
	pthread_cancel(seq_thread_ids[seq_no]);
}

void populateBuffer()
{
	cout << "Populating buffer" << endl;
	no_packets_sent = 0;
	for(int i = 0; i < windowSize; i++)
		{
			createMessagePacket();
			no_packets_sent++;
		}
	cout << "Done populating" << endl;
}

//return minimum value of 2 segments if cwnd falls below it
int getNewSSthresh()
{
	SSTHRESH = maximum( 2* max_seg_size, cwnd/2);
	printf("\n New SSTHRESH is %d \n", SSTHRESH);
	return SSTHRESH;
}

int getNextTobeSentPacket()
{
	for(int i = 0; i < supportBuffer.size(); i++)
	{		
		if(!supportBuffer[i].isPacketReceived)
		{	
			return supportBuffer[i].sequence_no;	
		}
	}
	return client_nextSeq_no;
}

//Assumption: 1RTT is time to get ack for all windowSize packets and until timeout occured
void initiateFlow()
{
	cwnd = 1 * max_seg_size;
	int TO_BE_CONTINUED = 1;
	do
	  {
		FAST_RETRANSMIT_MODE = false;
		ACK_TIMEOUT = false;
		ack_counter = 0;		
	        windowSize = cwnd/max_seg_size;
		printf("\n\n\nNEXT ITERATION \nCwnd: %d   window size: %d\n", cwnd, windowSize);
		initial_window_seq_no = client_nextSeq_no;
		send_buffer.clear();
		supportBuffer.clear();
		
		no_packets_sent = 0;
		populateBuffer();
		//create a thread to receive acknowledgements
		
		for(int i = 0; i < windowSize; i++)
		{
			int seq_no = send_buffer[i].sequence_no;
			sendSegment(send_buffer[i]);
			pthread_t tid;
			pthread_create(&tid, NULL, startDataTimer, &supportBuffer[i]);
			printf("\n creating thread for seq no: %d\n", seq_no); 
			thread_ids.push_back(tid);
			seq_thread_ids.insert(pair<int,pthread_t>(seq_no, tid));
			//increment after creating the thread, since 1st data will have the same timeout
		}
		//pthread_create(&receiveThread, NULL, receiveAck, NULL);
		no_packets_sent = 0;
		//sleep(1);//giving time to get threads created
		printf("\n creating receive thread\n");
		pthread_create(&receiveThread, NULL, receiveAck, NULL);
		
		printf("%d %d %d ",FAST_RETRANSMIT_MODE, ACK_TIMEOUT,  ack_counter);
		while(!(FAST_RETRANSMIT_MODE || ACK_TIMEOUT || ack_counter == windowSize))
		{
		//	cout << "waiting" << endl;
		}
		//printf("ashu came out\n");
		if(FAST_RETRANSMIT_MODE)
		{
			handleRetransmissions();
			SSTHRESH = getNewSSthresh();
			cwnd = SSTHRESH;
			if(ACK_TIMEOUT)
			{
				killTimerThreads();
				client_nextSeq_no = getNextTobeSentPacket();
				printf("---------------TIME OUT occured.!! %d \n", client_nextSeq_no);
				cwnd = 1*max_seg_size;
			}
			/*if(cwnd < SSTHRESH)//slowStart
			{				
				cwnd = 2 * max_seg_size;
			}
			else//congestion avoidance
			{
				cwnd = SSTHRESH;
			}*/
			//retransmit the packets and wait for their acknowledgements until the current next sequence
		}
		else if(ack_counter == windowSize)
		{
			cout << "equal mode" << endl;
			if(cwnd < SSTHRESH)
			{

				int add_size = ack_counter * max_seg_size;
				cwnd = minimum( cwnd+add_size, SSTHRESH);
			}	
			else
			{
				cwnd += max_seg_size;
			}
			cwnd = minimum(cwnd, AWS);
			client_nextSeq_no = receivedAckNumber;//latest received acknowledgment no
		}
		else
		{
			while(!ACK_TIMEOUT);//wait until timeout occurs	
			killTimerThreads();
			client_nextSeq_no = getNextTobeSentPacket();
			printf("---------------TIME OUT occured.!! %d \n", client_nextSeq_no);
			SSTHRESH = getNewSSthresh();
			cwnd = 1*max_seg_size;
		}
		killTimerThreads();
		pthread_cancel(receiveThread);//Kill any previous receive Thread if present
		printf("\n________________________Press 1 to continue_______________________\n" );
		scanf("%d",&TO_BE_CONTINUED);
	}while(TO_BE_CONTINUED == 1);
}

int getMessageFromBuffer(int seq_no)
{
	for(int i = 0; i < send_buffer.size(); i++)
	{
		if(send_buffer[i].sequence_no == seq_no)
		{
			return i;
		}
	}
	return -1;
}

void handleRetransmissions()
{	
	int seq_no = sequence_no_to_be_retransmitted;
	do 
	{
		killTimerForRetransmitted(seq_no);
		printf("Sending packet with seq no: %d", seq_no);
		int index  = getMessageFromBuffer(seq_no);
		if(index != -1)
		{
			struct message temp_msg = send_buffer[index];
			printf("Sending packet with seq no: %d", temp_msg.sequence_no);		
			sendSegment(temp_msg);
			pthread_t tid;
			pthread_create(&tid, NULL, startDataTimer, &send_buffer[index]);
			while(!(ACK_TIMEOUT || isAckReceived(temp_msg.sequence_no)));
			if(isAckReceived(temp_msg.sequence_no))
			{
				seq_no = ack_msg.acknowledgement_no;
			}
		}
	}while(seq_no < client_nextSeq_no && !ACK_TIMEOUT);//stop retransmitting if there is timeout
	killTimerThreads();
}

void handleSignalUSR1(int signum)
{
	killTimerThreads();
	exit(0);
}

void registerSignals()
{
	signal(SIGINT,handleSignalUSR1);
}

int main(int argc, char** argv)
{
	cout  << "The client's process id: " << getpid()<< endl;
	registerSignals();
	client_process_id = getpid();
	generateInitialSequenceNum();
	parseArguments(argc, argv);
	createClientSocket();
	while(CONNECTION_RESET)
	{
		while(!CONNECTION_ESTABLISHED)
		{
			way3handShake();			
		}
		initiateFlow();			
		//terminateConnection();
	}
}
