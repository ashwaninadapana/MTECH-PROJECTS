#include "ar.h"
#include "nb.h"
#include "hmm.h"
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <sys/times.h>


int main(int argc, char *argv[])
{
   FILE *fp;
   struct tms tmsstart, tmsend;
   clock_t clkStartTime, clkEndTime;
   static long clktck = 0;
   //tracking start time of the process
   clktck = sysconf(_SC_CLK_TCK);
   clkStartTime = times(&tmsstart);
   srand(clkStartTime);

   fp = Init(argc, argv);   
   ReadData(fp);              
   fclose(fp);
   
   //there is no need to determine ranges of features if it is just for testing
   if (mode != TEST)
   {
      SelectFeatures();                
   }

   if (outputlevel > 1)
   {
      //Summarize(); 
   }
   
   performActivityRecognition();

   if (mode != TRAIN)
   {
      printResults();
   }
   Finish();

   clkEndTime = times(&tmsend);
   printf("AR done (elapsed CPU time = %7.2f seconds).\n", (clkEndTime - clkStartTime) / (double) clktck);
   return(0);
}


// Initialize global parameters and other data structures.
FILE *Init(int argc, char *argv[])
{
   FILE *fp;
   int i, j;

   if (argc > 2)
   {
      fp = fopen(argv[2], "r");
      if (fp == NULL)
      {
         printf("Error reading parameter file %s\n", argv[2]);
	 exit(1);
      }
   }
   else
   {
      printf("Reading data from standard input\n");
      fp = stdin;
   }
   model = NB;
   strcpy(modelfilename, "model");
   numactivities = 1;
   numfeatures = 5;
   numphysicalsensors = 1;
   numsensors = 1;
   eval = 1;
   partitiontype = 1;
   outputlevel = 1;
   evnum = 0;
   stream = 0;
   //if default mode is not provided, it trains and tests the file
   mode = BOTH;
   check_fuzzy = false;
   //no of events predicted right and wrong
      
   ReadOptions(argc, argv);
   ReadHeader(fp);                                        
   fclose(fp);

   if (argc > 1)
   {
      fp = fopen(argv[1], "r");
      if (fp == NULL)
      {
         printf("Error reading file %s\n", argv[1]);
	 exit(1);
      }
   }
   else
   {
      printf("Reading data from standard input\n");
      fp = stdin;
   }

   // Initialize variables
   aevents = (int **) malloc(MAXALENGTH * sizeof(int *));
   evidence = (int ***) malloc(numactivities * sizeof(int **));
   emissionProb = (double ***) malloc(numactivities * sizeof(double **));
   lemissionProb =
      (LargeNumber ***) malloc(numactivities * sizeof(LargeNumber **));
   freq = (int **) malloc(numactivities * sizeof(int *));
   lengthactivities = (int **) malloc(numactivities * sizeof(int *));
   previousactivity = (int **) malloc(numactivities * sizeof(int *));
   starts = (int **) malloc(numactivities * sizeof(int *));
   tr = (double **) malloc(numactivities * sizeof(double *));
   testevidence = (int **) malloc(numfeatures * sizeof(int *));
   afreq = (int *) malloc(numactivities * sizeof(int));
   open = (int *) malloc(numactivities * sizeof(int));
   stotal = (int *) malloc(numactivities * sizeof(int));
   prior = (double *) malloc(numactivities * sizeof(double));
   svalues = (int *) malloc(numfeatures * sizeof(int));
   sfreq = (int **) malloc(numactivities * sizeof(int *));
   ltr = (LargeNumber **) malloc(numactivities * sizeof(LargeNumber *));
   likelihood = (LargeNumber *) malloc(numactivities * sizeof(LargeNumber));
   lprior = (LargeNumber *) malloc(numactivities * sizeof(LargeNumber));
   thresholds = (int **) malloc(numfeatures * sizeof(int *));

   for (i=0; i<numactivities; i++)
   {
      afreq[i] = 0;
      open[i] = 0;
      evidence[i] = (int **) malloc(numfeatures * sizeof(int *));
      emissionProb[i] = (double **) malloc(numfeatures * sizeof(double *));
      lemissionProb[i] =
         (LargeNumber **) malloc(numfeatures * sizeof(LargeNumber *));
      freq[i] = (int *) malloc(numactivities * sizeof(int));
      lengthactivities[i] = (int *) malloc(sizeof(int));
      *lengthactivities[i] = 0;
      previousactivity[i] = (int *) malloc(sizeof(int));
      starts[i] = (int *) malloc(sizeof(int));
      tr[i] = (double *) malloc(numactivities * sizeof(double));
      sfreq[i] = (int *) malloc(numsensors * sizeof(int));
      ltr[i] = (LargeNumber *) malloc(numactivities * sizeof(LargeNumber));
      for (j=0; j<numfeatures; j++)
      {
         evidence[i][j] = (int *) malloc(numfeaturevalues[j] * sizeof(int));
         emissionProb[i][j] =
	    (double *) malloc(numfeaturevalues[j] * sizeof(double));
         lemissionProb[i][j] =
	    (LargeNumber *) malloc(numfeaturevalues[j] * sizeof(LargeNumber));
      }

      //helps for storing the activities which is actually activity i but predicted to be j
      for (j=0; j<numactivities; j++)
      {
         freq[i][j] = 0;
      }      
     //frequency of sensors for this activity i;
      for (j=0; j<numsensors; j++)
      {     
	  sfreq[i][j] = 0;
      }
   }
   for (i=0; i<MAXALENGTH; i++)
   {
      aevents[i] = (int *) malloc(numfeatures * sizeof(int));
   }   
   for (i=0; i<numfeatures; i++)
   {
      testevidence[i] = (int *) malloc(numfeaturevalues[i] * sizeof(int));
      thresholds[i] = (int *) malloc((numfeaturevalues[i] - 1 ) * sizeof(int));
   }

      tempsensorValues = (int*) malloc(MAXALENGTH * sizeof(int));
      lightsensorValues = (int*) malloc(MAXALENGTH * sizeof(int));

   right = 0;
   wrong = 0;
   evnum = 0;

   // Because the probabilities get arbitrarily small we represent them in mantissa exponent format
   min = makeLargeNumber((long double) 1);
   min.mantissa = 1;
   min.exponent = -10000000;

   return(fp);
}


