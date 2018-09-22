#include "ar.h"
#include "hmm.h"
#include <unistd.h>
#include <sys/times.h>

/*training using HMM
evidence: stores the frequency of every sensor, times, days, activity lengths, count of each previous activity for each activity
*/

double pred_max_prob = 0.0;
void HMMTrain(int crossValidationNum)
{
   int  num=0, snum, id, start, previous, label;

   if (mode == TRAIN)
   {
      printf("HMM Train\n");
   }   
   else
   {
	 printf("\t Cross Validation fold: %d of %d...\n\n", crossValidationNum+1, CROSS_VALIDATION_FOLDS);
   }

   if (stream == 0)                           
   {
      for(int i=0; i<numactivities; i++)
      {
         
         for(int j=0; j<afreq[i]; j++)
         {
	    if((mode == TRAIN) || (CROSS_VALIDATION_FOLDS == 1) || (partition[num] != crossValidationNum))
            {
	       //get the event no of this activity and search for the events related to this activity using length of the activities
               start = starts[i][j];
               snum = lengthactivities[i][j];
	       for (int k=0, n = 0; n< snum; k++)
	       {
	          id = aevents[start+k][SENSOR];
		  if (aevents[start+k][LABEL] == i)
		  {
		     calculateState(aevents[start+k], sizes[i][j], previousactivity[i][j]);
	             sfreq[i][id] += 1;
	             calculateEvidence(evidence[i]);
                     stotal[i] += 1;//count of all events for this activity
		     n++;
	          }
	       }
	       // Update transition frequency from previous activity to this activity
	       tr[previousactivity[i][j]][i] += (double) 1.0;
            }
            num++;
         }
      }
   }
   else          
   {
      previous = 0;
      start = 0;
      for (int i=0; i<evnum; i++)
      {
	 label = aevents[i][LABEL];
	 id = aevents[i][SENSOR];
	 calculateState(aevents[i], i - start, previous);
	 sfreq[label][id] += 1;
	 stotal[label] += 1;
	 calculateEvidence(evidence[label]);
	 if (i > 0)
	    tr[previous][label] += (double) 1.0;
	 else tr[label][label] += (double) 1.0;
	 if (label != previous)
	 {
            previous = label;
	    start = i;
         }
      }
   }
}

/*calculate emission probability based on frequency of each feature out of total events of this activity
 each evidence variable has 5 arrays like sensor, time, dow, previous activity, length of activities */
void calculateEmission()
{
   double val, min = 0.0000001;

   for(int i=0; i<numactivities; i++)
   {
      for (int j=0; j<numfeatures; j++)
      {
         for (int k=0; k<numfeaturevalues[j]; k++)
	 {
	    if (evidence[i][j][k] == 0)
	    {
	       //to avoid 0 probability
	       val = min;
	    }
	    else 
	    {
		val = (double) evidence[i][j][k];
	    }
             // if there are no events for activity i, assign a minimum value
	    if (stotal[i] == 0)
	    {
	       emissionProb[i][j][k] = min;
	    }	    
	   else 
	   {
		emissionProb[i][j][k] = val / (double) stotal[i];
	   }         
	}
      } 
    }
}


//normalise the transition probabilities, so that their sum becomes 1
void normalizeTransitionProb()
{

   for (int i=0; i<numactivities; i++)
   {
      double total = 0.0;
      for(int j=0; j<numactivities; j++)
      {
         total += tr[i][j];
      }
      if (total != (double) 0)
      {
	      for(int j=0; j<numactivities; j++)
	      {
		    tr[i][j] /= total;
	      }
      }   
   }
}


// Use Viterbi algorithm to update likelihood of each activity given the most recent sensor event
void updateLikelihood(int *event)
{
   int i, j;
   LargeNumber emission;
   LargeNumber total = makeLargeNumber((long double) 0);
   LargeNumber zero = makeLargeNumber((long double) 0);

   //update likelihoods of each activity
   for (i=0; i<numactivities; i++)       
   {
      emission = makeLargeNumber((long double) 1);

      // Calculate the emission probability of activity i by multiplying the probabilities of each feature value   
      for (j=0; j<numfeatures; j++)
      {
         emission = multiply(emission, lemissionProb[i][j][svalues[j]]);
      }
      /* For each possible prior activity j, combine the probability of previous activity j with transition probability (j-->i) and emission probability to get updated probability. Note that likelihood was initialized earlier to the prior probability for the activity.*/
      for (j=0; j<numactivities; j++)
      {	
	 likelihood[i] = add(likelihood[i], multiply(lprior[j], multiply(ltr[j][i], emission)));
      }
   }

   // normalise the likelihoods and update lpriori
   for (i=0; i<numactivities; i++)
   {
      total = add(total, likelihood[i]);
   }
   // Normalize to make total equal 1
   for (i=0; i<numactivities; i++)
   {
      if (!isEqual(total, zero))
      {
         lprior[i] = divide(likelihood[i], total);
      } 
  }
}


