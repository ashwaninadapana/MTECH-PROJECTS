// SpeechBasedPlaylist.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include "string"
#include "fstream"
#include "vector"
#include "sstream" 
#include "cmath"
#include "map"
#include <cstdlib>
#include "Utility.h"
#include "LBG.h"
#include "HMM.h"
#include "ModelCreation.h"
#include "LPCProcessor.h"
#include "RecordSplitter.h"

#define STATES 5
#define OBSERVATIONS 8
using namespace std;

map<string,string> prop_map;
map<string, HMM> hmm_templates;
map<string, string> templatePaths;
map<string, string> playListPaths;
int model_spaces = 0;
string models;
string songplaylistpath;
void parseTemplateFilePaths(string , string , bool );

void loadHMMTemplate(string key,string filename)
{
	utility.checkFileExistence(filename);
	char *current;
	ifstream file(filename.c_str());
	HMM hmm;
	string str;
	try
	{
		int i = 0, j = 0, count = 0;
		while (i < STATES && getline(file, str,'\n'))
		{
			j = 0;
			char* cstr=const_cast<char*>(str.c_str());
			current=strtok(cstr,",");
			while(current!=NULL)
			{
				long double value = stold(current);
				hmm.A[i][j] = value;
				current=strtok(NULL,",");
				j++;
			}
			i++;
		}
		
		while(count < model_spaces)
		{
			getline(file, str,'\n');
			count++;
		}

		i = 0, j = 0;
		while (i < STATES && getline(file, str,'\n'))
		{
			j = 0;
			char* cstr=const_cast<char*>(str.c_str());
			current=strtok(cstr,",");
			while(current!=NULL)
			{
				long double value = stold(current);
				hmm.B[i][j] = value;
				current=strtok(NULL,",");
				j++;
			}
			i++;
		}

		count = 0;
		while(count < model_spaces)
		{
			getline(file, str,'\n');
			count++;
		}

		i = 0;
		while (getline(file, str,'\n'))
		{
			char* cstr=const_cast<char*>(str.c_str());
			current=strtok(cstr,",");
			while(current!=NULL)
			{
				long double value = stold(current);
				hmm.pi[i] = value;
				current=strtok(NULL,",");
				i++;
			}
		}
		hmm_templates.insert(pair<string, HMM>(key,hmm));
		file.close();
	}
	catch (std::exception& e)
	{
		cerr << "exception caught in loading Template: " <<filename << ". Cause: " << e.what() << endl;
		throw e;
	}
}


void parseTemplateFilePaths(string template_file_paths, string seperator, bool isPlayListPath)
{
	char* cstr=const_cast<char*>(template_file_paths.c_str());
	char* current;
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
			if(isPlayListPath)
			{
				playListPaths.insert(pair<string, string>(key,value));
			}
			else
			{
				templatePaths.insert(pair<string, string>(key,value));
			}
			cout << current << endl;
			current=strtok(NULL,seperator.c_str());
		}
	}
	catch (std::exception& e)
	{
		cerr << "exception caught: " << e.what() << endl;
		throw e;
	}
}

void processTemplates(string template_file_paths)
{
    parseTemplateFilePaths(template_file_paths, ";", false);
	map<string, string> :: iterator itr;
	for(itr = templatePaths.begin(); itr != templatePaths.end(); itr++)
	{
		cout << itr->first << " => " << itr->second << '\n';
		loadHMMTemplate(itr->first, itr->second);
	}
}

void initialiseModels()
{
	processTemplates(models);
}

void initialiseProperties()
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
		    models = prop_map["models"];
			model_spaces = stoi(prop_map["model_spaces"]);
			songplaylistpath = prop_map["songplaylistpath"];
			parseTemplateFilePaths(songplaylistpath,";", true);
		}
		catch (std::exception& e)
		{
			cerr << "exception caught in initialising the properties. Cause: " << e.what() << endl;
			throw e;
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
	
void openFolder(string spokenWord)
{
	char explorer[100];
	strcpy(explorer, "start explorer ");
	const char* path = playListPaths[spokenWord].c_str() ;
	string command = strcat( explorer,path);
	system(command.c_str());
}

int _tmain(int argc, _TCHAR* argv[])
{
	
	//RecordSplitter recordSplitter;
	//ModelCreation modelCreation;
	
	initialiseProperties();
	
	LBG lbg;
	lbg.initialiseCodeBook();
	initialiseModels();
	while(1)
	{
		cout << "Enter any character to start the recording again " << endl;
		utility.callRecordingModule();
		LPCProcessor* lpcProcessor = new LPCProcessor(utility.record_FilePath);
		lbg.createObservationSequence();
		
		map<string, HMM> :: iterator itr;
		long double probability  = 0.0 ;
		string word = "DOESN'T MATCH";
		for(itr = hmm_templates.begin() ; itr != hmm_templates.end(); itr++)
		{
			HMM hmm = itr->second;
			hmm.HMMalphaBetaGammaObsSeqInitialiser();
			long double curr_probability = hmm.getProbability();
			cout << itr->first << "  " << curr_probability << endl;
			if(curr_probability > probability)
			{
				probability = curr_probability;
				word = itr->first;
			}
		}
		
		openFolder(word);
		cout << "RESULT : " << word << endl;
		int a;
		scanf("%d", &a);
	} 
	return 0;
}
