// 174101006_HMM.cpp : Defines the entry point for the console application.
//
#ifndef HMM_H
#define HMM_H
#include "stdafx.h"
#include "iostream"
#include "string"
#include "fstream"
#include "vector"
#include "stdlib.h"
#include "sstream" 
#include "math.h"
#include "map"
#include<cstdlib>
#include <algorithm>
#include <random>
#include "Utility.h"
#include <time.h>

#define STATES 5 // no of states in HMM
#define OBSERVATIONS 8 // no of distinct observations per state

using namespace std;
Utility utility;
long double arr[] = {0.9,0.1,0,0,0,0,0.9,0.1,0,0,0,
	0,0.9,0.1,0,0,0,0,0.9,0.1,0,0,0,0,1};

int TIME = 0;

class HMM
{
public:
		double pi[STATES]; //initial state distribution
		long double A[STATES][STATES];
		long double B[STATES][OBSERVATIONS], **alpha, **beta, **gamma, **delta, ***XI;
		int **si;
		vector<int> observations, stateSequence;
		vector<float> samples, energies;
		vector<int> zcr;

		//Read from Properties file
		string recorded_file;
		string obs_sequence;
		string wavFilePath;
		string recordingModuleExeFileName;
		string model_file_path;
		int inputDuration;
		int batchStep ;
		float normalisationAmplitude;
		int Kvalue;
		int hammingWindowSize;
		int highest_amplitude_index ;
		int frame_shift_size;
		double averageWeight;
		map<string,string> prop_map;
	
		int samplesCount;
		float samplesSum;
		float dc_shift;
		float highest_amplitude;
		int startPoint;
		int endPoint;
		int frame_count;
		int max_add_silence_frames;

		HMM()
		{
			initialise_properties();
			pi[0] = 1; //initial state distribution
			pi[1] = 0;
			pi[2] = 0;
			pi[3] = 0;
			pi[4] = 0;
		}

		void HMMalphaBetaGammaObsSeqInitialiser()
		{
			populateObservations();
			if(TIME != 0)
			{
				alpha = (long double**)malloc(TIME * sizeof(long double));
				for (int i=0; i<TIME; i++)
					alpha[i] = (long double *)malloc(STATES * sizeof(long double));
				
				beta = (long double**)malloc(TIME*sizeof(long double));
				for (int i=0; i<TIME; i++)
					beta[i] = (long double *)malloc(STATES * sizeof(long double));
				
				gamma = (long double**)malloc((TIME-1)*sizeof(long double)); 
				for (int i=0; i<TIME-1; i++)
					gamma[i] = (long double *)malloc(STATES * sizeof(long double));
				
				delta = (long double**)malloc(TIME * sizeof(long double)); 
				for (int i=0; i<TIME; i++)
					delta[i] = (long double *)malloc(STATES * sizeof(long double));
				
				XI = (long double***)malloc((TIME-1) * sizeof(long double));
				for (int i=0; i<TIME; i++)
				{
					XI[i] =  (long double **)malloc(STATES * sizeof(long double));
					for (int j=0; j<STATES; j++)
					{
						XI[i][j] = (long double *)malloc(STATES * sizeof(long double));
					}
				}
				
				si = (int**)malloc(TIME * sizeof(int));
				for (int i=0; i<TIME; i++)
					si[i] = (int *)malloc(STATES * sizeof(int));
				
			}
		}

		double getProbability()
		{
				return forwardProcedure();
		}

		void initialiseHMM()
		{
			try
			{
				int count = 0;
				while(count < 100)
				{
					cout <<endl<<endl<<"***********************************" << count+1 << "Iteration*************************************"<<endl;
					forwardProcedure();
					backwardProcedure();
					viterbiAlgorithm();
					calculateXIMatrix();
					calculateGammaMatrix();

					calculatePI();
					calculateTransitionMatrix();
					calculateBMatix();

					remodelMatrices();
					count++;
				}
			}
			catch (std::exception& e)
			{
				cerr << "Sorry !! Unable to process!!! " << endl;
			}
		}