// recognise the activity name of the sequence of sensor events
void HMMTest(int crossValidationNum)
{
   int i, j, k, n, num=0, snum, id, label=-1, class, prev, start, previous;

   if (mode == TEST)
    {    
	  printf("HMM Test\n");
    }
    else 
    {     
	printf("\tCV fold %d of %d ...\n\n", crossValidationNum+1, CROSS_VALIDATION_FOLDS);
    }
   calculatePrior();           
   calculateEmission();	
   normalizeTransitionProb();
   makeAllLarge();        
   double temp_sensor_val = -1.0;
   double light_sensor_val = -1.0;
   if (stream == 0)                         
   {
      for (i=0; i<numactivities; i++)                  
      {
         for (j=0; j<afreq[i]; j++)
         {
	    //generally CROSS_VALIDATION_FOLDS-1 partitions are used for training and only 1 is used for testing
	    if ((mode == TEST) || (CROSS_VALIDATION_FOLDS == 1) || (partition[num] == crossValidationNum))
            {
	       start = starts[i][j];
               TestInit();
               snum = lengthactivities[i][j];
	     
	       prev = -1;
               for (k=0, n=0; n<snum; k++)
               {
		   //only activity i events
		  if (aevents[start+k][LABEL] == i )
		  {
	             id = aevents[start+k][SENSOR];
		     
	             calculateState(aevents[start+k], sizes[i][j], previousactivity[i][j]);
	             calculateEvidence(testevidence);
	             //printf("snum: %d temp[%d] %d\n",snum, start+k, tempsensorValues[start+k]);
  		     temp_sensor_val = tempsensorValues[start+k] > 0.0 ? tempsensorValues[start+k]: temp_sensor_val;
  		     light_sensor_val = lightsensorValues[start+k] > 0.0 ? lightsensorValues[start+k]: light_sensor_val;
		     // calculate the likelihood of hidden state: activity given sensor event
		     updateLikelihood(aevents[start+k]);
	             prev = label;
		     if ((k == (snum-1)) && (outputlevel > 2))
		     {
		        printf(" event ");
		        printEvent(aevents[start+k]);
		     }
		     n++;
	          }
               }
               //get the label with maximum probability
	       label = getMax();       
	       //printf("check fuzzy:%d\n",check_fuzzy);
	       if(check_fuzzy)
		{
		//	printf("aply");
	       		applyfuzzyrules(pred_max_prob, activitynames[label], temp_sensor_val, light_sensor_val);
		}		       
		//store the right and wrong predictions and also store the value for the confusion matrix
		temp_sensor_val = -1;	
		light_sensor_val = -1;       
		if (outputlevel > 2)
	          printf("  activity %d label %d\n", i, label);
	       freq[i][label] += 1;
	       if (label == i)              
	          right++;
               else wrong++;
            }
            num++;
         }
      }
      printf("right- %d, wrong- %d\n", right, wrong);
   }
   else                                  
   {
      previous = 0;
      start = 0;
      for (i=0; i<evnum-WINDOW; i++)              
      {
         TestInit();
	 prev = 0;
	 for (j=0; j<WINDOW; j++)
	 {
	    label = aevents[i+j][LABEL];
	    id = aevents[i+j][SENSOR];
	    calculateState(aevents[i+j], i + j - start, previous);
	    calculateEvidence(testevidence);
	    updateLikelihood(aevents[i+j]);
	    if (aevents[i+j][LABEL] != previous)
	    {
	       previous = aevents[i+j][LABEL];
	       start = i+j;
	    }
	    prev = 1;
	 }
	 if ((i+j) < (evnum-1))
	 {
	    if (label != aevents[i+j][LABEL])
	    {
	       class = getMax();
	       freq[label][class] += 1;
	       if (class == label)
	          right++;
               else wrong++;
	    }
	 }
      }
   }
}


// Return activity with the maximum likelihood.
int getMax()
{
   int i, activity=0;
   LargeNumber max = makeLargeNumber((long double) 0);

   for (i=0; i<numactivities; i++)
   {
      if (isGreaterThan(likelihood[i], max))
      {
         max = likelihood[i];
	 activity = i;
      }
   }
   return(activity);
}


// Save HMM to a file.
void saveHMM(FILE *fp)
{
   int i, j;

   for (i=0; i<numactivities; i++)
      for (j=0; j<numactivities; j++)
         fprintf(fp, "%lf ", tr[i][j]);
   fprintf(fp, "\n");
}


// Read HMM from a file.
void readHMM(FILE *fp)
{
   int i, j;

   for (i=0; i<numactivities; i++)
      for (j=0; j<numactivities; j++)
         fscanf(fp, "%lf ", &(tr[i][j]));
   fscanf(fp, "\n");
}
