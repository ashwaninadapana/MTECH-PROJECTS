// Yes_No_speech_processor.cpp : Defines the entry point for the console application.
#ifndef LCPProcessor_H
#define LCPProcessor_H
#include "stdafx.h"
#include "iostream"
#include "string"
#include "fstream"
#include "vector"
#include "sstream" 
#include "cmath"
#include "map"
#include "Utility.h"
#include<cstdlib>
using namespace std;

class LPCProcessor
{
public:
		Utility utility;
		vector<vector<double>> cepstralData;
		vector<float> samples, energies, HMsamples;
		vector<double> Rvalues;
		vector<vector<double>> cepstralCoefficients;
		vector<double> Acoefficients;
		vector<int> zcr;
		map<string,string> prop_map;

		//Read from Properties file
		string record_FilePath;
		string cepstral_file_path;
		string wavFilePath;
		string recordingModuleExeFileName;
		int record_inputDuration;
		int batchStep;
		float normalisationAmplitude;
		int Kvalue;
		int hammingWindowSize;
		int highest_amplitude_index;
		int frame_shift_size;
		map<string, vector<vector<double>>> templates;

		int samplesCount;
		float samplesSum;
		float dc_shift;
		float highest_amplitude;
		int startPoint;
		int endPoint;
		int frame_count;

