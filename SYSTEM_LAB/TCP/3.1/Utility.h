#include <string.h>

using namespace std;

struct message
{
	unsigned int syn:1;
	unsigned int ack:1;
	unsigned int fin:1;
	int     sequence_no;
	int          windowSize;
	int	     MSS;
	int     acknowledgement_no;	
	char         data[3];
};

struct support //assists to save information for packets received and sending acknowledgements
{
	int sequence_no;
	int acknowledgement_no;
	bool isPacketReceived;
	float sleepTime;
};

int minimum(int x, int y)
{
	int min = x>y? y: x;
	return min;
}

int maximum(int x, int y)
{
	int min = x>y? x: y;
	return min;
}

bool is_digit(string str)
{
    return str.find_first_not_of("0123456789") == string::npos;
}

