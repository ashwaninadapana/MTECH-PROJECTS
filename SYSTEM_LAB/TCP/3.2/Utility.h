#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/timeb.h>  /* ftime, timeb (for timestamp in millisecond) */

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
	unsigned long long int startTime;
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

unsigned long long int getCurrentTimeInMicroSec()
{
	struct timeval timer_usec; 
	unsigned long long int timestamp_usec; /* timestamp in microsecond */
	if (!gettimeofday(&timer_usec, NULL)) 
	{
		timestamp_usec = ((long long int) timer_usec.tv_sec) * 1000000ll + (long long int) timer_usec.tv_usec;
	}
	else 
	{
		timestamp_usec = -1;
	}
	printf("%lld microseconds since epoch\n", timestamp_usec);

	return timestamp_usec;
}