	//initialises the values
	void initialiseBakisModel()
	{
		try
		{
			int k = 0;
			for(int i = 0; i < STATES; i++)
			{
				for(int j = 0; j < STATES; j++)
				{
					A[i][j] = arr[k];
					k++;
				}
			}
			is_A_StochasticMatrix();
			populateBmatrix(true);
			is_B_StochasticMatrix();
		}
		catch (std::exception& e)
		{
			cerr << "exception caught in initialising the properties. Cause: " << e.what() << endl;
			throw e;
		}
	}

	//initialises the values
	void initialiseModel(long double AMatrix[][STATES], long double BMatrix[][OBSERVATIONS], long double PiMatrix[])
	{
		try
		{
			for(int i = 0; i < STATES; i++)
			{
				for(int j = 0; j < STATES; j++)
				{
					A[i][j] = AMatrix[i][j];
				}
			}

			for(int i = 0; i < STATES; i++)
			{
				for(int j = 0; j < OBSERVATIONS; j++)
				{
					B[i][j] = BMatrix[i][j];
				}
			}

			for(int j = 0; j < STATES; j++)
				{
					pi[j] = PiMatrix[j];
				}

			is_A_StochasticMatrix();
			is_B_StochasticMatrix();
		}
		catch (std::exception& e)
		{
			cerr << "exception caught in initialising the properties. Cause: " << e.what() << endl;
			throw e;
		}
	}

	void initialise_properties()
	{
		try
		{
			string filename = "Recognition.properties";
			utility.checkFileExistence(filename);
			ifstream file(filename.c_str());
			string str;
			while (getline(file,str)) 
				{
					int nPos = str.find('=');
					if (string::npos == nPos) 
						continue;

					string key = str.substr(0,nPos);
					string value = str.substr(nPos + 1,str.length() - nPos + 1);
					prop_map.insert(pair<string, string>(key,value));
				}
			recorded_file = prop_map["record_file_path"] ;
			batchStep =  stoi(prop_map["batch_step"]) ;
			normalisationAmplitude =  stof(prop_map["normalisation_amplitude"]) ;
			wavFilePath = prop_map["wav_file_path"] ;
			inputDuration = stoi(prop_map["record_input_duration"]) ;
			recordingModuleExeFileName = prop_map["recording_module_exe_file_name"];
			Kvalue = stoi(prop_map["Kvalue"]);
			hammingWindowSize = stoi(prop_map["hamming_window_size"]);
			frame_shift_size = stoi(prop_map["frame_shift_size"]);
			frame_count = stoi(prop_map["frame_count"]);
			max_add_silence_frames = stoi(prop_map["max_add_silence_frames"]);
			obs_sequence = prop_map["obs_sequence_file_path"];
			model_file_path = prop_map["model_file_path"];
		}
		catch (std::exception& e)
		{
			cerr << "exception caught in initialising the properties. Cause: " << e.what() << endl;
			throw e;
		}
	}

	void remodelMatrices()
	{
		long double value = pow(10.0,-80.0);
		for(int i = 0; i < STATES; i++)
		{
			int count = 0;
			for(int j =0; j < STATES ; j++)
			{
				if(A[i][j] == 0)
				{
					count++;
					A[i][j] = value;
				}
			}
			for(int j =0; j < STATES && count > 0; j++)
			{
				while(count > 0 && A[i][j] > value)
				{
					A[i][j] -= value;
					count--;
				}
			}
		}

		for(int i = 0; i < STATES; i++)
		{
			int count = 0;
			for(int j =0; j < OBSERVATIONS ; j++)
			{
				if(B[i][j] == 0)
				{
					count++;
					B[i][j] = value;
				}
			}
			for(int j =0; j < OBSERVATIONS && count > 0; j++)
			{
				while(count > 0 && B[i][j] > value)
				{
					B[i][j] -= value;
					count--;
				}
			}
		}
	}

	//calculates dc shift
	void calculateDcShift()
	{
		try
		{
			printf("*****************************Calculating DC Shift***************************\n");
			if(samples.size() > 0)
			{
				dc_shift = samplesSum/samples.size();
			}
			cout << "DC Shift: " << dc_shift << endl;
		}
		catch (std::exception& e)
		{
			cerr << "Sorry !! Unable to calculate DC shift!!! " << endl;
		}
	}