//read the options giving through the command line
void ReadOptions(int argc, char *argv[])
{
 
   printf("___________________________reading the options____________________________________________\n");

   /*
	./ar.out <data_file_name> <data_parameter_file_name> 
	The first 3 are mandatory. So start reading the parameters from arg 3
  */

   int i = 3;

   while (i < argc)
   {
      if (strcmp(argv[i], "-output") == 0)
      {
         i++;
	 sscanf(argv[i], "%d", &outputlevel);
	 if ((outputlevel < 1) || (outputlevel > 3))
	 {
	    printf("%s: output should be in between 1 and 3\n", argv[0]);
	    exit(1);
	 }
      }
      else if (strcmp(argv[i], "-partitiontype") == 0)
      {
         i++;
	 sscanf(argv[i], "%d", &partitiontype);
	 if ((partitiontype < 1) || (partitiontype > 2))
	 {
	    printf("%s: partitiontype should be either 1 or 2\n", argv[0]);
	    exit(1);
	 }
      }
      else if (strcmp(argv[i], "-modelfile") == 0)
      {
         i++;
	 sscanf(argv[i], "%s", modelfilename);
      }
      else if (strcmp(argv[i], "-mode") == 0)
      {
         i++;
	 sscanf(argv[i], "%d", &mode);
	 if ((mode < 1) || (mode > 3))
	 {
	    printf("%s: mode must be 1-3\n", argv[0]);
	    exit(1);
	 }
      }
      else
      {
         printf("%s: unknown option %s\n", argv[0], argv[i]);
	 exit(1);
      }
      i++;
   }

   return;
}


// Read the paramter file that are used while processing the data file
void ReadHeader(FILE *fp)
{
   char *cptr, buffer[MAXBUFFER], s1[80];
   int i, length;
   int readnumfeaturevalues = 0;

   if (fp == NULL)
   {
      printf("Sorry!! Parameter filename is not provided\n");
      exit(0);	
   }
   else                                
   {
      cptr = fgets(buffer, MAXBUFFER, fp);
      while (cptr != NULL)
      {
         length = strlen(cptr);
         if ((length > 0) && (cptr[0] != '%'))
         {
	    sscanf(cptr, "%s", s1);

	    //number of activities that are present in the data file
	    if (strcmp(s1, "numactivities") == 0)
	    {
               cptr = fgets(buffer, MAXBUFFER, fp);
	       sscanf(buffer, "%d", &numactivities);
            }
	    //number of features that are required for an activity
	    else if (strcmp(s1, "numfeatures") == 0)
	    {
               cptr = fgets(buffer, MAXBUFFER, fp);
	       sscanf(buffer, "%d", &numfeatures);
               numfeaturevalues = (int *) malloc(numfeatures * sizeof(int));
               selectfeatures = (int *) malloc(numfeatures * sizeof(int));
	       for (i=0; i<numfeatures; i++)
	          selectfeatures[i] = 0;
            }
	    else if(strcmp(s1,"temperaturesensors") == 0)
            {
		cptr = fgets(buffer, MAXBUFFER, fp);
	        temperatureSensorKeywrd = (char *) malloc(MAXSTR * sizeof(char));
		sscanf(buffer, "%s", temperatureSensorKeywrd);
		printf("temperaturesensors: %s \n",temperatureSensorKeywrd);
		check_fuzzy = true;
	    }
	    else if(strcmp(s1,"lightsensors") == 0)
            {
		cptr = fgets(buffer, MAXBUFFER, fp);
		lightSensorKeywrd = (char *) malloc(MAXSTR * sizeof(char));
		sscanf(buffer, "%s", lightSensorKeywrd);
		//printf("lightSensorKeywrd :%s\n",lightSensorKeywrd);
		check_fuzzy = true;
	    }
	    else if (strcmp(s1, "numfeaturevalues") == 0)
	    {
	       for (i=0; i<numfeatures; i++)
	       {
                  cptr = fgets(buffer, MAXBUFFER, fp);
	          sscanf(buffer, "%d", &numfeaturevalues[i]);
	       }
	       numsensors = numfeaturevalues[SENSOR];
               readnumfeaturevalues = 1;
	    }
	    // this section gives the names of activities
	    else if (strcmp(s1, "activitynames") == 0)
	    {
	       activitynames = (char **) malloc(numactivities * sizeof(char *));
	       for (i=0; i<numactivities; i++)
	       {
	          activitynames[i] = (char *) malloc(MAXSTR * sizeof(char));
                  cptr = fgets(buffer, MAXBUFFER, fp);
	          sscanf(buffer, "%s", activitynames[i]);
	       }
	    }
            //selectfeatures section tells which features are checked in the default ranges or equal frequency binning
            else if (strcmp(s1, "selectfeatures") == 0)
	    {
	       for (i=0; i<numfeatures; i++)
	       {
                  cptr = fgets(buffer, MAXBUFFER, fp);
	          sscanf(buffer, "%d", &selectfeatures[i]);
	       }
	    }
	    //Number of physical sensors that are used in the data file
            else if (strcmp(s1, "numphysicalsensors") == 0)
	    {
               cptr = fgets(buffer, MAXBUFFER, fp);
	       sscanf(buffer, "%d", &numphysicalsensors);
	       sensormap =
	          (char ***) malloc(numphysicalsensors * sizeof(char **));
	       for (i=0; i<numphysicalsensors; i++)
	       {
	          sensormap[i] = (char **) malloc(2 * sizeof(char *));
	          sensormap[i][0] = (char *) malloc(MAXSTR * sizeof(char));
	          sensormap[i][1] = (char *) malloc(MAXSTR * sizeof(char));
	       }
	    }
	    // map each physical sensor IDs to logical sensor IDs
	    else if (strcmp(s1, "mapsensors") == 0)
	    {
	       for (i=0; i<numphysicalsensors; i++)
	       {
                  cptr = fgets(buffer, MAXBUFFER, fp);
	          sscanf(buffer, "%s %s", sensormap[i][0], sensormap[i][1]);
	       }
            }
	    //model which is used for training
	    else if (strcmp(s1, "model") == 0)
	    {
               cptr = fgets(buffer, MAXBUFFER, fp);
	       sscanf(buffer, "%s", s1);
	       if (strcmp(s1, "naivebayes") == 0)
	       {
	          model = NB; 
	          printf("Using naive bayes classifier\n");
	       }
               else if (strcmp(s1, "hmm") == 0)
	       {
	          model = HMM;
		  printf("Using hidden Markov model\n");
	       }
	    }
         }
         cptr = fgets(buffer, MAXBUFFER, fp);                
      }
      // Use default feature values 
      if (readnumfeaturevalues == 0)               
      {
	 numsensors = numfeaturevalues[SENSOR] = numphysicalsensors;
	 numfeaturevalues[TIME] = 5;
	 numfeaturevalues[DOW] = 7;
	 numfeaturevalues[PREVIOUS] = numactivities;
	 numfeaturevalues[LENGTH] = 3;
      }
   }
}

