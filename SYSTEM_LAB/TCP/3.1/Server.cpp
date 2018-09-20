#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <arpa/inet.h>
#include <vector>
#include "Utility.h"
#define MAX_INITIAL_SEQUENCE_NUMBER 5000
#define MAX_CAPACITY 32// Advertised WIndow Size in Bytes
#define SMSS 1

using namespace std;

void printMsg(string message);
void initiateFlow();
void initialisePropertiesForFlow();

struct message msg, emptyMsg = {0,0,0,0,0,'\0'};
int sock, client_sock;
int server_portno;
struct sockaddr_in server, client;
unsigned int sockAddr_in_length = sizeof(struct sockaddr_in);
bool CONNECTION_ESTABLISHED = false;
bool CONNECTION_RESET = true;
bool IS_PACKET_RECEIVED = false;
int client_initialSeq_no = 0;
int client_nextSeq_no = 0;
int server_nextSeq_no = 0;
int initial_window_seq_no = -1;
int max_seg_size = 0;
int client_max_seg_size = 0;
int windowSize = 0;
int blankSupportBufferIndex = -1;

vector<struct support> supportBuffer;
struct message receive_buffer;//acknowledge packets received. Size is 1

void createServerSocket()
{
   /* 
   * socket: create the server socket 
   */
	  sock = socket(AF_INET, SOCK_DGRAM, 0);
	  if (sock < 0) 
	    	cout << "ERROR opening socket" << endl;
	  bzero((char *) &server, sizeof(server));//clearing the server variable
	  server.sin_family = AF_INET;
	  server.sin_addr.s_addr = htonl(INADDR_ANY);//htonl(inet_addr("127.0.0.1"));
	  server.sin_port = htons((unsigned short)server_portno);
	  
	  if (bind(sock, (struct sockaddr *) &server, sockAddr_in_length) < 0) 
	    	cout << "ERROR in binding" << endl;
  	
	  client_sock = socket(AF_INET, SOCK_DGRAM, 0);
	  if (client_sock < 0) 
	    	cout << "ERROR opening client socket" << endl;
	  bzero((char *) &client, sizeof(client));//clearing the client variable
}

int generateInitialSequenceNum()
{
	int random = (rand() % (MAX_INITIAL_SEQUENCE_NUMBER + 1 - 1)) + 1;
	server_nextSeq_no = 0;//random;
	cout << "Server Initial sequence no: " << server_nextSeq_no << endl; 
	return random;
}

int sendSegment(struct message obj)
{	
	//cout << "Sent: " << endl << "\t\tSyn: "<<  obj.syn << " Sequence no: " << obj.sequence_no << " fin: " << obj.fin  << " ack: " << obj.ack << " Acknowledgement no: " <<obj.acknowledgement_no << " Length: " << strlen(obj.data) << endl;
	printf("Sent ---------------->seq No: %d Acknow no: %d\n", obj.sequence_no, obj.acknowledgement_no);
	return sendto(sock, &obj, sizeof(obj), 0, (struct sockaddr *) &client, sockAddr_in_length);
}

int receiveSegment()
{
	int size = recvfrom(sock, &msg, sizeof(msg), 0, (struct sockaddr *) &client, &sockAddr_in_length);
	printf("Received <----------------seq No: %d Acknow no: %d\n",  msg.sequence_no, msg.acknowledgement_no);
	return size;
}

void printMsg(string message)
{
	//cout << message << ":" << endl << "\t\tSyn: "<<  msg.syn << " Sequence no: " << msg.sequence_no << " fin: " << msg.fin  << " ack: " << msg.ack << " Acknowledgement no: " <<msg.acknowledgement_no << " Length: " << strlen(msg.data) << endl;
	printf("%s ---------------->seq No: %d Acknow no: %d\n", message.c_str(), msg.sequence_no, msg.acknowledgement_no);
}

void setMSS()
{
	client_max_seg_size = msg.MSS;
	max_seg_size = minimum(client_max_seg_size, SMSS);
	cout << "MSS: " << max_seg_size << endl;
	
}