	//applies dc shift to all the samples
	void applyDCshift()
	{
		try
		{
			printf("*****************************Applying DC Shift***************************\n");
			for (int i = startPoint; i<endPoint; ++i) 
			{
				 samples[i] -= dc_shift; 
			}
		}
		catch (std::exception& e)
		{
			cerr << "Sorry !! Unable to apply DC Shift!!! " << endl;
		}
	}

	//calculates normalisation factor and multiply each sample with this factor to normalise all the samples
	void normalise()
	{
		try
		{
			printf("*****************************Normalising the samples***************************\n");
			if(highest_amplitude != 0.0)
			{
				float normalisation_factor = normalisationAmplitude/highest_amplitude;
				cout << "NormalisationFactor: " << normalisation_factor <<endl; 

				//Normalising the samples
				for (int i = startPoint; i<endPoint; i++) 
				{	
					samples[i] *= normalisation_factor; 
				}
			}
			else
			{
				cout << "No Normalisation is done" << endl;
			}
		}
		catch (std::exception& e)
		{
			cerr << "Sorry !! Unable to normalise!!! " << endl;
		}
	}

	void callRecordingModule()
	{
		try
		{
			int iterations = 7;
			int count = 7;
			while(count <= iterations)
			{
				cout << "Please record the numbers from 0 to 9 with an ample amount of gap in between two numbers "<< endl;
				string duration = to_string(static_cast<long long>(inputDuration));
				if(inputDuration > 0)
				{
					stringstream fileName;
					fileName << recorded_file;
					fileName << "_";
					fileName << count;
					stringstream wavfileName;
					wavfileName << wavFilePath;
					wavfileName << "_";
					wavfileName << count;
					string command = "\""+recordingModuleExeFileName+ "\"" + " " + duration +" "+ wavfileName.str() +" "+ fileName.str();
					cout << command <<endl;
					system(command.c_str());
					splitWaveFile(count, fileName.str());
					count++;
				}
				else
				{
					throw "Change to positive input duration" ;
				 
				}
			}
		}
		catch (std::exception& e)
		{
			cerr << "Sorry !! Unable to continue in call Recording Module!!! " << endl;
			throw e;
		}
		catch (const char* msg)
		{
			cerr << "Sorry !! Unable to continue in call Recording Module!!!Cause: " <<msg << endl;
			exit(1);
		}
	}

	void splitWaveFile(int iteration_no, string fileName)
	{
		try
		{
			 utility.checkFileExistence(recorded_file);
			 processRecordedFile(fileName);
			 calculateDcShift();
			 applyDCshift();
			 normalise();
			 splitSamples(iteration_no);
			 energies.clear();
			 zcr.clear();
		}
		catch (std::exception& e)
		{
			cerr << "Sorry !! Unable to process in split wave!!! " << endl;
		}
	}

	void processRecordedFile(string recordedFile)
	{
		printf("*****************************Starting File Processing***************************\n");
		ifstream file(recordedFile.c_str());
		string str;
		try
		{
			while (getline(file, str,'\r'))
			{
				populateSamples(str,"\n");
			}
			samplesCount = samples.size();
			endPoint = samplesCount;
			cout <<"Samples count: " <<  samplesCount << endl;
			file.close();
		}
		catch (std::exception& e)
		{
			cerr << "exception caught: " << e.what() << endl;
			throw e;
		}
	}

	//populate the sample
	void populateSamples(string str,string sep)
	{
		char* cstr=const_cast<char*>(str.c_str());
		char* current;
		float high_amp = 0.0;
		float value = 0.0;
		try
		{
			current=strtok(cstr,sep.c_str());
			while(current!=NULL)
			{
				value = stof(current);
				samplesSum += value;
				samples.push_back(value);
				high_amp = high_amp < abs(value) ? abs(value) :high_amp;
				current=strtok(NULL,sep.c_str());
			}
			cout << "highest amplitude:" << high_amp << endl;
		}
		catch (std::exception& e)
		{
			cerr << "exception caught: " << e.what() << endl;
			throw e;
		}
	}