/*read the data from the dataset and consider the activities which are relevant to any activity. Otherwise just ignore it
Lines starting with % should nt be considered
*/
void ReadData(FILE *fp)
{
   char *cptr, buffer[256], status[80];
   char date[80], time[80], sensorid[80], sensorvalue[80], activity_name[80];
   int i, is_independent_event, length, num, same, previous;

   same = 0;                                  
   is_independent_event = 0;
   previous = 0;
   cptr = fgets(buffer, 256, fp);


   while (cptr != NULL)
   {
      strcpy(activity_name, "nolabel");
      length = strlen(cptr);
      //printf("%s\n",cptr);
      if (length > 0 && (cptr[0] != '%'))
      {
         //remove ending spaces
         while ((length > 1) && ((cptr[length-2] == ' ') || (cptr[length-1] == '	')))
	 {
            length--;
         } 
         
	 //activity begin/end
         if (((cptr[length-4] == 'e') && (cptr[length-3] == 'n') && (cptr[length-2] == 'd')) || ((cptr[length-6] == 'b') && (cptr[length-5] == 'e') && (cptr[length-4] == 'g') && (cptr[length-3] == 'i') && (cptr[length-2] == 'n')))
         {
            sscanf(cptr, "%s %s %s %s %s %s", date, time, sensorid, sensorvalue, activity_name, status);
         }
	 else
         {
            sscanf(cptr, "%s %s %s %s %s", date, time, sensorid, sensorvalue, activity_name);
	    is_independent_event = strcmp(activity_name, "nolabel") == 0 ? 0 : 1;        
	 }
	
 
	if (is_independent_event)
	 {
	    num = FindActivity(activity_name);
	    /*  add event to activity if it is already active
		Otherwise, 
		add it as a new occurance(both begin and end)
	    */
	    if (open[num] == 1)  
	    {
		AddActivity(date, time, sensorid, sensorvalue, num, 0, same, previous);
	    }
	    else                                           
	    {
		AddActivity(date, time, sensorid, sensorvalue, num, 1, same, previous);
		AddActivity(date, time, sensorid, sensorvalue, num, 1, same, previous);
		previous = num;
	    }
	    is_independent_event = 0;
	 }
         else if (strcmp(activity_name, "nolabel") == 0)
         {
	   //activity name is not present, so add this if there is any currently open activity
		if (same > 0)
		{
			for (i=0; i<numactivities; i++)
			{
				  if (open[i] == 1)
				  {
				     AddActivity(date, time, sensorid, sensorvalue, i, 0, same, previous);
				  }               
			}
		}
   
	 }
         else 
         {
	    //activity_name <begin/end>
            num = FindActivity(activity_name);
            same = AddActivity(date, time, sensorid, sensorvalue, num, 1, same, previous);
	    //printf("%s\n",cptr);
	    if (same == 0)   
            {//activity is finished, hence save this activity as previous activity
	       previous = num;
            }
	    for (i=0; i<numactivities; i++)// Check for other current activities
            {
               if ((i != num) && (open[i] == 1))
		{
                  AddActivity(date, time, sensorid, sensorvalue, i, 0, same, previous);	    
		}
	    }
         }
      }

      cptr = fgets(buffer, 256, fp);
   }

   //check if there are any activities that are not present in the data, then show the message
   for (i=0; i<numactivities; i++)
   {
      if (afreq[i] == 0)
      {    
	     printf("Activity %s is not found in the data\n", activitynames[i]);
      }
   }
}     


//returns the index of the activity name. Otherwise program aborts
int FindActivity(char *name)
{
   int i;
  // printf("name: %s",name);
   for (i=0; i<numactivities; i++)
      if (strcmp(name, activitynames[i]) == 0)
         return(i);

   printf("Unrecognized activity label %s\n", name);
   exit(1);
}


