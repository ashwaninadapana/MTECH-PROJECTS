// vowelCodeBook.cpp : Defines the entry point for the console application.
#ifndef Utility_H
#define Utility_H
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

using namespace std;

class Utility
{
	public:
		int    *tokuraWeights;
		string record_FilePath;
		string wavFilePath;
		string recordingModuleExeFileName;
		int    record_inputDuration;

		Utility()
		{
			tokuraWeights = new int[12];
			tokuraWeights[0] = 1;
			tokuraWeights[1] = 3;
			tokuraWeights[2] =7;
			tokuraWeights[3] =13;
			tokuraWeights[4] =19;
			tokuraWeights[5] =22;
			tokuraWeights[6] =25;
			tokuraWeights[7] =33;
			tokuraWeights[8] =42; 
			tokuraWeights[9] =50; 
			tokuraWeights[10] =56; 
			tokuraWeights[11] =61;
			initialise_properties();
		}

		~Utility()
		{
			if (tokuraWeights != NULL)
				delete [] tokuraWeights;
		}

		bool validate_powOf2(int value)
		{
			int temp = value;
			 while (((temp % 2) == 0) && temp > 1) /* While K_levels is even and > 1 */
			   temp /= 2;
			if (temp != 1) 
			{
				cout << "Sorry !!! Cannot continue as value \"" << value << "\" is not a power of 2." << endl;
				return false;
			}
			return true;
		}

		// Check if the file exists or not
		void checkFileExistence(string filename)
		{
			ifstream file(filename.c_str());
			if (!file) 
			{
				cout << "Sorry !!! Cannot continue as \"" << filename << "\" file does not exist!" << endl;
				exit(1);
			}
			file.close();
		}

		//calculate tokuras distance between 2 vectors( Training vector and centroid)
		double calculateTokuraDistance(vector<double> currentC, vector<double> referenceTemplate)
		{
			double weight = 0.0;
			if((currentC.size() == referenceTemplate.size()))
			{
				for(int i = 0; i < referenceTemplate.size() ; i++)
				{
					weight += pow(currentC[i] - referenceTemplate[i], 2) * tokuraWeights[i];
				}
				return weight;
			}
		}

		void callRecordingModule()
		{
			string duration = to_string(static_cast<long long>(record_inputDuration));
			string command = "\""+recordingModuleExeFileName+ "\"" + " " + duration +" "+ wavFilePath +" "+ record_FilePath;
			cout << command <<endl;
			system(command.c_str());
		}

	private:
	// initialise the general properties required to generate code book
		void initialise_properties()
		{
			try
			{
				map<string,string> prop_map;
				string filename = "Recognition.properties";
				checkFileExistence(filename);
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
				record_FilePath = prop_map["record_file_path"] ;
				wavFilePath = prop_map["wav_file_path"] ;
				record_inputDuration = stoi(prop_map["record_input_duration"]) ;
				recordingModuleExeFileName = prop_map["recording_module_exe_file_name"];
			}
			catch (std::exception& e)
			{
				cerr << "exception caught in initialising the properties. Cause: " << e.what() << endl;
				throw e;
			}

		}
};
#endif