	    LPCProcessor(string recordFile)
		{
			record_FilePath = recordFile;
		    cepstral_file_path = "";
		    wavFilePath = "";
		    recordingModuleExeFileName = "";
		    record_inputDuration = 0;
		    batchStep = 0;
		    normalisationAmplitude = 0;
		    Kvalue = 0;
		    hammingWindowSize = 0;
		    highest_amplitude_index = 0;
		    frame_shift_size = 0;
		    samplesCount = 0;
			samplesSum = 0;
			dc_shift = 0;
			highest_amplitude = 0;
		    startPoint = 0;
			endPoint = 0;
			frame_count = 0;
			start_process();
		}

private:
		void start_process()
		{
			try
			{
				initialise_properties();
				utility.checkFileExistence(record_FilePath);
				processFiles();
		
				calculateDcShift();
				applyDCshift();
		
				calculateHighestAmplitude();
				trimValues();
				normalise();

				performLPC();
			}
			catch (std::exception& e)
			{
				cerr << "Sorry !! error encountered while creating cepstral coefficients file!!! " << e.what() << endl;
			}
		}

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
				batchStep =  stoi(prop_map["batch_step"]) ;
				normalisationAmplitude =  stof(prop_map["normalisation_amplitude"]) ;
				wavFilePath = prop_map["wav_file_path"] ;
				record_inputDuration = stoi(prop_map["record_input_duration"]) ;
				recordingModuleExeFileName = prop_map["recording_module_exe_file_name"];
				Kvalue = stoi(prop_map["Kvalue"]);
				hammingWindowSize = stoi(prop_map["hamming_window_size"]);
				frame_shift_size = stoi(prop_map["frame_shift_size"]);
				frame_count = stoi(prop_map["frame_count"]);
				cepstral_file_path = prop_map["cepstral_file_path"];

			}
			catch (std::exception& e)
			{
				cerr << "exception caught in initialising the properties. Cause: " << e.what() << endl;
				throw e;
			}

		}

		void callRecordingModule()
		{
			string duration = to_string(static_cast<long long>(record_inputDuration));
			string command = "\""+recordingModuleExeFileName+ "\"" + " " + duration +" "+ wavFilePath +" "+ record_FilePath;
			cout << command <<endl;
			system(command.c_str());
		}

		void processTemplates(string template_file_paths)
		{
			map<string, string> templatePaths = parseTemplateFilePaths(template_file_paths, ",");
			map<string, string> :: iterator itr;
			for(itr = templatePaths.begin(); itr != templatePaths.end(); itr++)
			{
				cout << itr->first << " => " << itr->second << '\n';
				loadTemplate(itr->first, itr->second);
			}
		}

		void loadTemplate(string key,string filename)
		{
			utility.checkFileExistence(filename);
			char *current;
			vector<vector<double>> CtemplateVector(frame_count, vector<double>());
			int frameno = 0;
			ifstream file(filename.c_str());
			string str;
			try
			{
				while (getline(file, str,'\n'))
				{
					char* cstr=const_cast<char*>(str.c_str());
					current=strtok(cstr,",");
					while(current!=NULL)
					{
						double value = stof(current);
						CtemplateVector[frameno].push_back(value);
						current=strtok(NULL,",");
					}
					frameno++;
				}
				templates.insert(pair<string, vector<vector<double>>>(key,CtemplateVector));
				file.close();
			}
			catch (std::exception& e)
			{
				cerr << "exception caught in loading Template: " <<filename << ". Cause: " << e.what() << endl;
				throw e;
			}

		}

		map<string,string> parseTemplateFilePaths(string template_file_paths, string seperator)
		{
			char* cstr=const_cast<char*>(template_file_paths.c_str());
			char* current;
			map<string, string> templatePaths;
			cout << "Templates: " << endl;
			try
			{
				current=strtok(cstr,seperator.c_str());
				while(current!=NULL)
				{
					string dupCurrent = current;
					int nPos = dupCurrent.find(":");
					if (string::npos == nPos) 
							continue;

					string key = dupCurrent.substr(0,nPos);
					string value = dupCurrent.substr(nPos + 1,dupCurrent.length() - nPos + 1);
					templatePaths.insert(pair<string, string>(key,value));
					cout << current << endl;
					current=strtok(NULL,seperator.c_str());
				}
			}
			catch (std::exception& e)
			{
				cerr << "exception caught: " << e.what() << endl;
				throw e;
			}
			return templatePaths;

		}

		/*This is used to process the files and store the values*/
		void processFiles()
		{
			printf("*****************************Starting File Processing***************************\n");
			ifstream file(record_FilePath.c_str());
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

		//calculates dc shift
		void calculateDcShift()
		{
			printf("*****************************Calculating DC Shift***************************\n");
			if(samples.size() > 0)
			{
				dc_shift = samplesSum/samples.size();
			}
			cout << "DC Shift: " << dc_shift << endl;
		}

		//applies dc shift to all the samples
		void applyDCshift()
		{
			printf("*****************************Applying DC Shift***************************\n");
			for (int i = startPoint; i<endPoint; ++i) 
			{
				 samples[i] -= dc_shift; 
			}
		}

		void trimValues()
		{
			printf("*****************************Calculating Energies to Find Start and End Point***************************\n");
			int start = 5, end = batchStep;
			bool isDone = false;
			if(samplesCount > 0)
			{
				isDone = calculateEnergyZcr();
				if(isDone)
				{
					int energySize = energies.size() ;
					if(energySize > 0)
					{
							double previousEnergy  = energies[start];
							double firstRatioBarrier = 20;
							double lastRatioBarrier = 0.7;
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
						if(isFirstTrimDone && !isLastTrimDone && i == energySize)
						{
							endPoint = i;
							isLastTrimDone = true;  
						}
						energies.clear();
						zcr.clear();
						startPoint = (startPoint-3) * batchStep;
						endPoint = (endPoint+3) * batchStep;
					    startPoint = startPoint < 0 ? 0: startPoint;
					    endPoint = endPoint > samples.size()? samples.size(): endPoint;
						cout << "Start Point: " << startPoint << " End Point: " << endPoint <<endl;  
					}
				}
			}
			createTrimmedFile();
		}

		void createTrimmedFile()
		{
			try
			{
				FILE * outputfile;
				outputfile = fopen ("trimmedModel.txt", "w");
				for(int i = startPoint; i < endPoint ; i++)
				{
					fprintf(outputfile, "%lf\n",samples[i]);
				}
				fclose(outputfile);
			}
			catch (std::exception& e)
			{
				cerr << "Sorry !! Unable to create model file!!! " << endl;
				throw e;
			}
		}

		//calculates highest amplitude irrespective of sign
		void calculateHighestAmplitude()
		{
			float high_amp = 0.0;
			int highAmpIndex;
			for (int i = startPoint; i<endPoint; i++) 
			{
				 if(high_amp < abs(samples[i]))
				{
					high_amp = abs(samples[i]);
					highAmpIndex = i;
				}
			}
			highest_amplitude = high_amp;
			highest_amplitude_index = highAmpIndex;
			cout << "highest amplitude:" << highest_amplitude << endl;
		}

		//calculates normalisation factor and multiply each sample with this factor to normalise all the samples
		void normalise()
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
				calculateHighestAmplitude();
			}
			else
			{
				cout << "No Normalisation is done" << endl;
			}
		}

		//Calculate Energy and ZCR of the samples for every batch
		bool calculateEnergyZcr()
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
			printFloatVector(energies, "ENERGY VALUES");
			printIntVector(zcr, "ZCR_VALUES");
			return isDone;
		}

		//Calculate Energy and ZCR of the samples for every batch
		void calculateEnergyZcr( bool calZCR)
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
			printFloatVector(energies, "ENERGY VALUES");
			printIntVector(zcr, "ZCR_VALUES");
		}

		void applyHammingWindow(int start , int end)
		{
			int N = end- start -1;
			float constant = 2 * 3.14;
			if(start < end && samplesCount >= end)
			{
				for(int i = start ; i < end ;i++)
				{
					float weight = 0.54 - (0.46 * cos((constant * i)/N));
					HMsamples.push_back(samples[i]* weight);
				}
			}
		}

		void calculateR(int start, int end)
		{
			applyHammingWindow(start,end);
	
			//calculate R[i] : sum of samples and sample with a shift
			for(int p = 0; p <= Kvalue ; p++)
			{
				double sum = 0;
				for(int i = 0 ; i < HMsamples.size() - p ; i++)
				{
					sum += HMsamples[i] * HMsamples[i+p];
				}
				Rvalues.push_back(sum);
			}
		}

		void writeCepstralCoefficients()
		{
			ofstream outputfile(cepstral_file_path);
			
			//ofstream universe("digit_universe.csv", ios::app);
			try
			{
				for(int i=0; i < cepstralCoefficients.size(); i++)
				{
					int j=1;
					for(; j < cepstralCoefficients[i].size()-1 ; j++)
					{
						outputfile << cepstralCoefficients[i][j] << ",";
						//universe << cepstralCoefficients[i][j] << ",";
					}
					outputfile << cepstralCoefficients[i][j] << "\n";
					//universe << cepstralCoefficients[i][j] << "\n";
				}
				outputfile.close();
			}
			catch (std::exception& e)
			{
				cerr << "Sorry !! Unable to create separate cepstral coefficients file!!! " << endl;
				outputfile.close();
				throw e;
			}
		}

		void performLPC()
		{
			cout << "\n\n********************Linear Predictice Coding**************************" << endl;
			int count = 0;
			int start = startPoint;
			int end = start + hammingWindowSize;
			
			while(end <endPoint)
			{
				//cout << "\n\ncalculating R's for frame " << count +1 << endl;
				//cout << "SampleSize" << samplesCount <<  " , start  " << start << " , End:" << end << "for frame "<< count+1 << endl;
				calculateR(start,end);
				//cout << "calculating Alpha Coefficients's for frame " << count +1 << endl;
				calculateCoeffecientsByLevinsonDurbinAlgo();
				//cout << "calculating Cepstral Coefficients's for frame " << count +1 << endl;
				calculateCepstralCoefficients(count);
				//cout << "Minimising cepstral Coefficients's for frame " << count +1 << endl;
				minimiseCepstralCoefficientSensitivies(count);
				//Calculate start and End to evaluate Cepstral coefficients for next frame
				start += frame_shift_size;
				end = start + hammingWindowSize;
			
				//clear all vectors to calculate Cepstral coefficients for next frame
				clearVectorsToCalNextCepstral();
		
				//keeps track of no of frames for which cepstral coefficients are to be calculated
				count++;
			}
			writeCepstralCoefficients();
		}

		void calculateCoeffecientsByLevinsonDurbinAlgo()
		{
			double energy = Rvalues[0];
			vector<double> previousCoefficients;
			for(int i = 1 ; i <= Kvalue ;i++)
			{
				double coefficient = 0.0;
				for(int j = 1; j < i; j++)
				{
					coefficient += previousCoefficients[j-1]* Rvalues[i-j];
				}
				coefficient = (Rvalues[i]-coefficient)/energy;
				Acoefficients.push_back(coefficient);
				for(int j = 1; j < i ; j++)//Do not calculate the last calculated value
				{
					Acoefficients[j-1] = previousCoefficients[j-1] - (coefficient * previousCoefficients[i-j-1]);
				}
				previousCoefficients = Acoefficients;
				energy *= (1-pow(coefficient,2));
			}
		//	printDoubleVector(Acoefficients , "Alpha Values");
		}

		void calculateCepstralCoefficients(int frameno)
		{
			double gain = Rvalues[0];//energy
			int q = Kvalue;
			vector<double> Cvector;
			cepstralCoefficients.push_back(Cvector);
			cepstralCoefficients[frameno].push_back(log(gain));
			for(int m = 1; m <= Kvalue ; m++)
			{
				double value = Acoefficients[m-1];
				for(int k = 1; k <= m - 1 ; k++)
				{
					value += ((float)k/m) * cepstralCoefficients[frameno][k] * Acoefficients[m-k-1];
				}
				cepstralCoefficients[frameno].push_back(value);
			}
	
			for(int m = Kvalue+1 ; m <= q ; m++)
			{
				int value = 0;
				for(int k = m - Kvalue ; k <= m - 1 ; k++)
				{
					value +=  ((float)k/m) * cepstralCoefficients[frameno][k] * Acoefficients[m-k-1];
				}
				cepstralCoefficients[frameno].push_back(value);
			}

		}

		void minimiseCepstralCoefficientSensitivies(int frameno)
		{
			int q = Kvalue;
			for(int i = 0; i< q; i++)
			{
				double weight = 1+((double)(q/2) * sin((22/7.0)*i/q));
				cepstralCoefficients[frameno][i] = cepstralCoefficients[frameno][i]*weight;
			}
			//printDoubleVector(cepstralCoefficients[frameno]," Cepstral Coefficients" );
		}

		void clearVectorsToCalNextCepstral()
		{
			HMsamples.clear();
			Rvalues.clear();
			Acoefficients.clear();
		}

		//Prints the float values of vector
		void printFloatVector(vector<float> vect,string key)
		{
			cout << "*****************************" << key << "***************************" << endl;
			for (int i = 0; i<vect.size(); ++i) 
			{
				 cout << vect[i] << endl;
			}
			cout << endl;
		}

		void printDoubleVector(vector<double> vect,string key)
		{
			cout << "*****************************" << key << "***************************" << endl;
			for (int i = 0; i<vect.size(); ++i) 
			{
				cout << vect[i] << ",";
			}
			cout << endl;
		}

		//Print the integer values of vector
		void printIntVector(vector<int> vect,string key)
		{
			cout << "*****************************" << key << "***************************" << endl;
			for (int i = 0; i<vect.size(); ++i) 
			{
				 cout << vect[i] << "\t";
			}
			cout << endl;
		}

};
#endif