//adds the activity to aevents sequence
int AddActivity(char *date, char *time, char *sensorid, char *sensorvalue, int activity, int label, int same, int previous)
{  
   int occurrence, length;
   occurrence = afreq[activity];                   
   length = lengthactivities[activity][occurrence];
    
   if (evnum < MAXALENGTH)
   {
      ProcessData(activity, occurrence, length, date, time, sensorid, sensorvalue);
      lengthactivities[activity][occurrence] += 1;
     // printf("%s\n",activity);
      
     
      if(temperatureSensorKeywrd != NULL && strstr(sensorid,temperatureSensorKeywrd)== sensorid)
      {
	tempsensorValues[evnum-1] = atoi(sensorvalue);
	//printf("temp %d %d\n",evnum-1, tempsensorValues[evnum-1]);
      }
      else if(lightSensorKeywrd != NULL && strstr(sensorid,lightSensorKeywrd)== sensorid)
      {
	lightsensorValues[evnum-1] = atoi(sensorvalue);
	//printf("light %d %d\n",evnum-1, atoi(sensorvalue));
      }
      else if(lightSensorKeywrd != NULL || temperatureSensorKeywrd != NULL)
      {
	tempsensorValues[evnum-1] = -1;
	lightsensorValues[evnum-1] = -1;
      }
   }
   else
   {
      printf("Event %s %s %s %s\n", date, time, sensorid, sensorvalue);
      printf("Activity length for %s exceeds maximum\n", activitynames[activity]);
   }
   
   //starting new activity
   if ((label == 1) && (open[activity] == 0))
   {
      open[activity] = 1;
      previousactivity[activity][occurrence] = previous;
      starts[activity][occurrence] = evnum - 1;
      ++same;
      return same;
   }
   else if (label == 0)    
   { //as there is no label associated with this and no open activities, dont change the currently open activities count
      return(same);                                        
   }
   //end of an activity, increase the frequency of this activity and make activity closed
   else
   {
      afreq[activity] += 1;
      open[activity] = 0;
      
      starts[activity] = (int *) realloc(starts[activity], (afreq[activity] + 1) * sizeof(int));
      lengthactivities[activity] = (int *) realloc(lengthactivities[activity], (afreq[activity] + 1) * sizeof(int));
      previousactivity[activity] = (int *) realloc(previousactivity[activity], (afreq[activity] + 1) * sizeof(int));
      lengthactivities[activity][afreq[activity]] = 0;
      previousactivity[activity][afreq[activity]] = 0;
      starts[activity][afreq[activity]] = 0;
      return(same-1);
   }
}


// processing the information parsed from the file and converts into range values if applicable
void ProcessData(int activity, int occurrence, int length, char *dstr, char *tstr, char *sistr, char *svstr)
{
   char temp[4];
   int dow, sensorid, sensorvalue, tnum;
   //computing the day of week
   temp[0] = dstr[6];                                    
   temp[1] = '\0';
   dow = atoi(temp);
   if (dow == 7)
      dow += 30;
   else if (dow == 8)
      dow += 61;
   temp[0] = dstr[8];
   temp[1] = dstr[9];
   temp[2] = '\0';
   dow += atoi(temp);
   dow = dow % 7;

   // Compute time of day
   temp[0] = tstr[0];                                    
   temp[1] = tstr[1];
   temp[2] = '\0';
   tnum = atoi(temp);
   //get the sensor id
   sensorid = MapSensors(sistr);

   //checks whether the sensor is off, otherwise it is on
   if (strcmp(svstr, "OFF") == 0)
      sensorvalue = OFF;
   else 
      sensorvalue = ON;

   aevents[evnum][SENSOR] = sensorid;
   aevents[evnum][TIME] = tnum;
   aevents[evnum][DOW] = dow;
   aevents[evnum][SENSORVALUE] = sensorvalue;
   aevents[evnum][LABEL] = activity;
   evnum++;
}

//returns the logical sensor id of the physical sensor
int MapSensors(char *sistr)
{
   int i;

   for (i=0; i<numphysicalsensors; i++)
   {      
	if (strcmp(sensormap[i][0], sistr) == 0)
	{
          return(atoi(sensormap[i][1]));
	}
   }
   printf("\n sensor %s doesn't exist in the parameter file", sistr);
   return -1;
}


// These comparison function is a helper to sort data values for equal frequency binning.
int cmp(int *t1, int *t2)
{
   return(*t1 - *t2);
}


int comp(const void *t1, const void *t2)
{
   int *v1 = (int *) t1;
   int *v2 = (int *) t2;

   return(cmp(v1, v2));
}

/*
fill the range values in threshold for equal-frequency or user-supplied range
0 - user-supplied range
1 - equal frequency binning
SENSOR - no range values
TIME 1
DOW - no range values
PREVIOUS - no range values
#define LENGTH 4
*/
void SelectFeatures()
{
   int i, j, k, n, num, *data, tvalue, discretize;

   for (i=0; i<numfeatures; i++)
   { 
    //if 0, use 
      discretize = 1;
      if (selectfeatures[i] == 0)//default
      {
         if (i == TIME)
         {
	    for (j=0; j<numfeaturevalues[i]; j++)
	    {
		/*0-5: 0, 6-10: 1, 11-15: 2, 16-20: 3, 21-23: 4*/
	       if (j == 0)
	       {
	          thresholds[i][j] = 5;            
	       }
	       else if (j == 1)
	       {	       
		   thresholds[i][j] = 10;         
	       }	       
	       else if (j == 2)
	       {
	          thresholds[i][j] = 15;        
	       }	       
	       else 
	       {	
		   thresholds[i][j] = 20;     
	       }	    
	    }
         }
         else if (i == LENGTH) 
         {
	    for (j=0; j<numfeaturevalues[i]; j++)
	    {
		/*0-150:0, 150-500:1, >500:2*/
	       if (j == 0)
	          thresholds[i][j] = 150;            
	       else                               
	          thresholds[i][j] = 500;
	    }
         }
         else 
         {
		discretize = 0;
      	 }
      }
      else             //  equal frequency binning
      {
         num = 0;
         if (i == LENGTH)
         {
            for (j=0; j<numactivities; j++)
	       num += afreq[j];
         }
         else num = evnum;
      
         data = (int *) malloc(num * sizeof(int));
         n = 0;
         if (i == LENGTH)
         {
	    for (j=0; j<numactivities; j++)
	       for (k=0; k<afreq[j]; k++)
	          data[n++] = lengthactivities[j][k];
         }
         else
         {
	    for (j=0; j<evnum; j++)
	       data[n++] = aevents[j][i];
         }
   
         qsort(data, n, sizeof(int), comp);
   
	 if (outputlevel > 1)
            printf("The range values for feature %d are ", i);
         for (j=0; j<numfeaturevalues[i] - 1; j++)
         {
            tvalue = (j + 1) * (n / numfeaturevalues[i]);
            thresholds[i][j] = data[tvalue];
	    if (outputlevel > 1)
               printf("%d ", thresholds[i][j]);
         }
	 if (outputlevel > 1)
            printf("\n");
         free(data);
      }

      //process the data and change the values of the data
      if (discretize == 1)
      {
         if (i == LENGTH)        
         {
	    if (stream == 0)
	    {
               sizes = (int **) malloc(numactivities * sizeof(int *));
               for (j=0; j<numactivities; j++)
               {
	          sizes[j] = (int *) malloc(afreq[j] * sizeof(int));
                  for (k=0; k<afreq[j]; k++)
	          {
	             num = 0;
	             while ((num < (numfeaturevalues[i] - 1)) && (lengthactivities[j][k] > thresholds[i][num]))
                        num++;
	             sizes[j][k] = num;
	          }
               }
	    }
         }
         else                   
         {
	    for (j=0; j<evnum; j++)
	    {
	       num = 0;
	       while ((num < (numfeaturevalues[i] - 1)) && (aevents[j][i] > thresholds[i][num]))
                  num++;
	       aevents[j][i] = num;
	    }
         }
      }
   }
}