	//split the recorded samples
	void splitSamples(int iteration_no)
	{
		try
		{
			printf("*****************************Calculating Energies to Find Start and End Point***************************\n");
			int start = 0, end = batchStep;
			int split_sample = 0;
			bool isEnergyCalculationDone = false;
			bool isSplittingDone = false;
			if(samplesCount > 0)
			{
				isEnergyCalculationDone = calculateEnergyZcr();
				if(isEnergyCalculationDone)
				{
					int energySize = energies.size() ;
					if(energySize > 4)
					{
						start = 4;
						while(!isSplittingDone)
						{
							double previousEnergy  = energies[start];
							double firstRatioBarrier = 10;
							double lastRatioBarrier = 0.07;
							double lastEnergyCheckValue = previousEnergy;
							bool isFirstTrimDone = false;
							bool isLastTrimDone = false;
							int i = start+1;
							for(; i< energySize && (!isFirstTrimDone || !isLastTrimDone) ; i++)
							{
								if((energies[i]/previousEnergy) > firstRatioBarrier && !isFirstTrimDone)
								{
									lastEnergyCheckValue = energies[i];
									startPoint = i;
									isFirstTrimDone = true;
								}
								if(isFirstTrimDone)
								{
									if(energies[i]/lastEnergyCheckValue < lastRatioBarrier && !isLastTrimDone)
									{
										endPoint = i;
										isLastTrimDone = true;
									}
								}
								previousEnergy = energies[i];
							}
							if(isFirstTrimDone && isLastTrimDone)
							{
								if(endPoint - startPoint > 2)// split into file if there are atleast 300 samples
								{
									createSampleFile(startPoint, endPoint, split_sample, iteration_no);
									split_sample++;
								}
								start = endPoint;
								cout << "Start Point: " << startPoint << " End Point: " << endPoint <<endl;  
							}
							if(isFirstTrimDone && !isLastTrimDone && i == energySize)
							{
								endPoint = i;
								isLastTrimDone = true;
								if(endPoint - startPoint > 2)// split into file if there are atleast 300 samples
								{
									createSampleFile(startPoint, endPoint, split_sample, iteration_no);
									split_sample++;
								}
								isSplittingDone = true;
								cout << "Start Point: " << startPoint << " End Point: " << endPoint <<endl;  
							}
							else if(i == energySize)
							{
								isSplittingDone = true;
							}
						}
					}
				}
			}
		}
		catch (std::exception& e)
		{
			cerr << "Sorry !! Unable to split the recording module into different samples!!! " << endl;
		}
	}

	//Calculate Energy and ZCR of the samples for every batch
	bool calculateEnergyZcr()
	{
		try
		{
			printf("*****************************Calculating Energies and ZCR***************************\n");
	
			int start = startPoint, end = start+batchStep;
			bool isDone = false;
			if(endPoint - startPoint > 0)
			{
				end = end > endPoint ? endPoint: end;
				while (!isDone) 
				{
					//re-initialise the values
					int zcr_count = 0;
					float energy_sum = 0.0;
					//To Check if this is last block
					if(end == endPoint)
					{
						isDone = true;
					}
		
					//calculate energy and zcr
					float previous = samples[start];		
					for (int i = start; i < end; i++) {
						energy_sum += pow(samples[i] , 2);
						if (previous * samples[i] <= 0.0) {
							zcr_count++;
						}
						previous = samples[i];
					}
					energies.push_back(energy_sum);
					zcr.push_back(zcr_count);

					//set the start and end for the next iteration
					start = end;
					end = end + batchStep;
					if(end > endPoint)
					{
						end = endPoint;
					}
				}
			}
			return isDone;
		}
		catch (std::exception& e)
		{
			cerr << "Sorry !! Unable to calculate energy and zcr for different batches!!! " << endl;
			throw e;
		}
	
	}

	//sample file creation after cutting the values
	void createSampleFile(int startPoint, int endPoint, int sample_no, int iteration_no)
	{
		try
		{
			startPoint = startPoint > max_add_silence_frames? startPoint-max_add_silence_frames : 0;
			endPoint = endPoint+max_add_silence_frames < energies.size() ? endPoint+max_add_silence_frames : energies.size();
			endPoint = endPoint*batchStep;
			endPoint = endPoint > samplesCount ? samplesCount: endPoint;
			startPoint = startPoint * batchStep;
			cout << "startPoint" << startPoint << ", End point: " << endPoint << ", sample no: " << sample_no << endl;
			stringstream fileSuffix;
			fileSuffix << sample_no;
			fileSuffix << "_";
			fileSuffix << iteration_no;
			ofstream outputfile("174101006_"+fileSuffix.str()+".txt");
			for(int i=startPoint; i < endPoint; i++)
			{
				outputfile << samples[i]<<"\n";
			}
		}
		catch (std::exception& e)
		{
			cerr << "Sorry !! Unable to create separate sample file!!! " << endl;
			throw e;
		}
	}

