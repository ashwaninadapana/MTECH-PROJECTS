#include "ar.h"
#include "nb.h"

//train the naive bayes classifier so that it can be used for testing
void NBCTrain(int cvnum)
{
   int i, j, k, n, num=0, snum, start, label, previous;

   if (mode == TRAIN)
      printf("NB Train\n");
   else printf("NB Train (CV fold %d of %d) ...\n\n", cvnum+1, CROSS_VALIDATION_FOLDS);
   if (stream == 0) 
   {
      for (i=0; i<numactivities; i++)                
      {
         for (j=0; j<afreq[i]; j++)       
         {
	                                                
	    if ((mode == TRAIN) || (CROSS_VALIDATION_FOLDS == 1) || (partition[num] != cvnum))
            {
	       start = starts[i][j];
               snum = lengthactivities[i][j];
               for (k=0, n=0; n<snum; k++)
               {
		  if (i == aevents[start+k][LABEL])
		  {
	             calculateState(aevents[start+k], sizes[i][j], previousactivity[i][j]);
                     stotal[i] += 1;
	             calculateEvidence(evidence[i]);
		     n++;
	          }
               }
            }
            num++;
         }
      }
   }
   else  //streaming data
   {
      previous = 0;
      start = 0;
      for (i=0; i<evnum; i++)
      {
	 label = aevents[i][LABEL];
	 calculateState(aevents[i], i - start, previous);
	 stotal[label] += 1;
	 calculateEvidence(evidence[label]);
	 if (label != previous)
	 {
	    previous = label;
	    start = i;
	 }
      }
   }
}


//predict the activity based on sequence of events using naive bayes
void NBCTest(int cvnum)
{
   int i, j, k, n, snum, label, class, num=0, start, previous;
   double p[numactivities], minvalue, mprob;

   if (mode == TEST)
   {
      printf("NB Test\n");
   }
   printf("\tCV fold %d of %d ...\n\n", cvnum+1, CROSS_VALIDATION_FOLDS);
   //calculate initial pi matrix
   calculatePrior();           

   if (stream == 0)                         
   {
      for (i=0; i<numactivities; i++)                  
      {
        
         for (j=0; j<afreq[i]; j++)          
         {
	    if ((mode == TEST) || ( CROSS_VALIDATION_FOLDS == 1) || (partition[num] == cvnum))
            {
	       start = starts[i][j];
               TestInit();// Initialize test evidence
               snum = lengthactivities[i][j];
               for (k=0, n=0; n<snum; k++)
               {
		   //as events in aevents structure are overlapped, we need only the events related to this activity i
		  if (i == aevents[start+k][LABEL])
		  {
	             calculateState(aevents[start+k], sizes[i][j], previousactivity[i][j]);
		     calculateEvidence(testevidence);
		     n++;
	          }
               }
               //checks for activity or class which has the maximum probability
	       minvalue = 0;
	       mprob = 0;
	       for (k=0; k<numactivities; k++)
	       {
	          p[k] = calculateProb(prior[k], k);
	          if ((k == 0) || (p[k] < mprob))
	          {
	             minvalue = k;
	             mprob = p[k];
	          }
	       }
	       class = minvalue;
	       //store the frequency of the predicted values against actual activity
	       freq[i][class] += 1;
	       //store number of correct and incorrect results in order to show the accuracy
	       if (i == class)              
	          right++;
               else wrong++;
            }
            num++;
         }
      }
   }
   else                                   
   {
      previous = 0;
      start = 0;
      for (i=0; i<evnum-WINDOW; i++)       
      {
         TestInit();
	 label = aevents[i][LABEL];

	 for (j=0; j<WINDOW; j++)
	 {
	    calculateState(aevents[i+j], i + j - start, previous);
	    calculateEvidence(testevidence);
	    if (label != previous)
	    {
	       previous = label;
	       start = i;
	    }
         }
	 minvalue = 0;
	 mprob = 0;
	 for (j=0; j<numactivities; j++)
	 {
	    p[j] = calculateProb(prior[j], j);
	    if ((j == 0) || (p[j] < mprob))
	    {
	       minvalue = j;
	       mprob = p[j];
	    }
	 }
	 class = minvalue;

	 freq[label][class] += 1;
	 if (label == class)
	    right++;
         else wrong++;
      }
   }
}

/*
 Because feature values appear multiple times for each data point, we calculate the product of each occurrence of the feature value
 in order to compute the probability of the event features given an activity a, or P(e|a).
*/
double calculateProb(double p, int a)
{
   int i, j, trainval, testval;
   double ratio;
   
    // Start with prior probability
   p = (double) -1.0 * log(p);                  
   for (i=0; i<numfeatures; i++)
   {
      for (j=0; j<numfeaturevalues[i]; j++)
      {
	 //populated while reading the model
         trainval = evidence[a][i][j];
         testval = testevidence[i][j];
         //consider the evidence which has some value in it
	 if (testevidence[i][j] != 0)      
	 {
            //if there are no events corresponding to the feature and number of events related to this feature are not empty, replace the train value with some min value and calculate emission probability for this subfeature
            if (trainval == 0)             
               p -= log((double) testval) + log((double) MIN);
            else           
	    {
	       //emission probability of the subfeature is multiplied with the number of times this subfeature occured in this event
	       ratio = (double) trainval / (double) stotal[a];
	       p -= log((double) testval) + log(ratio);
	    }
         }
      }
   }

   return(p);
}