void sendSynAck()
{
	//Send Syn + Ack 
	msg.ack = 1;
	msg.acknowledgement_no = client_nextSeq_no;
	msg.sequence_no = server_nextSeq_no++;
	msg.MSS = SMSS;
	msg.windowSize = MAX_CAPACITY;
	int msg_size = sendSegment(msg);
	if (msg_size < 0)
	{
		cout << "ERROR in sending SYN+ACK 2nd Handshake signal" << endl;
			return;	
	}
	else
	{
		CONNECTION_ESTABLISHED = true;
		printMsg("The message sent");
		msg_size = receiveSegment();
	   	if (msg_size < 0)
	   	{
	      		cout << "ERROR in recieving ACK 3rd HandShake signal." << endl;
	      		return;
		}
		printMsg("The message received");
		if(msg.fin == 0 && msg.syn == 0 && msg.ack == 1 && msg.sequence_no == client_nextSeq_no)//for the acknowledgement
		{
				cout << "Connection established!!. You can send more packets" << endl;
				CONNECTION_ESTABLISHED = true;
				return;
		}
		else if(msg.fin == 0 && msg.syn == 1 && msg.ack == 0 && msg.sequence_no == client_initialSeq_no)//if SYN packet is again sent, connection not established, send sys+ack again
		{
				setMSS();
				cout << "Seems previous Syn-ACK sent is lost" << endl;
				CONNECTION_ESTABLISHED = false;
				server_nextSeq_no--;
				sendSynAck();
				cout << "Connection failed" << endl;
				return;
		}
		else if(msg.fin == 0 && msg.syn == 0 && msg.ack == 0 && msg.sequence_no == client_nextSeq_no)//ACK lost, but the following packet is received.
		{
			IS_PACKET_RECEIVED = true;
			initialisePropertiesForFlow();
			initiateFlow();//already received next Packet
		}
	}
}

//TODO: add packet loss
void way3handShake()
{
	//receive initial SYN, HandShake step 1 received
	cout << "Waiting for HandShake!!" << endl;
        int msg_size = receiveSegment();
   	if (msg_size < 0)
   	{
      		cout << "ERROR in receiving 1st Handshake signal" << endl;	
      		return;
	}
	else
	{
		setMSS();
		printMsg("The message received");
		client_initialSeq_no = msg.sequence_no;
		client_nextSeq_no = client_initialSeq_no + 1;
		if(msg.syn == 1 && msg.ack == 0 && msg.fin == 0)
		{ 
			sendSynAck();
		}
	}
}


//TODO: add packet loss
void terminateConnection()
{	
	cout << " TRYING TO END CONNECTION " << endl;
	cout << "trying to receive the message" << endl;
	int msg_size = receiveSegment();
	cout << "message received" << endl;	
	if (msg_size < 0)
	{
	      	cout << "ERROR in receiving from server.";
		cout << "Failed to terminate the connection" << endl;
	}
	else
	{
		printMsg("The message received");	
		if(msg.syn == 0 && msg.ack == 0 && msg.fin == 1)//Receive ACK
		{
			//send ack 
			client_nextSeq_no = msg.sequence_no + 1;		
			msg.sequence_no = msg.acknowledgement_no;
			msg.acknowledgement_no = client_nextSeq_no;
			msg.ack = 1;
			msg.fin = 0;
	
			int msg_size = sendSegment(msg);//send FIN
			if (msg_size < 0)
			{
			      	cout << "ERROR in sending to server.";
				cout << "Failed to terminate the connection" << endl;
			}
			else
			{
				printMsg("The message sent");	
			
				sleep(10);//server closes 
				msg.fin = 1;
				int msg_size = sendSegment(msg);//send FIN
				if (msg_size < 0)
				{
				      	cout << "ERROR in sending to server.";
					cout << "Failed to terminate the connection" << endl;
				}	
				else
				{
					printMsg("The message sent");	
					msg_size = receiveSegment();
					if (msg_size < 0)
					{
					      	cout << "ERROR in receiving from server.";
					}
					else
					{
						if(msg.syn == 0 && msg.ack == 1 && msg.fin == 0)//Receive ACK
						{
								printMsg("The message received");
								cout << "The connection is terminated." << endl;
								CONNECTION_RESET = true;	
						}	
						else
						{
							cout << "Failed to terminate the connection" << endl;
						}
					}
				}
			}
		}
		else
		{
			cout << "Error!!! Connection failed." << endl;
		}

	}
}

//parse the arguments
void parseArguments(int argc, char** argv)
{
	if (argc != 2) 
	{
		cout << "Arguments are less than 2." << endl << " Usage: ./server <server_port_no>" << endl;     			
		exit(1);
  	}
	if(is_digit(argv[1]))
	{
  		server_portno = atoi(argv[1]);
	}
	else
	{
		cout << " The argument passed is not a number." << endl;
		exit(0);
	}
}

int getIndex(int difference)
{
	if(difference !=0)
	{
		int index = ((difference/max_seg_size) % windowSize);
		cout << index<< endl;
		return index;
	}
	else if(difference == 0)
	{
		return 0;
	}
	return -1;
}

//returns index to save the information regarding the received packet
int getSupportBufferIndex(uint32_t receivedPacketSeqNo)
{
	int diff_seq = receivedPacketSeqNo - initial_window_seq_no;
	if(diff_seq < 0)
	{
		cout << "The packet with seq no " << receivedPacketSeqNo << " is already received" << endl;
		return -1;
	}
	return getIndex(diff_seq);
}