// assign range values to activity lengths
int DLength(int size)
{
   int num;

   num = 0;
   while ((num < (numfeaturevalues[LENGTH] - 1)) && (size > thresholds[LENGTH][num]))
      num++;

   return(num);
}


//gives the summary about the input data
void Summarize()
{
   int i, j, total=0, *altotal, *astotal, time;
   float *lmean, *lvariance, *smean, *svariance;

   //particular activity -all occurances activity lengths
   altotal = (int *) malloc(numactivities * sizeof(int));
   lmean = (float *) malloc(numactivities * sizeof(float));
   lvariance = (float *) malloc(numactivities * sizeof(float));
   astotal = (int *) malloc(numactivities * sizeof(int));
   //total of all activity occurance start times
   smean = (float *) malloc(numactivities * sizeof(float));
   svariance = (float *) malloc(numactivities * sizeof(float));

   for (i=0; i<numactivities; i++)
   {
      altotal[i] = 0;
      astotal[i] = 0;
      for (j=0; j<afreq[i]; j++)
      {
	 //total activities length count
         total += lengthactivities[i][j];
         altotal[i] += lengthactivities[i][j];
	 time = aevents[starts[i][j]][TIME];
         astotal[i] += time;
      }
      
      //calulating mean of activity lengths of all occurances
      if (afreq[i] == (float) 0)
      {
         lmean[i] = 0;
	 smean[i] = 0;
      }
      else
      {
         lmean[i] = (float) altotal[i] / (float) afreq[i];
         smean[i] = (float) astotal[i] / (float) afreq[i];
      }
      lvariance[i] = 0;
      svariance[i] = 0;
   }
   
   printf("________________________   SUMMARY   ______________________________\n");
   printf("Total number of useful sensor events:  %d\n", total);
   for (i=0; i<numactivities; i++)
   {  
      printf("Activity %s has %d occurrences\n", activitynames[i], afreq[i]);
      for (j=0; j<afreq[i]; j++)
      {  
         printf(" Occurrence %d started at valid event no: %d sensor id: %d,time: %d, used %d sensor events\n",j, starts[i][j], aevents[starts[i][j]][SENSOR], aevents[starts[i][j]][TIME], lengthactivities[i][j]);
	 float difference = lengthactivities[i][j] - lmean[i];
         lvariance[i] += difference * difference;
         time = aevents[starts[i][j]][TIME];
         svariance[i] += (float) ((time - smean[i]) * (time - smean[i]));
      }
      printf("\n");
      if (lmean[i] == (float) 0)//if mean is 0, variance is 0
      {
	   lvariance[i] = 0;
      }      
      else
      {
       	   lvariance[i] /= afreq[i];
      }
      if (smean[i] == (float) 0)
      {      
	   svariance[i] = 0;
      }
      else 
      {	
	svariance[i] /= afreq[i];
      }      
      printf("activity length: Mean = %f, variance = %f\n", lmean[i], lvariance[i]);
      printf("activity start time: mean = %f, variance = %f\n", smean[i], svariance[i]);
   }
   printf("___________________________________________________________________________\n");
}

//checks the mode and performs activity recognition if the mode includes testing. Otherwise trains the model
void performActivityRecognition()
{
   int i;
   printf("learn activity models\n");
   if (mode == BOTH)
   {
      Partition();
   }
   if (mode == TRAIN)
   {
      TrainInit();
      if (model == NB)
         NBCTrain(-1);
      else if (model == HMM)
         HMMTrain(-1);
      saveModel();
   }
   else if (mode == BOTH)
   {
      //k- fold cross validation
      for (i=0; i < CROSS_VALIDATION_FOLDS; i++)        
      {
         TrainInit();
         if (model == NB)
         {
            NBCTrain(i);
	    NBCTest(i);
         }
         else if (model == HMM)
         {
            HMMTrain(i);
            HMMTest(i);
         }
      }
   }
   else if (mode == TEST)
   {
      readModel();
      if (model == NB)
      {
         NBCTest(-1);
      }
      else if (model == HMM)
      {       
	  HMMTest(-1);
      }
   }
}


// Partition data streams into test and train examples.
void Partition()
{
   int i, j, num=0, count=0;

   for (i=0; i<numactivities; i++)
      for (j=0; j<afreq[i]; j++)
         num++;

   partition = (int *) malloc(num * sizeof(int));

   //deterministic pattern partition
   if (partitiontype == 1)                   
   {
      for (i=0; i<numactivities; i++)
         for (j=0; j<afreq[i]; j++)
	 {
	    partition[count] = count % CROSS_VALIDATION_FOLDS;
	    count++;
         }
   }
   // partition the activities randomly
   else if (partitiontype == 2)                     
   {
      for (i=0; i<numactivities; i++)
         for (j=0; j<afreq[i]; j++)
	 {
	    partition[count] = rand() % CROSS_VALIDATION_FOLDS;
	    count++;
         }
   }
}