	//populates matrix B( states * observations) entries
	void populateBmatrix(bool equalProbability)
	{
		try
		{
			if(equalProbability)
			{
				double probability = 1.0/OBSERVATIONS;
				cout << " B probabilty " << probability << endl;
				for(int i = 0; i < STATES ; i++)
				{
					for(int j = 0; j< OBSERVATIONS ; j++)
					{
						B[i][j] = probability;// sets equal probability
					}
				}
			}
			else
			{
				cout << "Take B matrix probability of each observation per state" << endl;
				for(int i = 0; i < STATES ; i++)
				{
					for(int j = 0; j< OBSERVATIONS ; j++)
					{
						cout <<"Enter probability of observation " << j  << " on state" << i <<endl;
						double value = 0.0;
						scanf("%lf", &value);
						B[i][j] = value;// sets equal probability
					}
				}
			}
		//	printBMatrix(B, "B");
		}
		catch (std::exception& e)
		{
			cerr << "Sorry !! Unable to populate B matrix " << endl;
			throw e;
		}
	}

	//calculate alpha values for problem 1: Evaluation problem and find 
	//the probabilty of observation sequence given the model
	double forwardProcedure()
	{
		try
		{
		//	cout << "Forward" << endl;
			int t = 0;
			
			//initialise
			for(int i = 0 ; i < STATES ; i++)
			{
				alpha[t][i] = pi[i] * B[i][observations[t]];
			}
			//cout << "alpha" << endl;

			//induction
			for(t = 1; t < TIME ; t++)
			{
				for(int j = 0 ; j < STATES ; j++)
				{
					
					alpha[t][j] = B[j][observations[t]];
					double sum = 0.0;
					for(int i = 0; i < STATES; i++)
					{
						sum += alpha[t-1][i] * A[i][j];
					}
					alpha[t][j] *= sum;
				}
				cout <<"";
			}

			//termination : calculate probabilty of observation sequence given the model
			long double probabilty = 0.0;
			for(int i = 0; i < STATES; i++)
			{
				probabilty += alpha[TIME-1][i];
			}
			cout << "Probabilty of Observation sequence given the model :" << probabilty << endl;
			return probabilty;
		}
		catch (std::exception& e)
		{
			cerr << "Sorry !! Unable to process forward procedure " << endl;
			throw e;
		}
	}

	//calculate the beta matrix
	void backwardProcedure()
	{
		try
		{
			 //initialise
			  int t = TIME -1;
			  for(int i = 0 ;i < STATES ; i++)
			  {
				  beta[t][i] = 1.0;
			  }
			  t--;
			  for(; t >= 0 ; t--)
			  {
				  for(int i = 0; i< STATES; i++)
				  {
					  beta[t][i] = 0.0;
					  for(int j = 0; j <STATES; j++)
					  {
						  beta[t][i]  += A[i][j] *B[j][observations[t+1]] * beta[t+1][j];
					  }
				  }
			  }
			  //printMatrix(beta, "BETA");	
		}
		catch (std::exception& e)
		{
			cerr << "Sorry !! Unable to process backward procedure " << endl;
			throw e;
		}
	}

