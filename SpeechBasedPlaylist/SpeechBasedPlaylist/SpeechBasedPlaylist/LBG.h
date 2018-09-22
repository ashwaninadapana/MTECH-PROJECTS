// vowelCodeBook.cpp : Defines the entry point for the console application.
#ifndef LBG_H
#define LBG_H
#include "stdafx.h"
#include "iostream"
#include "string"
#include "fstream"
#include "vector"
#include "sstream" 
#include "cmath"
#include "map"
#include <cstdlib>
#include <algorithm>
#include <random>
#include "Utility.h"

using namespace std;

class LBG
{
	public:
		
		struct cbook_entry
		{
			vector<vector<double>> t_vector;
			vector<double> centroid;
		};

		vector<cbook_entry> codeBook;
		vector<vector<double>> trainingData;
		vector<vector<double>> cepstralData;
		vector<int> observationSeq;
		string TRAINING_FILE_PATH;
		string codebookFile;
		int K_levels;
		int current_levels; 
		float splitting_factor;
		double THRESHOLD;
		string cepstal_file;
		string obs_sequence;
		Utility utility;

		 LBG()
		 { 
			TRAINING_FILE_PATH = "" ;
			K_levels = 0;
			current_levels = 0; 
			splitting_factor = 0.3;
			THRESHOLD = 0.1;
			cepstal_file = "";
			obs_sequence = "";	
			codebookFile = "";
		 };

		 void createCodeBook()
		 {
			process();
		 }

		 void initialiseCodeBook()
		 {
			initialise_properties();
			utility.checkFileExistence(codebookFile);
			readCodeBook();
		 }

		~LBG()
		{
		}

		void createObservationSequence()
		{
			initialiseCepstralData();
			if(codeBook.size() >0)
				{
					for(int i = 0 ; i < cepstralData.size(); i++)
					{
						double distance = utility.calculateTokuraDistance(cepstralData[i],codeBook[0].centroid);
						int observation = 0;
						for(int j = 1; j< codeBook.size(); j++)
						{
							double current = utility.calculateTokuraDistance(cepstralData[i],codeBook[j].centroid);
							if(current < distance) //min distance
							{
								distance = current;
								observation = j;
							}
						}
						observationSeq.push_back(observation);
					}
					writeObservationSequenceIntoFile();
				}
		}

	private:
			void process()
			{
					initialise_properties();
					//Read the file Universe.csv into vector
					initialiseTrainingData();
					cout << "Total Training Vectors: "<< trainingData.size() << endl;
					cout << "*********************************** LBG **************************************" << endl;
					processLBG();
					writeCodeBook();		
			}