// Report the results of activity recognition.
void printResults()
{
   if (outputlevel > 0)                             
   {
      printf("\n__________________________________________Confusion matrix____________________________________________\n");
      printf("          Class label\n   Actual");
      for (int i=0; i<numactivities; i++)
      {
         if (i < 10)
             printf("  %d ", i);
         else 
	     printf(" %d ", i);
      }
      printf("\n\n     ");
      for (int i = 0; i < numactivities; i++)
      {
	 for (int j = 4; j >= 0; j--)
	 {
            if (j >= strlen(activitynames[i]))
	    {	  
	     	activitynames[i][j] = ' ';
            }	 
	}         
	 printf("%c%c%c%c%c   ",activitynames[i][0], activitynames[i][1], activitynames[i][2], activitynames[i][3], activitynames[i][4]);
	 if (strlen(activitynames[i]) < 5)
	    printf(" ");
	 if (strlen(activitynames[i]) < 4)
	    printf(" ");
	 if (strlen(activitynames[i]) < 3)
	    printf(" ");
	 if (strlen(activitynames[i]) < 2)
	    printf(" ");

         for (int j=0; j < numactivities; j++)
         {
            printf("%d ", freq[i][j]);
	    if (freq[i][j] < 100)
	       printf(" ");
	    if (freq[i][j] < 10)
	       printf(" ");
         }

	 if (freq[i][i] == 0)
            printf("   (%f)\n     ", (float) 0.0);
         else 
	    printf("   (%f)\n     ", (float) freq[i][i] / (float) afreq[i]);
      }
   }
	
                                                       // Print accuracy results
   printf("right %d wrong %d Average accuracy is %f\n",
          right, wrong, (float) right / (float) (right + wrong));

   if ((outputlevel > 1) && (model == HMM))
   {
	printf("_______________________________________Emission probabilities__________________________________________________\n");
        for(int i = 0; i < numactivities; i++)
	{
		printf("activity: %s\n", activitynames[i]);
		for (int j=0; j<numfeatures; j++)
		 {
		    printf("-->");
		    for (int k=0; k<numfeaturevalues[j]; k++)
		       printLargeNumber(lemissionProb[i][j][k]);
		    printf("\n");
		 }
		printf("\n\n");
	}
	printf("_______________________________________Transition probabilities__________________________________________________\n");

	for (int i=0; i<numactivities; i++)
	{
	 printf("activity: %s\n", activitynames[i]);
	 for (int j=0; j<numactivities; j++)
	 {
	    printf("\t\t to %s : ", activitynames[j]);
	    printLargeNumber(ltr[i][j]);
	    printf("\n");
	 }
	}
   }
}


// Initialize variables used to train activity model.
void TrainInit()
{
   int i, j, k;

   for (i=0; i<numfeatures; i++)
      svalues[i] = 0;

   for (i=0; i<numactivities; i++)
   {
      stotal[i] = 0;
      prior[i] = (double) 0.0;

      for (j=0; j<numsensors; j++)
         sfreq[i][j] = 0;//store the frequency of each sensor for each activity

      for (j=0; j<numactivities; j++)
         tr[i][j] = (double) 0.0;//transition probability

      for (j=0; j<numfeatures; j++)
      {
         for (k=0; k<numfeaturevalues[j]; k++)
	 {	
	    evidence[i][j][k] = 0;
      	 }
      }
   }
}


// Initialize variables used to test activity model.
void TestInit()
{
   int i, j;

   for (i=0; i<numfeatures; i++)
   {
      svalues[i] = 0;
      for (j=0; j<numfeaturevalues[i]; j++)
      {
         testevidence[i][j] = 0;
      }
   }
   for (i=0; i<numactivities; i++)
   {
      likelihood[i] = makeLargeNumber((long double) 1);
   }
}


//store the values of current sensor event
void calculateState(int *event, int size, int previous)
{
   int length = -1;

   svalues[SENSOR] = event[SENSOR];
   svalues[TIME] = event[TIME];
   svalues[DOW] = event[DOW];
   svalues[PREVIOUS] = previous;
   if (stream == 1)
   {
      length = DLength(size);
   }
   else
   {	
      length = size;
   }
   if(length != -1)
   {
   	svalues[LENGTH] = length;
   }
}

//stores the frequency of every feature in each activity
void calculateEvidence(int **e)
{
   e[SENSOR][svalues[SENSOR]] += 1;
   e[TIME][svalues[TIME]] += 1;
   e[DOW][svalues[DOW]] += 1;
   e[PREVIOUS][svalues[PREVIOUS]] += 1;
   e[LENGTH][svalues[LENGTH]] += 1;
}

/*
 calculate prior probability for activity "i" based on the number of sensor events of activity i out of all events
 prior probability = no.events/no.total_events
 stotal[i] - count of all events for this activity i
*/
void calculatePrior()
{
   int total_events = 0;

   for (int i=0; i<numactivities; i++)
   {
      total_events += stotal[i];
   }

   for (int i=0; i<numactivities; i++)
   {
      prior[i] = (double) stotal[i] / (double) total_events; 
   }
}


//returns the sum of 2 large numbers
LargeNumber add(LargeNumber op1, LargeNumber op2)
{
   LargeNumber result;
   long int diff = 0;

   result.exponent = 0;
   result.mantissa = 0;

   if ((op2.exponent == 0) && (op2.mantissa == 0))
      result = op1;
   else if ((op1.exponent == 0) && (op1.mantissa == 0))
      result = op2;
   else
   {
      diff = abs(op1.exponent - op2.exponent);
      if (diff < DOUBLELIMIT)
      {
         if (op1.exponent < op2.exponent)
	 {
	    result.exponent = op1.exponent;
	    result.mantissa = op2.mantissa *
	                      powl(10, fabs(op1.exponent - op2.exponent));
	    result.mantissa += op1.mantissa;
	 }
	 else
	 {
	    result.exponent = op2.exponent;
	    result.mantissa = op1.mantissa *
	                      powl(10, fabs(op2.exponent - op1.exponent));
	    result.mantissa += op2.mantissa;
	 }
      }
      else
      {
         if (op1.exponent < op2.exponent)
	    result = op2;
	 else result = op1;
      }
      result = standardize(result);
   }

   return(result);
}


//returns the difference of 2 large numbers
LargeNumber subtract(LargeNumber op1, LargeNumber op2)
{
   op2.mantissa *= (-1.0);
   return(add(op1, op2));
}


// returns the product of 2 large numbers
LargeNumber multiply(LargeNumber op1, LargeNumber op2)
{
   LargeNumber result;

   result.mantissa = op1.mantissa * op2.mantissa;
   result.exponent = op1.exponent + op2.exponent;
   result = standardize(result);

   return(result);
}