	//solution to problem 2
	void viterbiAlgorithm()
	{
		try
		{
			int t = 0;
			//initialise
			for(int i = 0; i < STATES; i++)
			{
				delta[t][i] = pi[i]* B[i][observations[t]];
				si[t][i] = 0;
			}

			for(t++; t < TIME ; t++)
			{
				for(int j = 0; j < STATES; j++)
				{
					double max = delta[t-1][0] * A[0][j];
					int index = 0;
					for(int i = 1; i < STATES; i++)
					{
						double current = delta[t-1][i] * A[i][j];
						if(current > max) // check for maximum delta
						{
							max = current;
							index = i;
						}
					}
					delta[t][j] = max * B[j][observations[t]];
					si[t][j] = index;
				}
			}
			//printMatrix(delta, "DELTA");
			//printIntMatrix(si,"SI");
			double max = -1.0;
			int index = -1;
			//decide the last state where delta is maximum
			for(int i = 0; i < STATES; i++)
			{
				if(max < delta[TIME-1][i])
				{
					max = delta[TIME-1][i];
					index = i;
				}
			}
			double p = max;
			cout << "P* " << p << endl;	

			stateSequence.clear();
			stateSequence.push_back(index);

			//backtracking the state sequence
			for(int t = TIME-2; t>= 0; t--)
			{ 
				stateSequence.push_back(si[t+1][stateSequence.back()]);
			}
			reverse(stateSequence.begin(),stateSequence.end());

			cout << "State Sequence:  " ;
			for(int i = 0 ; i < stateSequence.size() ; i++)
			{
				cout << stateSequence[i] << " ";
			}
			cout << endl;
		}
		catch (std::exception& e)
		{
			cerr << "Sorry !! Unable to process in veterbi algorithm " << endl;
			throw e;
		}
	}

	//generate random numbers and populate the observation sequence
	void populateObservations()
	{
		try
		{	
			observations.clear();
			utility.checkFileExistence(obs_sequence);
			char *current;
			ifstream file(obs_sequence.c_str());
			string str;
				while (getline(file, str,'\n'))
				{
					char* cstr=const_cast<char*>(str.c_str());
					observations.push_back(stof(cstr));
				}
				TIME = observations.size();
				file.close();
		}
		catch (std::exception& e)
		{
			cerr << "exception caught in loading Template: " <<obs_sequence << ". Cause: " << e.what() << endl;
			throw e;
		}
	}


	//caluclates XI matrix
	void calculateXIMatrix()
	{
		try
		{
			int t = 0;
			double sum = 0.0;
			for(; t < TIME-1; t++)
			{
				sum = 0.0;
				for(int i = 0; i <STATES; i++)
				{
					for(int j = 0; j< STATES; j++)
					{
						sum+=alpha[t][i] *A[i][j] * B[j][observations[t+1]] * beta[t+1][j];
					}
				}

				if(sum != 0) //only if denominator is not zero
				{
					for(int i = 0; i < STATES; i++)
					{
						for(int j = 0; j < STATES; j++)
						{
							XI[t][i][j] = (alpha[t][i] * A[i][j] * B[j][observations[t+1]] * beta[t+1][j]) / sum;
						}
					}
				}
			}
		}
		catch (std::exception& e)
		{
			cerr << "Sorry !! Unable to populate XI matrix " << endl;
			throw e;
		}
	}


	//calculates Gamma matrix
	void calculateGammaMatrix()
	{
		try
		{
			for(int t = 0; t < TIME-1 ; t++)
			{
				for(int i  = 0; i < STATES; i++)
				{
					gamma[t][i] =  0.0;
					for(int j  = 0; j < STATES; j++)
					{
						gamma[t][i] += XI[t][i][j]; 
					}
				}
			}
		}
		catch (std::exception& e)
		{
			cerr << "Sorry !! Unable to populate Gamma matrix " << endl;
			throw e;
		}
		//printMatrix(gamma,"gamma");
	}


	//recalculates PI matrix
	void calculatePI()
	{
		try
		{
			for(int i = 0; i < STATES; i++)
			{
				pi[i] = gamma[0][i];
			}
		}
		catch (std::exception& e)
		{
			cerr << "Sorry !! Unable to populate PI matrix " << endl;
			throw e;
		}
	}


	//recalulates A matrix
	void calculateTransitionMatrix()
	{
		try
		{
			for(int i = 0; i< STATES; i++)
			{
				for(int j = 0; j< STATES; j++)
				{
					A[i][j] = 0.0;
					long double denominator = 0.0, numerator = 0.0;
					for(int t = 0; t < TIME - 1; t++)
					{
						numerator += XI[t][i][j];
						denominator += gamma[t][i];
					}
					if(denominator != 0 &&  numerator != 0)
					{
						A[i][j] = numerator/denominator;
					}
				}
			}
			//printAMatrix(A,"A");
			is_A_StochasticMatrix();
		}
		catch (std::exception& e)
		{
			cerr << "Sorry !! Unable to populate A trasition probability matrix " << endl;
			throw e;
		}
	}

