#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <string.h>

using namespace std;

string filename = "";
set<string> uniqueEle;
int column = -1;
bool printLogical = false;

//trim the string		
string trim(string s)
{
   size_t p = s.find_first_not_of(" \t");
   s.erase(0, p);

   p = s.find_last_not_of(" \t");
   if (string::npos != p)
      s.erase(p+1);
   return s;
}

vector<string> getSpaceVectorListCommaSeparatedValues(string list)
{
	
	vector<string> ItemVector;
	char* cstr=const_cast<char*>(list.c_str());
	char* current= strtok(cstr," \t");
	while(current!=NULL)
	{
		ItemVector.push_back(trim(current));
		current= strtok(NULL," \t");
	}	
	return ItemVector;
}


int main(int argc, char** argv)
{
	if(argc == 4)
	{	
		filename = argv[1];
		column = atoi(argv[2]);	
		if(column < 0)
		{
			cout << "enter the positive number of the column" << endl;
			exit(0);
		}	
		printLogical = strcmp(argv[3],"true")==0 ? true:false;
	}

	ifstream infile;
	string line;
    	infile.open (filename.c_str(), ios_base::in);
	getline(infile,line); 
	int count = 0;
	cout << line << endl;
    	while(!infile.eof())
        {
		if(!line.empty())
		{
		      	vector<string> itemList = getSpaceVectorListCommaSeparatedValues(line);
			if(itemList.size() >= column)
			{
				uniqueEle.insert(itemList[column-1]);
				cout << count << " : " << itemList[column-1] << endl;
				count++;
			}
		}			
		getline(infile,line); 
        }
	infile.close();
	vector<string>::iterator ip;
	cout << "removing duplicate elements" << endl;
	ofstream ofile;
	ofile.open("uniqueELe.txt");
	count = 0;
	for(string f : uniqueEle)
	{
		 ofile << f;
		 if(printLogical)
		 	ofile << " " << count++ << endl;
		 else
			ofile << endl;
	}    
}