//returns quotient of division operation in the format of large number
LargeNumber divide(LargeNumber op1, LargeNumber op2)
{
   LargeNumber result;

   result.mantissa = op1.mantissa / op2.mantissa;
   result.exponent = op1.exponent - op2.exponent;
   result = standardize(result);

   return(result);
}


// Convert double values to mantissa exponent format for floating-point arithmetic.
void makeAllLarge()
{


   for (int i=0; i<numactivities; i++)
   {
      if (prior[i] == (double) 0.0)
      {
         lprior[i] = min;
      }
      else
      {	
	 lprior[i] = makeLargeNumber((long double) prior[i]);
      }
      
      for (int j=0; j<numactivities; j++)           
      {
         if (tr[i][j] == (double) 0.0)
	    ltr[i][j] = min;
	 else ltr[i][j] = makeLargeNumber((long double) tr[i][j]);
      }

      for (int j=0; j<numfeatures; j++)
      {
         for (int k=0; k<numfeaturevalues[j]; k++)
         {
            if (emissionProb[i][j][k] == 0)
 	     {
		lemissionProb[i][j][k] = min;
	     }
	    else 
	     {		
		lemissionProb[i][j][k] = makeLargeNumber((long double) emissionProb[i][j][k]);
	     }         
	 }
      }
   }
}


// Read model parameters from a file.
void readModel()
{
   FILE *fp;
   char name[MAXSTR];
   int i, j, k, num;

   if (model == NB)
      sprintf(name, "%s.nbc", modelfilename);
   else if (model == HMM)
      sprintf(name, "%s.hmm", modelfilename);

   fp = fopen(name, "r");

   if (fp == NULL)
   {
      printf("Model file cannot be read\n");
      exit(1);
   }

   fscanf(fp, "%d\n", &numactivities);
   for (i=0; i<numactivities; i++)
      fscanf(fp, "%s ", activitynames[i]);
   fscanf(fp, "\n");
   fscanf(fp, "%d\n", &numfeatures);
   for (i=0; i<numfeatures; i++)
      fscanf(fp, "%d ", &numfeaturevalues[i]);
   fscanf(fp, "\n");
   for (i=0; i<numfeatures; i++)
   {
      if ((i == TIME) || (i == LENGTH))
      {
         for (j=0; j<numfeaturevalues[i] - 1; j++)
	    fscanf(fp, "%d ", &thresholds[i][j]);
      }
   }
   fscanf(fp, "\n");
   fscanf(fp, "%d\n", &numphysicalsensors);
   for (i=0; i<numphysicalsensors; i++)
      fscanf(fp, "%s %s ", sensormap[i][0], sensormap[i][1]);
   fscanf(fp, "\n");
   for (i=0; i<numactivities; i++)
      fscanf(fp, "%d ", &(stotal[i]));
   fscanf(fp, "\n");
   for (i=0; i<numactivities; i++)
   {
      for (j=0; j<numfeatures; j++)
         for (k=0; k<numfeaturevalues[j]; k++)
	    fscanf(fp, "%d	", &evidence[i][j][k]);
      fprintf(fp, "\n");
   }
   fscanf(fp, "\n");

   if (model == HMM)
      readHMM(fp);

   //preprocess the data
   for (i=0; i<numfeatures; i++)
   {
      if (i == LENGTH)
      {
         if (stream == 0)
         {
            sizes = (int **) malloc(numactivities * sizeof(int *));
	    for (j=0; j<numactivities; j++)
	    {
	       sizes[j] = (int *) malloc(afreq[j] * sizeof(int));
	       for (k=0; k<afreq[j]; k++)
	       {
	          num = 0;
	          while ((num < (numfeaturevalues[i] - 1)) &&
	                 (lengthactivities[j][k] > thresholds[i][num]))
                     num++;
                  sizes[j][k] = num;
	       }
	    }
         }
      }
      else if (i == TIME)
      {
	 for (j=0; j<evnum; j++)
	 {
	    num = 0;
	    while ((num < (numfeaturevalues[i] - 1)) &&
	              (aevents[j][i] > thresholds[i][num]))
               num++;
	    aevents[j][i] = num;
         }
      }
   }
   fclose(fp);
}


// Save model parameters to a file.
void saveModel()
{
   FILE *fp;
   char name[MAXSTR];
   int i, j, k;

   printf("model is %d\n", model);
   if (model == NB)
      sprintf(name, "%s.nbc", modelfilename);
   else if (model == HMM)
      sprintf(name, "%s.hmm", modelfilename);
   
   fp = fopen(name, "w");

   if (fp == NULL)
   {
      printf("Model file cannot be created\n");
      exit(1);
   }

   fprintf(fp, "%d\n", numactivities);

   for (i=0; i<numactivities; i++)
   {
     	 fprintf(fp, "%s ", activitynames[i]);
   }
   fprintf(fp, "\n");
   fprintf(fp, "%d\n", numfeatures);
 
   for (i=0; i<numfeatures; i++)
   {    
	  fprintf(fp, "%d ", numfeaturevalues[i]);
   }
   fprintf(fp, "\n");
   for (i=0; i<numfeatures; i++)
   {
      if ((i == TIME) || (i == LENGTH))
      {
         for (j=0; j<numfeaturevalues[i] - 1; j++)
	 {
	    	fprintf(fp, "%d ", thresholds[i][j]);
	 }      
      }
   }
   fprintf(fp, "\n");
   fprintf(fp, "%d\n", numphysicalsensors);
   
   for (i=0; i<numphysicalsensors; i++)
   {   
	   fprintf(fp, "%s %s ", sensormap[i][0], sensormap[i][1]);
   }

   fprintf(fp, "\n");
   for (i=0; i<numactivities; i++)
   {
      fprintf(fp, "%d ", stotal[i]);
   }
   fprintf(fp, "\n");
   for (i=0; i<numactivities; i++)
   {
      for (j=0; j<numfeatures; j++)
      {    
	     for (k=0; k<numfeaturevalues[j]; k++)
	     {		
		    fprintf(fp, "%d	", evidence[i][j][k]);
	     }    
	  fprintf(fp, "\n");
   	} 
    }  

   if (model == HMM)
      saveHMM(fp);
   
   fclose(fp);
}


