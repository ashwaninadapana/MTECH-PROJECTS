// 174101006_HMM.cpp : Defines the entry point for the console application.
//
#ifndef RecordSplitter_H
#define RecordSplitter_H
#include "stdafx.h"
#include "iostream"
#include "string"
#include "fstream"
#include "vector"
#include "sstream" 
#include "cmath"
#include "map"
#include<cstdlib>
#include <algorithm>
#include <random>
#include <time.h>
#include "Utility.h"
#define STATES 5 // no of states in HMM
#define OBSERVATIONS 8 // no of distinct observations per state
#define TIME 160 //no of observations

using namespace std;

class RecordSplitter
{
	public: 
		vector<float> samples, energies;
		vector<int> zcr;

		//Read from Properties file
		string recorded_file;
		int batchStep;
		float normalisationAmplitude;
		int hammingWindowSize;
		int highest_amplitude_index;
		int frame_shift_size;
		map<string,string> prop_map;

		int samplesCount;
		float samplesSum ;
		float dc_shift;
		float highest_amplitude;
		int startPoint;
		int endPoint;
		int frame_count;
		int max_add_silence_frames;
		Utility utility ;

	RecordSplitter()
		{
			try
			{
				int count = 0;
				
				samplesCount = 0;
				samplesSum = 0;
				dc_shift = 0;
				highest_amplitude = 0;
				startPoint = 0;
				endPoint = 0;
				initialise_properties();
				callRecordingModule();
			}
			catch (std::exception& e)
			{
				cerr << "Sorry !! Unable to process!!! " << endl;
			}
		}

	private:
	void initialise_properties()
	{
		try
		{
			string filename = "Recognition.properties";
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
			hammingWindowSize = stoi(prop_map["hamming_window_size"]);
			frame_shift_size = stoi(prop_map["frame_shift_size"]);
			frame_count = stoi(prop_map["frame_count"]);
			max_add_silence_frames=stoi(prop_map["max_add_silence_frames"]);
		}
		catch (std::exception& e)
		{
			cerr << "exception caught in initialising the properties. Cause: " << e.what() << endl;
			throw e;
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
			utility.callRecordingModule();	
			splitWaveFile(13, recorded_file);
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
			fileSuffix << "english";
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
};
#endif