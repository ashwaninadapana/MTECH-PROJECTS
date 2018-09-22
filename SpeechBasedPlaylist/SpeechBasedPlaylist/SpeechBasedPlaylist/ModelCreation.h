// 174101006_HMM.cpp : Defines the entry point for the console application.
//
#ifndef ModelCreation_H
#define ModelCreation_H
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
#include "HMM.h"
#include "LPCProcessor.h"

using namespace std;


class ModelCreation
{
	public:
		map<string,string> prop_map;
		string model_file_path;
		string obs_seq_files;
		Utility utility;
		long double A[STATES][STATES], PI[STATES];
		long double B[STATES][OBSERVATIONS];
		int model_spaces;
		vector<string> modelFiles;
		LBG lbg;//create codebook

		ModelCreation()
		{					 
			model_spaces = 0;
			initialiseABPI();
			lbg.initialiseCodeBook();
			initialise_properties();
			initialise();
		}

		~ModelCreation()
		{
		}
		
		void initialise()
		{
			string delim = ";";
			try
			{
				modelFiles = parseTemplate(obs_seq_files, delim);
				createModel(true, false);//initial Model
				createModel(false, false);
				createModel(false, true);
			}
			catch (std::exception& e)
			{
				cerr << "exception caught: " << e.what() << endl;
				throw e;
			}
		}

	private:

		void initialiseABPI()
		{
			for(int i = 0; i < STATES; i++)
			{
				for(int j = 0; j < STATES; j++)
				{
					A[i][j] = 0;
				}
			}
			for(int i = 0; i < STATES; i++)
			{
				for(int j = 0; j < OBSERVATIONS; j++)
				{
					B[i][j] = 0;
				}
			}
			for(int j = 0; j < STATES; j++)
			{
				PI[j] = 0;
			}
		}

		void createModel(bool useBakis, bool writeToFile)
		{
			long double middle_A[STATES][STATES],middle_B[STATES][OBSERVATIONS],middle_PI[STATES];
			if(!useBakis)
			{
				for(int i = 0; i < STATES; i++)
				{
					for(int j = 0; j < STATES; j++)
					{
						middle_A[i][j] = A[i][j];
					}
				}
				for(int i = 0; i < STATES; i++)
				{
					for(int j = 0; j < OBSERVATIONS; j++)
					{
						middle_B[i][j] = B[i][j];
					}
				}
				for(int j = 0; j < STATES; j++)
				{
					middle_PI[j] = PI[j];
				}
				initialiseABPI();
			}
			int count = 0;
			for(int i = 0; i < modelFiles.size(); i++)
				{
					LPCProcessor* lpcProcessor = new LPCProcessor(modelFiles[i]);
					lbg.createObservationSequence();
					HMM hmm;
					hmm.HMMalphaBetaGammaObsSeqInitialiser();
					if(useBakis)
					{
						hmm.initialiseBakisModel();
					}
					else
					{
						hmm.initialiseModel(middle_A,middle_B,middle_PI);
					}
					hmm.initialiseHMM();
					summationModels(hmm);
					count++;
				}
			averageModels(count);
			
			if(writeToFile)
			{
				writeABPImatrix();
			}
		}

		vector<string> parseTemplate(string template_file_paths, string seperator)
		{
			char* cstr=const_cast<char*>(template_file_paths.c_str());
			char* current;
			vector<string> templatePaths;
			cout << "Templates: " << endl;
			try
			{
				current=strtok(cstr,seperator.c_str());
				while(current!=NULL)
				{
					templatePaths.push_back(current);
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

		void summationModels(HMM hmm)
		{
			for(int i = 0; i < STATES; i++)
			{
				for(int j = 0; j < STATES; j++)
				{
					A[i][j] += hmm.A[i][j];
				}
			}

			for(int i = 0; i < STATES; i++)
			{
				for(int j = 0; j < OBSERVATIONS; j++)
				{
					B[i][j] += hmm.B[i][j];
				}
			}

			for(int i = 0; i < STATES; i++)
			{
				PI[i] += hmm.pi[i];
			}
		}

		void averageModels(int count)
		{
			for(int i = 0; i < STATES; i++)
			{
				for(int j = 0; j < STATES; j++)
				{
					A[i][j] = A[i][j]/count;
				}
			}

			for(int i = 0; i < STATES; i++)
			{
				for(int j = 0; j < OBSERVATIONS; j++)
				{
					B[i][j] = B[i][j]/count;
				}
			}

			for(int i = 0; i < STATES; i++)
			{
				PI[i] = PI[i]/count;
			}
		}

		
	void writeABPImatrix()
	{
			FILE * outputfile;
			outputfile = fopen (model_file_path.c_str(),"w");
			try
			{
			   	//A matrix
				for(int i=0; i < STATES; i++)
					{
						int j=0;
						for(; j < STATES-1; j++)
						{
							fprintf (outputfile, "%30.20lf,",A[i][j]);
						}
						fprintf (outputfile, "%30.20lf\n",A[i][j]);
					}
					
					int count = 0;
					while(count < model_spaces)
					{
						fprintf (outputfile, "\n");
						count++;
					}

					//B matrix
					for(int i=0; i < STATES; i++)
					{
						int j=0;
						for(; j < OBSERVATIONS-1; j++)
						{
							fprintf (outputfile, "%30.20lf,",B[i][j]);
						}
						fprintf (outputfile, "%30.20lf\n",B[i][j]);
					}
					count = 0;
					while(count < model_spaces)
					{
						fprintf (outputfile, "\n");
						count++;
					}

					//PI matrix
					int i;
					for(i =0; i < STATES-1; i++)
					{
						fprintf (outputfile, "%30.20lf,",PI[i]);
					}
					fprintf (outputfile, "%30.20lf",PI[i]);
					fclose(outputfile);
			}
			catch (std::exception& e)
			{
				cerr << "Sorry !! Unable to create model file!!! " << endl;
				fclose(outputfile);
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
			    obs_seq_files = prop_map["model_creation_obs_seq_files"];
				model_file_path = prop_map["model_fileName"];
				model_spaces = stoi(prop_map["model_spaces"]);
			}
			catch (std::exception& e)
			{
				cerr << "exception caught in initialising the properties. Cause: " << e.what() << endl;
				throw e;
			}
		}
};
#endif;