// Convert double to mantissa exponent number representation.
LargeNumber makeLargeNumber(long double num)
{
   LargeNumber result;

   result.exponent = 0;
   result.mantissa = num;
   result = standardize(result);

   return(result);
}


// Convert large number to mantissa exponent format for floating-point arithmetic.
LargeNumber standardize(LargeNumber num)
{
   long double sign = 1.0;
   LargeNumber result = num;

   if (result.mantissa != 0)
   {
      if (result.mantissa < 0)
      {
         result.mantissa = (long double) fabs(result.mantissa);
	 sign = -1.0;
      }
      while (result.mantissa >= 10)
      {
	 result.mantissa /= 10;
	 result.exponent += 1;
      }
      while (result.mantissa < 1)
      {
         result.mantissa *= 10;
         result.exponent -= 1;
      }

      result.mantissa = sign * result.mantissa;
   }
   else
   {	
	 result.exponent = 0;
   }
   return result;
}


// checks if 2 large numbers are equal
int isEqual(LargeNumber op1, LargeNumber op2)
{
   if ((op1.exponent == op2.exponent) && (op1.mantissa == op2.mantissa))
      return(TRUE);
   else return(FALSE);
}


// checks if op1 is greater than op2
int isGreaterThan(LargeNumber op1, LargeNumber op2)
{
   LargeNumber temp;

   // Handle case where one number is negative and the other is not
   if ((op1.mantissa < 0) && (op2.mantissa >= 0))
     return (FALSE); 
   if ((op1.mantissa >= 0) && (op2.mantissa < 0))
     return (TRUE);
     
   // If both negative, then make positive and swap
   if ((op1.mantissa < 0) && (op2.mantissa < 0))
   {
     temp.exponent = op1.exponent;
     temp.mantissa = (-1.0 * op1.mantissa);
     op1.exponent = op2.exponent;
     op1.mantissa = (-1.0 * op2.mantissa);
     op2 = temp;
   } 
   
   if (op1.exponent > op2.exponent)
   {
      return(TRUE);
   }
   else if (op1.exponent == op2.exponent)
   {
      if (op1.mantissa > op2.mantissa)
      {
         return(TRUE);
      }
   }

   return(FALSE);
}


// Print the description of a sensor event.
void printEvent(int *event)
{
   printf("%d %d %d %d %d\n",event[SENSOR], event[TIME],  event[DOW], event[SENSORVALUE], event[LABEL]);
}


void printLargeNumber(LargeNumber new)
{
   printf(" %fe%d ", (float) new.mantissa, (int) new.exponent);
}


//free the memory
void Finish()
{
   int i, j;

   for (i=0; i<numactivities; i++)
   {
      for (j=0; j<numfeatures; j++)
      {
         free(evidence[i][j]);
         free(emissionProb[i][j]);
         free(lemissionProb[i][j]);
      }
      free(lengthactivities[i]);
      free(previousactivity[i]);
      free(starts[i]);
      free(evidence[i]);
      free(emissionProb[i]);
      free(lemissionProb[i]);
      free(freq[i]);
      if (sizes != NULL)
         free(sizes[i]);
      free(tr[i]);
      free(sfreq[i]);
      free(ltr[i]);
      free(activitynames[i]);
   }
   for (i=0; i<numfeatures; i++)
   {
      free(testevidence[i]);
      free(thresholds[i]);
   }
   free(afreq);
   free(open);
   free(lengthactivities);
   free(previousactivity);
   free(freq);
   free(prior);
   free(lprior);
   free(stotal);
   free(svalues);
   free(selectfeatures);
   free(aevents);
   free(starts);
   if (sizes != NULL)
      free(sizes);
   free(numfeaturevalues);
   free(tr);
   free(sfreq);
   free(ltr);
   free(evidence);
   free(testevidence);
   free(thresholds);
   free(emissionProb);
   free(lemissionProb);
   free(likelihood);
   free(activitynames);
}

void applyfuzzyrules(double prob, char* activity, double temp_sensor_val, double light_sensor_val)
{
	/*
		TemperatureSensors: {very low, low, medium, high, very high} - {<=10, 11-21, 22-32, 33-43,>=44 }
		lightSensors: {dark, light, too much light} - { <35, 35- 50, >50}
	*/
	//light temperature sensors and activity predicted
	//time, day of week and activity predicted
	bool ac_on = false;
	bool light_on = false;
        bool exhaust_fan = false;
	bool geyser = false;
	if(temp_sensor_val!= -1 && temp_sensor_val <= 10)
	{
		ac_on = false;
	}
	if(temp_sensor_val!= -1 && temp_sensor_val >= 22)
	{
		ac_on = true;
	}
	if( light_sensor_val <= 50)
	{
		light_on = true;
	}
	if( light_sensor_val > 50)
	{
		light_on = false;
	}
	if(strcmp(activity, "Leave_Home")==0)
	{
		ac_on = false;
		light_on = false;
	}
	if(strcmp(activity, "sleep")==0 && temp_sensor_val != -1 && temp_sensor_val <= 10 )
	{
		ac_on = false;
		light_on = false;
	}
	if(strcmp(activity, "sleep")==0 && temp_sensor_val != -1 && temp_sensor_val >= 11 )
	{
		ac_on = true;
		light_on = false;
	}
	if(strcmp(activity, "Cook")==0 || strcmp(activity, "Cook_Breakfast")==0 || strcmp(activity, "Cook_Dinner")==0 || strcmp(activity, "Cook_Lunch")==0)
	{
		exhaust_fan = true;
	}
	if(strcmp(activity, "Bathe")==0)
	{
		geyser = true;
	}
	printf("activity: %s temp_s_val: %lf light_s_val: %lf ac: %d light_on: %d geyser: %d exhaust_fan: %d \n", activity, temp_sensor_val, light_sensor_val, ac_on,light_on, geyser, exhaust_fan);
}

