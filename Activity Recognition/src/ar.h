#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define MAXSTR 80
#define MAXALENGTH 10000000
#define MAXBUFFER 256
#define DOUBLELIMIT 15

//features that determine the sensor event are sensor, time of day, day of week, previous activity, and activity length
#define SENSOR 0
#define TIME 1
#define DOW 2
#define PREVIOUS 3
#define LENGTH 4

#define SENSORVALUE 3
#define LABEL 4

#define OFF 0
#define ON 1

#define MIN 0.000001
#define CROSS_VALIDATION_FOLDS 3         

#define TRUE 1
#define FALSE 0

#define NB 0
#define HMM 1

#define TRAIN 1
#define TEST 2
#define BOTH 3

#define WINDOW 10

typedef struct LargeNumber
{
   long double mantissa;
   long int exponent;
} LargeNumber;

char* temperatureSensorKeywrd;
char* lightSensorKeywrd;
int *tempsensorValues;
int *lightsensorValues;
char **activitynames;
char ***sensormap;
char modelfilename[MAXSTR];
int **aevents;
int **starts;
int **lengthactivities;               // The length of each activity occurrence
int **previousactivity;
//actual activity - i, freq stores the frequency of values predicted for this activity i
int **freq;      
//total occurances of each activity
int *afreq;                       
//stores the status of each activity
int *open;                  
//stores the count of sensor events that belong to each activity        
int *stotal;                 
// Number of possible values for each feature -- check
int *svalues;                     
//stores the count of each subfeature values for the training data
int ***evidence;                   
//stores the count of each subfeature values for the testing data
int **testevidence;                    
//number of features that are considered for each sensor event
int *numfeaturevalues;            
//this section tells how to select each feature value
int *selectfeatures;                        
//length of each activity occurance
int **sizes;                                 
//stores the frequency of each sensor event for each activity
int **sfreq;                          
//tells to which parition that occurance belongs to, which helps in dividing the test and train data set
int *partition;                          
// Threshold values for feature value ranges
int **thresholds;                  
int numactivities;
int numfeatures;
int numphysicalsensors;
int numsensors;
int eval;   
bool check_fuzzy;                                     
//tells which type of partition is performed. either deterministic or random
int partitiontype;                                   
int outputlevel;
int right;
int wrong;
int model;
//tells the currently processed events
int evnum;                             
int mode;                                               
int stream;
double *prior;
//tr-transition probability matrix , emissonProb- emission probability matrix, prefix-l : in terms of large number
double **tr;                                      
double ***emissionProb;                              
LargeNumber min;
LargeNumber *likelihood;
LargeNumber **ltr;                           
LargeNumber *lprior;                              
LargeNumber ***lemissionProb;                    

void performActivityRecognition();
void TrainInit();
void TestInit();
void ReadOptions(int argc, char *argv[]);
void ReadHeader(FILE *fp);
void ReadData(FILE *fp);
void calculateState(int event[6], int size, int previous);
void calculateEvidence(int **e);
void Finish();
void ProcessData(int activity, int occurrence, int length,
                 char *dstr, char *tstr, char *sistr, char *svstr);
void NBCTrain(int cvnum);
void NBCTest(int cvnum);
void calculatePrior();
void SelectFeatures();
void Partition();
void makeAllLarge();
void printEvent(int *event);
void printLargeNumber(LargeNumber num);
void Summarize();
void printResults();
void readModel();
void saveModel();
FILE *Init(int argc, char *argv[]);
int FindActivity(char *name);
int AddActivity(char *date, char *time, char *sensorid, char *sensorvalue,
                int activity, int label, int same, int previous);
int MapSensors(char *sistr);
int cmp(int *t1, int *t2);
int comp(const void *t1, const void *t2);
int isEqual(LargeNumber op1, LargeNumber op2);
int isGreaterThan(LargeNumber op1, LargeNumber op2);
int DLength(int size);
double calculateProb(double p, int a);
LargeNumber makeLargeNumber(long double num);
LargeNumber standardize(LargeNumber num);
LargeNumber makeLargeNumber(long double num);
LargeNumber add(LargeNumber op1, LargeNumber op2);
LargeNumber subtract(LargeNumber op1, LargeNumber op2);
LargeNumber multiply(LargeNumber op1, LargeNumber op2);
LargeNumber divide(LargeNumber op1, LargeNumber op2);
void applyfuzzyrules(double, char* , double, double);