			void writeCodeBook()
			{
				ofstream outputfile(codebookFile);

				try
				{
					for(int i=0; i < codeBook.size(); i++)
					{
						int j=0;
						for(; j < codeBook[i].centroid.size()-1 ; j++)
						{
							outputfile << codeBook[i].centroid[j] << ",";
						}
						outputfile << codeBook[i].centroid[j] << "\n";
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

			void readCodeBook()
			{
				ifstream file( codebookFile );
				string str;
				char* current;
				while (getline(file,str)) 
					{
						vector<double> newVector;
						char* cstr=const_cast<char*>(str.c_str());
						current = strtok(cstr,",");
						while(current!=NULL)
						{
							double value = stof(current);
							newVector.push_back(value);
							current=strtok(NULL,",");
						}
						cbook_entry cb_entry;
						cb_entry.centroid = newVector;
						codeBook.push_back(cb_entry);
					}
			}

		void processLBG()
		{
			current_levels = 1;
			double distortion_diff = 0.0;

			initialiseSingleVectorCodeBook();
			int count = 1;
			while(current_levels < K_levels)
			{
				splitCodeBook();
				current_levels *= 2;

				//initialise distortions for each splitting
				double prevDistortion = 0.0;
				double currDistortion = 0.0;
				do
				{
					prevDistortion = currDistortion;
					classifyTrainingDataIntoClusters();
			
					currDistortion = calculateOverallDistortion();

					updateCentroid();
			
					cout << "count: "  << count << endl;
					count++;

					distortion_diff = currDistortion - prevDistortion;
					distortion_diff = distortion_diff>0 ? distortion_diff: (-1 * distortion_diff);
					cout << endl;
				} while( distortion_diff > THRESHOLD);
			}
			cout << "Final Centroids" << endl;
			printCentroids();
		}

		// initialise the general properties required to generate code book
		void initialise_properties()
		{
			try
			{
				map<string,string> prop_map;
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
				TRAINING_FILE_PATH = prop_map["training_data_file_path"] ;
				K_levels =  stoi(prop_map["K_levels"]) ;
				THRESHOLD = stof(prop_map["codebook_threshold"]);
				splitting_factor = stof(prop_map["splitting_factor_lbg"]);
				cepstal_file = prop_map["cepstral_file_path"];
				obs_sequence = prop_map["obs_sequence_file_path"];
				codebookFile = prop_map["codebookFilePath"];

				//validate all initialised Properties
				validate();
			}
			catch (std::exception& e)
			{
				cerr << "exception caught in initialising the properties. Cause: " << e.what() << endl;
				throw e;
			}

		}

		void validate()
		{
			if(!utility.validate_powOf2(K_levels))
				exit(1);
			utility.checkFileExistence(TRAINING_FILE_PATH);
		}

		//read the training data from the csv file line by line and save it in vector "trainingData"
		void initialiseTrainingData()
		{
			ifstream file(TRAINING_FILE_PATH.c_str());
			string str;
			char* current;
			while (getline(file,str)) 
				{
					vector<double> newVector;
					char* cstr=const_cast<char*>(str.c_str());
					current = strtok(cstr,",");
					while(current!=NULL)
					{
						double value = stof(current);
						newVector.push_back(value);
						current=strtok(NULL,",");
					}
					trainingData.push_back(newVector);
				}
		}

		//read the training data from the csv file line by line and save it in vector "cepstralData"
		void initialiseCepstralData()
		{
			cepstralData.clear();
			observationSeq.clear();
			ifstream file(cepstal_file.c_str());
			string str;
			char* current;
			while (getline(file,str)) 
				{
					vector<double> newVector;
					char* cstr=const_cast<char*>(str.c_str());
					current = strtok(cstr,",");
					while(current!=NULL)
					{
						double value = stof(current);
						newVector.push_back(value);
						current=strtok(NULL,",");
					}
					cepstralData.push_back(newVector);
				}
		}

		void initialiseSingleVectorCodeBook()
		{
			double temp[12] = {0.0,0.0,0.0,0.0,
							  0.0,0.0,0.0,0.0,
							  0.0,0.0,0.0,0.0};
			for(int i = 0; i < current_levels && trainingData.size() >0 ; i++)//calculate Distance for each centroid in all Levels Present in the codebook
				{
					cbook_entry cb_entry;
					for(int j = 0; j < trainingData.size() ; j++)//calculate Distance for each centroid in all Levels Present in the codebook
					{
						vector<double> current_vector = trainingData[j]; 
						for(int k = 0; k < current_vector.size() ; k++)
						{
							temp[k] += current_vector[k];
						}
					}
					codeBook.push_back(cb_entry);
					for(int j = 0 ; j < trainingData[0].size() ; j++)
					{
						(codeBook[i].centroid).push_back(temp[j]/trainingData.size());
					}
				}
		}

		void splitCodeBook()
		{
			int originalSize = codeBook.size();
			for( int i = 0; i < originalSize ; i++)
			{
				cbook_entry cb_entry;
				for(int j = 0; j < codeBook[i].centroid.size(); j++)
				{
					double val = codeBook[i].centroid[j];
					cb_entry.centroid.push_back( val * (1 + splitting_factor) );
					codeBook[i].centroid[j] = val * (1 - splitting_factor);
				}
				codeBook.push_back(cb_entry);
			}
		}

		//This classifies the training vectors into clusters whose tokuras distance is min of all
		void classifyTrainingDataIntoClusters()
		{
			clearClusters();
			if(codeBook.size() > 0)
				{
					for(int i =0; i < trainingData.size(); i++)// iterating over training vectors
					{
						double minDistance = utility.calculateTokuraDistance(trainingData[i],codeBook[0].centroid);
						int index = 0;
						for(int j = 1; j < current_levels; j++)//calculate Distance for each centroid in all Levels Present in the codebook
						{
							double distance = utility.calculateTokuraDistance(trainingData[i],codeBook[j].centroid);
							if(minDistance > distance)
							{
								minDistance = distance;
								index = j;
							}
						}
						codeBook[index].t_vector.push_back(trainingData[i]);
					}
				}
		}
		
		//calculates centroid of each cluster
		void updateCentroid()
		{
			for(int i = 0; i < current_levels; i++)//calculate Distance for each centroid in all Levels Present in the codebook
				{
					double temp[12] = {0.0,0.0,0.0,0.0,
									  0.0,0.0,0.0,0.0,
									  0.0,0.0,0.0,0.0};
					int clusterSize = codeBook[i].t_vector.size();
					for(int j = 0; j < clusterSize ; j++)//calculate Distance for each centroid in all Levels Present in the codebook
					{
						vector<double> current_vector = codeBook[i].t_vector[j]; 
						for(int k = 0; k < current_vector.size() ; k++)
						{
							temp[k] += current_vector[k];
						}
					}

					for(int j = 0; j< codeBook[i].centroid.size(); j++)
					{
						codeBook[i].centroid[j] = temp[j]/clusterSize;
					}
				}

			printCentroids();
		}

		void printCentroids()
		{
			for(int i =0; i < codeBook.size(); i++)
			{
				cout<< "Training Data Size: " << codeBook[i].t_vector.size() << ", Level: "<< i+1 << endl;
				int j = 0;
				for(j = 0; j < codeBook[i].centroid.size() - 1; j++)
				{
					cout<< codeBook[i].centroid[j] << ",  ";
				}
				cout<< codeBook[i].centroid[j]<< endl;
			}
		}

		//Calculates distortion after classifying and centroid calculation
		double calculateOverallDistortion()
		{
			double temp[12] = {0.0,0.0,0.0,0.0,
							  0.0,0.0,0.0,0.0,
							  0.0,0.0,0.0,0.0};
			double distortion = 0.0;
			for(int i = 0; i < current_levels; i++) //calculate Distance for each centroid in all Levels Present in the codebook
				{
					for(int j = 0 ; j < codeBook[i].t_vector.size() ; j++)
					{
						distortion += utility.calculateTokuraDistance(codeBook[i].t_vector[j] , codeBook[i].centroid);
					}
					//cout << " Distortion for current level "<< i << " is : " << distortion << endl;
				}
			distortion = distortion / trainingData.size();

			cout << "Distortion: "<< distortion << endl;
			return distortion;
		}

		void clearClusters()
		{
			for(int i = 0; i < current_levels; i++) //calculate Distance for each centroid in all Levels Present in the codebook
				{
					codeBook[i].t_vector.clear();	
				}
		}

		
		void writeObservationSequenceIntoFile()
		{
			ofstream outputfile(obs_sequence);
			try
			{
				for(int i=0; i < observationSeq.size(); i++)
				{
					outputfile << observationSeq[i] << "\n";
				}
				outputfile.close();
			}
			catch (std::exception& e)
			{
				cerr << "Sorry !! Unable to create separate observation Sequence file!!! " << endl;
				outputfile.close();
				throw e;
			}
		}
};
#endif