	//check if the sum of probabilities in a row is 1
	void is_A_StochasticMatrix()
	{
		try
		{
			for(int i = 0; i < STATES; i++)
			{
				double sum = 0.0;
				for(int j = 0; j < STATES; j++)
				{
					sum += A[i][j];
				}
				if(abs(sum - 1.0) > 0.00001)//ignore small differences
				{
					cout << "SUm not equal to 1 for " << i << "row" << endl;
					throw "Sum not equal to 1 for row" ;
				}
			}
		}
		catch (const char* msg)
		{
			cerr << "Sorry !! exception to check if A matrix is stochastic or not. Cause: "<<msg << endl;
			exit(1);
		}
	}

	//recalculates B matrix
	void calculateBMatix()
	{
		try
		{
			for(int j = 0; j < STATES; j++)
			{
				for(int k = 0; k < OBSERVATIONS; k++)
				{
					//cout << "B: " <<j << " "<< k <<endl;
					long double numerator = 0.0, denominator = 0.0;
					for(int t = 0; t< TIME-1 ; t++)
					{
						denominator += gamma[t][j];
						if(observations[t] == k) // if Observation at t is the particulat observation K
						{
							numerator += gamma[t][j];
						}
					}
					B[j][k] = 0.0;
					if(numerator != 0.0 && denominator!= 0.0)
					{
						B[j][k] = numerator/denominator;
					}
				}
			}
		}
		catch (std::exception& e)
		{
			cerr << "Sorry !! Unable to populate B matrix " << endl;
			throw e;
		}
		//printBMatrix(B,"B");
	}

	//checks if matrix B has all rows whose sum is 1
	void is_B_StochasticMatrix()
	{
		try
		{
			for(int i = 0; i < STATES; i++)
			{
				double sum = 0.0;
				for(int j = 0; j < OBSERVATIONS; j++)
				{
					sum += B[i][j];
				}
				if(abs(sum - 1.0) > 0.00001)//ignore small differences
				{
					throw "Sum is not equal to 1" ;
				}
			}
		}
		catch (const char* msg)
		{
			cerr << "Sorry !! Unable to check if B matrix is stochastic or not. Cause: "<<msg << endl;
			exit(1);
		}
	}

	//check if gamma matrix is stochastic
	void is_Gamma_stochastic()
	{
		for(int i = 0; i < TIME; i++)
		{
			double sum = 0.0;
			for(int j = 0; j < STATES; j++)
			{
				sum += gamma[i][j];
			}
			if(abs(sum - 1.0) > 0.00001)//ignore small differences
			{
				exit(1);
			}
		}
	}

	//prints the matrix values
	void printMatrix(long double arr[][STATES], string name)
	{
		cout <<"___________________________" <<name<<" MATRIX_____________________________"<< endl;
		for(int t = 0; t <TIME; t++)
		{
			for(int i = 0; i < STATES; i++)
			{
				cout<< arr[t][i] << "\t" ;
			}
			cout << endl;
		}
	}

	//prints the matrix valuess
	void printBMatrix(long double arr[][OBSERVATIONS], string name)
	{
		cout <<"___________________________" <<name<<" MATRIX_____________________________"<< endl;
		for(int t = 0; t <STATES; t++)
		{
			for(int i = 0; i < OBSERVATIONS; i++)
			{
				cout<< arr[t][i] << "   " ;
			}
			cout << endl;
		}
	}

	//prints the matrix values
	void printAMatrix(long double arr[][STATES], string name)
	{
		cout <<"___________________________" <<name<<" MATRIX_____________________________"<< endl;
		for(int t = 0; t <STATES; t++)
		{
			for(int i = 0; i < STATES; i++)
			{
				cout<< arr[t][i] << "\t" ;
			}
			cout << endl;
		}
	}

	//prints the matrix values
	void printIntMatrix(int arr[][STATES], string name)
	{
		cout <<"___________________________" <<name<<" MATRIX_____________________________"<< endl;
		for(int t = 0; t <TIME; t++)
		{
			for(int i = 0; i < STATES; i++)
			{
				cout<< arr[t][i] << "\t" ;
			}
			cout << endl;
		}
	}
};
#endif