//returns the sequence no of the first blank support buffer
int getSeqNoBlankSupportBuffer()
{
	for(int i = 0; i < windowSize; i++)
	{
		if(!supportBuffer[i].isPacketReceived)
		{
			blankSupportBufferIndex = i;
			return supportBuffer[i].sequence_no;
		}
	}
	//if the buffer is full, return windowSize and the client's expected next sequence no
	blankSupportBufferIndex = windowSize;
	return client_nextSeq_no;
}

struct support createNextNewBuffer()
{
	struct support entry;
	entry.sequence_no = client_nextSeq_no;
	client_nextSeq_no += max_seg_size;
	entry.acknowledgement_no = client_nextSeq_no;
	entry.isPacketReceived = false;
	//cout << "Support: seq: " << entry.sequence_no << " ack: " << entry.acknowledgement_no << endl; 
	return entry;
}

//fill the support buffer with new entries
void fillSupportBuffer()
{
	supportBuffer.clear(); //erase the supportBuffer
	//cout << "Client next seq no " << client_nextSeq_no << endl;
	initial_window_seq_no = client_nextSeq_no;
	//cout << "Initial window seq no " << initial_window_seq_no << endl;
	//populate support Buffer with windowSize entries
	for(int i = 0; i < windowSize; i++)
	{		
		supportBuffer.push_back(createNextNewBuffer());
	}
	//cout << "size of support buffer: " << supportBuffer.size();
}

void eraseInitialNonEmptySupportBuffer()
{
	//cout << "erasing the buffers upto blank buffer " <<blankSupportBufferIndex-1 << endl;
	if(blankSupportBufferIndex == windowSize)
	{
		fillSupportBuffer();	
	}
	else if(blankSupportBufferIndex >=0 && blankSupportBufferIndex < windowSize)
	{
		initial_window_seq_no = supportBuffer[blankSupportBufferIndex].sequence_no;
		for(int i = 0; i < blankSupportBufferIndex ; i++)
		{
			if(supportBuffer[0].isPacketReceived)
			{
				//cout << "erasing " << i << endl;
				supportBuffer.erase(supportBuffer.begin());
			}		
		}
		for(int i = 0; i < blankSupportBufferIndex ; i++)
		{
			//cout << "Pushing new support buffer" << endl;
			supportBuffer.push_back(createNextNewBuffer());		
		}	
	}
	blankSupportBufferIndex = 0;	
}
	
void initiateFlow()
{
	fillSupportBuffer();
	while(1)
	{
		int size = 1; 
		if(!IS_PACKET_RECEIVED)
		{
		      // cout << "Waiting for the data segment" << endl;
		       size = receiveSegment();
		       //cout << "block gone for the data segment" << endl;
		}
		IS_PACKET_RECEIVED = false;		
		if(size > 0)
		{
			//save received packet details in the buffer;			
			int index = getSupportBufferIndex(msg.sequence_no);
			cout << "Index for sequence no: " << msg.sequence_no << " is " << index << endl;
			if(index > -1)
			{
				cout << "Checking if the index is greater than -1 " << endl ;
				cout << supportBuffer[index].sequence_no << " " << msg.sequence_no << endl;
				if(supportBuffer[index].sequence_no == msg.sequence_no)
				{
					cout << "Marked received for sequence no: " << msg.sequence_no << " index: " << index << endl;
					supportBuffer[index].isPacketReceived = true;
				}	
			}
			struct message ackPacket = emptyMsg;
			ackPacket.sequence_no = server_nextSeq_no;
			ackPacket.acknowledgement_no = getSeqNoBlankSupportBuffer();
			ackPacket.ack = 1;
			sendSegment(ackPacket);
			if(blankSupportBufferIndex > 1 || blankSupportBufferIndex == windowSize)
			{
				eraseInitialNonEmptySupportBuffer();
			}	
					
		}
		else
		{
			cout << "Error in receiving" << endl;
		}
	}	

}
void initialisePropertiesForFlow()
{
	initial_window_seq_no = client_nextSeq_no;
	windowSize = MAX_CAPACITY/max_seg_size;//window size: each of max_seg_size
}

int main(int argc, char** argv)
{
	parseArguments(argc, argv);
	generateInitialSequenceNum();
	createServerSocket();
	while(CONNECTION_RESET)
	{
		while(!CONNECTION_ESTABLISHED)
		{
			way3handShake();		
			//client_nextSeq_no = ++server_nextSeq_no;//TODO:remove after doing handshake 
		}
		initialisePropertiesForFlow();
		initiateFlow();
		//terminateConnection();
	}
	return 1;
}
