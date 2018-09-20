#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <map>
#include <math.h>
#include <cstdlib>
#include <sstream>
#include <algorithm>

#define SELECT "select"
#define TAB 	"\t"
#define SEMI_COLON ";"
#define COMMA ", "
#define INPUT "input"
#define REG "reg"
#define MODULE "module"
#define ALWAYS "always @"
#define POSEDGE "posedge"
#define BEGIN "begin"
#define END "end"
#define ENDMODULE "endmodule"
#define ASSIGN "assign"
#define IF "if"
#define CASE "case"
#define COLON ":"
#define WIRE "wire"
#define INITIAL "initial"
#define ELSE "else"
#define ENDCASE "endcase"
#define PARAMETER "parameter"
#define OUTPUT "output"
#define DEASSIGN "deassign"
#define C_ADD "ADD" //capital ADD
#define S_ADD "add" //small add
#define C_SUB "SUB"
#define S_SUB "sub"
#define C_MULT "MULT"
#define S_MULT "mult"
#define C_LT  "LT"
#define S_LT  "lt"
#define C_GT  "GT"
#define S_GT  "gt"
#define C_LTE  "LTE"
#define S_LTE  "lte"
#define C_GTE  "GTE"
#define S_GTE  "gte"
#define C_DIV "DIV"
#define S_DIV "div"
#define C_EQ "E"
#define S_EQ "e"
#define C_RS "RS"
#define S_RS "rs"
#define C_LS "LS"
#define S_LS "ls"
#define ADD "+"
#define SUB "-"
#define MULT "*"
#define DIV "/"
#define LT "<"
#define GT ">"
#define LTE "<="
#define GTE ">="
#define LEFT_SHIFT "<<"
#define RIGHT_SHIFT ">>"
#define EQUAL "=="

using namespace std;

//input file name
string inputFileName = "datapath_output.txt";
//all register declarations
string registerFileName = "register.v";
//mux instantiation
string muxFileName = "mux.v";
//fsm module
string fsmFileName = "fsm.v";

//function declarations
void processInputFile();
void generateVerilogFile();
bool parseLine(string line);
void checkIdentifier(string line);
string trim(string s);
void traverseRegisters();
void printMUXList();
void printFSM();
void checkFileExistence(string filename);
bool isParseLineEnabled();
bool isNum(string token);
void generateAdderModule(string module_name, int inputWidth1, int inputWidth2, int outputWidth);//0 - add, 1- sub
vector<int> getIntVectorListCommaSeparatedValues(string list);
void printFUList();
vector<string> getStringVectorListCommaSeparatedValues(string list);
void populateVerilogStateRepresentations();
void postProcess();
void openOfStream();
void printStructures();
void processInputFile();
void generateSingleModule(string module_name, int inputWidth1, int inputWidth2, int outputWidth, string operation);
void populateMuxFuLinkNames();
void populateRegFuLinkNames();
void generateFunctionModule();
void writeStateTofsmFile();
string getBinary(int number, int bits);
void writeFuInputsToFsmFile();
void writeFuOutputsToFsmFile();
void createMux(int no_select_lines);
void closeOfStream();

//identifiers to identify the section in the input.txt file
string reg_identifier = "<Specify Registers>";
string mux_identifier = "<Specify MUX>";
string fu_identifier = "<Specify FUs>";
string controller_fsm_identifier = "<Controller FSM>";
string cs_for_mux_in_order = "<CS for MUX in order>";
string cs_fu_in_order = "<CS for FU in order>";
string control_access_pattern = "<CAP>";
string specify_rt_operations = "<Specify RT operations>";
string port_identifier = "RP";

//boolean variables to identify the section to which the line belongs to
bool isRegReadEnabled = false; 
bool isMuxReadEnabled = false; 
bool isFUReadEnabled = false; 
bool isControllerFSMEnabled = false;
bool isCSMuxEnabled = false;
bool isCSFUEnabled = false;
bool isCapEnabled = false;
bool isRTOperationsEnabled = false;

//variables required for post processing
int mux_count = 0;
int no_states = 0;
int bits_req = 0;

ofstream regfile;
ofstream muxfile;
ofstream fsmfile;

//Functional Unit
struct function_unit
{
	int output_width;
	int inputWidth1;
	int inputWidth2;
	vector<string> operations;
	int bits_selectLines;
	string input1WireName;
	string input2WireName;
	string outputWireName;
	string selectLineName;
};

//MUX
struct mux
{
	string output;
	map<string,string> input_cs;
	int bits_selectLines;
	string selectLineName;
	string outputWireName;
	int outputWireWidth;
	bool isoutputFU;
};

struct rtOperation
{
	string state;
	string operationType;
	
	//operation R  parameters
	string inputPortName;
	string readToReg;
	//operation A parameters
	string inputReg1;
	string inputReg2;
	string outputWidth;
	string inputWidth1;
	string inputWidth2;
	string fu;
	string fuOutputWire;
	//operation O parameters
	string outputPortName;
	string outputReg;
};

//FSM state
struct state
{	
	string binary_rep;
	int no_ways;
	string trueNextState;
	string falseNextState;
	string nextState;
	map<string,string> mux_select;
	map<string,string> reg_select;
	map<string,string> fu_select;
};

//Parses <Specify Rt operations> and save their registers that will be given as input to FU
struct regFuLink
{
	string input1WireName;
	string input2WireName;
};

struct portInfo
{
	int width;
	bool isInput;
};

//data structures required
map<int, int> is_mux_in_file;
map<string,struct state> stateList;
map<string, int> registers;
map<string,struct function_unit> fuList;
map<string,struct mux> muxList;
map<string, struct regFuLink> regFuLinkList;//used for populating the functional unit inputs with reg wires if there is no mux in between
map<string, vector<string> > muxLinks;//store reg and Fu related to MUX
map<string, struct portInfo> portWidth;
vector<struct rtOperation> rtOperationsList;
map<string, string> regWireList;
//As maps dont preserve the order in which they are inserted, we are using vectors to check the order while parsing the control access pattern
vector<string> muxSeqList;
vector<string> stateSeqList;
vector<string> regSeqList;
vector<string> fuSeqList;

int main(int argc, char** argv)
{
   try
   {
   		if(argc == 2)
   		{
   			
   			inputFileName = argv[1];
   			cout << "Input file is :" << inputFileName << endl;
		}	
		checkFileExistence(inputFileName);
		openOfStream();
		cout << "File " << inputFileName << "\" is present. So, now lets parse the file" << endl;
		processInputFile();	
		postProcess();
		
		generateVerilogFile();
		
		//printStructures();
	}
	catch (exception& e)
	{
		closeOfStream();
	    cout << "Exception occured. Cause: " << e.what() << '\n';
	}
	return 1;
}

void checkFileExistence(string filename)
{
	ifstream file(filename.c_str());
	if (!file) 
	{
		cout << "Sorry!!!!!!!! Cannot continue as \"" << filename << "\" does not exist!" << endl;
		exit(1);
	}
	file.close();
}

//After processing the input file
void postProcess()
{
	populateVerilogStateRepresentations();
	populateMuxFuLinkNames();
	populateRegFuLinkNames();
}

//print the datastructures populated
void printStructures()
{
	printMUXList();
	printFUList();
	printFSM();
}

//parses the input file
void processInputFile()
{
	ifstream infile;
	string line;
    	infile.open (inputFileName.c_str(), ios_base::in);
    	while(!infile.eof())
        {
	        getline(infile,line); 
	        checkIdentifier(line); //identify the section to which the line belongs to
	        bool isNotEmpty = true;
	        while(isNotEmpty && (isParseLineEnabled()) && !infile.eof()) //if line is not empty and belongs to the section which requires parsing, parse it
        	{
        			getline(infile,line); 
        			cout << line << endl;
	        		isNotEmpty = parseLine(line);	
			}
        }
	infile.close();
}

//identify the section
void checkIdentifier(string line)
{
	//cout << line << endl;
	if(line.find(reg_identifier) != string::npos )
	{
		cout << "----------------Enabling register read operation---------------" << endl;
		isRegReadEnabled = true; 
		isMuxReadEnabled = false; 
		isFUReadEnabled = false; 
		isControllerFSMEnabled = false;
		isCSMuxEnabled = false;
		isCSFUEnabled = false;
		isCapEnabled = false;
		isRTOperationsEnabled = false;
	}
	else if(line.find(mux_identifier) != string::npos )
	{
		cout << "----------------Enabling MUX read operation---------------" << endl;
		isRegReadEnabled = false; 
		isMuxReadEnabled = true; 
		isFUReadEnabled = false; 
		isControllerFSMEnabled = false;
		isCSMuxEnabled = false;
		isCSFUEnabled = false;
		isCapEnabled = false;
		isRTOperationsEnabled = false;
	}
	else if(line.find(fu_identifier) != string::npos )
	{
		cout << "----------------Enabling FU read operation---------------" << endl;
		isRegReadEnabled = false; 
		isMuxReadEnabled = false; 
		isFUReadEnabled = true; 
		isControllerFSMEnabled = false;
		isCSMuxEnabled = false;
		isCSFUEnabled = false;
		isCapEnabled = false;
		isRTOperationsEnabled = false;
	}
	else if(line.find(controller_fsm_identifier) != string::npos)
	{
		cout << "----------------Enabling FSM read operation---------------" << endl;
		isRegReadEnabled = false; 
		isMuxReadEnabled = false; 
		isFUReadEnabled = false; 
		isControllerFSMEnabled = true;
		isCSMuxEnabled = false;
		isCSFUEnabled = false;
		isCapEnabled = false;
		isRTOperationsEnabled = false;
 	}
	else if(line.find(cs_for_mux_in_order) != string::npos)
	{
		cout << "----------------Enabling CS for Mux read operation---------------" << endl;
		isRegReadEnabled = false; 
		isMuxReadEnabled = false; 
		isFUReadEnabled = false; 
		isControllerFSMEnabled = false;
		isCSMuxEnabled = true;
		isCSFUEnabled = false;
		isCapEnabled = false;
		isRTOperationsEnabled = false;
 	}
	else if(line.find(cs_fu_in_order) != string::npos)
	{
		cout << "----------------Enabling CS for FU read operation---------------" << endl;
		isRegReadEnabled = false; 
		isMuxReadEnabled = false; 
		isFUReadEnabled = false; 
		isControllerFSMEnabled = false;
		isCSMuxEnabled = false;
		isCSFUEnabled = true;
		isCapEnabled = false;	
		isRTOperationsEnabled = false;
	}
	else if(line.find(control_access_pattern) != string::npos)
	{
		cout << "----------------Enabling control access pattern read operation---------------" << endl;
		isRegReadEnabled = false; 
		isMuxReadEnabled = false; 
		isFUReadEnabled = false; 
		isControllerFSMEnabled = false;
		isCSMuxEnabled = false;
		isCSFUEnabled = false;
		isCapEnabled = true;	
		isRTOperationsEnabled = false;
	}
	else if(line.find(specify_rt_operations) != string::npos)
	{
		cout << "----------------Enabling RT read operation---------------" << endl;
		isRegReadEnabled = false; 
		isMuxReadEnabled = false; 
		isFUReadEnabled = false; 
		isControllerFSMEnabled = false;
		isCSMuxEnabled = false;
		isCSFUEnabled = false;
		isCapEnabled = false;	
		isRTOperationsEnabled = true;
	}
	else //does not belong to any section, so make all the flags false
	{
	     isFUReadEnabled = isMuxReadEnabled = isRegReadEnabled = 
	     isControllerFSMEnabled = isCSMuxEnabled = isCSFUEnabled= isCapEnabled =  isRTOperationsEnabled =false;	
	}
}

//If the line has to be parsed, then return true
bool isParseLineEnabled()
{
	return(isRegReadEnabled || 
	  		 isMuxReadEnabled || 
	   			isFUReadEnabled ||
				   isControllerFSMEnabled||
						isCSMuxEnabled ||
							isCSFUEnabled ||
							   isCapEnabled ||
							   		isRTOperationsEnabled);
}

//Traverse the registers and print its width
void traverseRegisters()
{
	map<string, int> :: iterator itr = registers.begin();
	for(itr = registers.begin(); itr != registers.end(); itr++)
	{
		cout << itr->first << " => " << itr->second << '\n';
	}
}

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

//converts number to string
string NumberToString(int number)
{
     ostringstream ss;
     ss << number;
     return ss.str();
}

//parses Line into fields like Mux_name, list of inputs, output of mux, select lines
void populateMux(string line)
{
			vector<string> regFuList;
			struct mux newMux;
			mux_count++;
			int blockNumber = 4;//no of <> blocks
			int begin_index = -1, end_index = 0;
			int i = 0;//Counter for <> blocks we have encountered.
			vector<string> ItemVector;//strings enclosed in <> are placed in this vector
			while( i < blockNumber )
			{
			    begin_index = line.find_first_of('<', begin_index + 1);
			    end_index = line.find_first_of('>', end_index + 1);
			    ItemVector.push_back(trim(line.substr(begin_index + 1, end_index - begin_index -1)));
			    i++;
			}
			//creating new object and inserting into the muxList
			string muxName = ItemVector[0];
			newMux.selectLineName = SELECT + NumberToString(mux_count);
			vector<string> inputs = getStringVectorListCommaSeparatedValues(ItemVector[1]);
			regFuList.insert(regFuList.end(), inputs.begin(),inputs.end());
			newMux.output = trim(ItemVector[2]);
			regFuList.push_back(newMux.output);
			vector<string> selectLines = getStringVectorListCommaSeparatedValues(ItemVector[3]);
			for(int i = 0; i < inputs.size(); i++)
			{
				newMux.input_cs.insert(pair<string,string>(selectLines[i], inputs[i]));
			}
			muxList.insert(pair<string,struct mux>(muxName,newMux));
			muxSeqList.push_back(muxName);
			muxLinks.insert(pair<string, vector<string> >(muxName, regFuList));
}

/*PATTERN:
	<Specify FUs> 
	ADDER1, (ADD, SUB), (48, 32,24)
*/
void populateFu(string line)
{
		struct function_unit fu;
		string fu_name = trim(line.substr(0,line.find_first_of(',', 0)));
		int blockNumber = 2;//no of () blocks
		int begin_index = -1, end_index = 0;
		int i = 0;//Counter for () blocks we have encountered.
		vector<string> ItemVector;//string enclosed in () brackets are placed in this vector
		while( i < blockNumber )
		{
		    begin_index = line.find_first_of('(', begin_index + 1);
		    end_index = line.find_first_of(')', end_index + 1);
		    ItemVector.push_back(trim(line.substr(begin_index + 1, end_index - begin_index -1)));
		    i++;
		}
		//creating new object and inserting into the fuList
		fu.operations = getStringVectorListCommaSeparatedValues(ItemVector[0]);
		vector<int> widthVector = getIntVectorListCommaSeparatedValues(ItemVector[1]);
		fu.output_width = widthVector[0];
		fu.inputWidth1 = widthVector[1];
		fu.inputWidth2 = widthVector[2];
		fu.outputWireName = fu_name + "_wire";
		fuList.insert(pair<string, struct function_unit>(fu_name,fu));
		fuSeqList.push_back(fu_name);
}

//populating the registers map with its name and width
void populateRegisters(string line)
{
	char* const_line = const_cast<char*>(line.c_str());
	string register_name = trim(strtok(const_line, ","));
	string width = "";
	if (!register_name.empty())
	{
		width = trim(strtok(NULL, " "));
		registers.insert(pair<string, int>(register_name,atoi(width.c_str())));
		regSeqList.push_back(register_name);
	}
}

//returns true if the token is T or t
bool isTrueState(string token)
{
	if(strcmp(token.c_str(), "T") == 0 || strcmp(token.c_str(), "t") == 0)
	{
		return true;
	}
	return false;
}

/* PATTERN:
	<Controller FSM>
	B11 1 B12
	C1 2 T B21 F B31
	B32 0
*/
void populatefSM(string line)
{
	no_states++;
	string dup_line = line;
	vector<string> ItemVector;/*Meaning of each entry in the vector.	
					index: what it is?
					0: statename, 1: no of ways 
					no of ways = 1 -> 2:next State
					no of ways = 2 -> (2: T, 4: F) or (2: F, 4: T) 
					(2: T, 4: F) -> 3: truestate , 5: falseState
					(2: F, 4: T) -> 3: falseState, 5: trueState
				   */
	char* cstr=const_cast<char*>(line.c_str());
	char* current= strtok(cstr," ");
	while(current!=NULL)
	{
		ItemVector.push_back(trim(current));
		current= strtok(NULL," ");
	}	
	
	
	struct state newState;
	string state_name = trim(ItemVector[0]);
	if(isNum(ItemVector[1]))
	{
		newState.no_ways = atoi(ItemVector[1].c_str());
	}
	else
	{
		cout  << "No of ways in the FSM is not a number" << endl;
		exit(0);
	}
	if(newState.no_ways >= 0)
	{
		if(newState.no_ways == 1 && ItemVector.size() == 3) //----> state 1 nextState
		{
			newState.nextState = trim(ItemVector[2]);
		}
		else if(newState.no_ways == 2 && ItemVector.size() == 6)
		{
			if(isTrueState(ItemVector[2]) && !isTrueState(ItemVector[4]))//----> state 2 T trueState F falseState
			{
				newState.trueNextState = ItemVector[3];
				newState.falseNextState = ItemVector[5];
			}
			else if(!isTrueState(ItemVector[2]) && isTrueState(ItemVector[4])) //----> state 2 F falseState T trueState
			{
				newState.falseNextState = ItemVector[3];
				newState.trueNextState = ItemVector[5];
			}
			else
			{
				cout << "Unknown Pattern in FSM: " << dup_line << endl;
				exit(0);
			}
			
		}
		else  if(newState.no_ways == 0)//State 0. this is final state as there are no outgoing edges
		{
			cout << "Final State line: " << dup_line << endl;
		}
		else
		{
			cout << "Invalid number of fields in the line" << dup_line << endl;
			exit(0);
		}
	}		
	else
	{
		cout << "Invalid no of ways in FSM line: " << dup_line << endl;
		exit(0);		
	}
	stateList.insert(pair<string, struct state>(state_name,newState));
	stateSeqList.push_back(state_name);
}

//populates no of select lines required for selecting the operation of this Functional unit
void populateBitsInFU(string line)
{
	//PATTERN: (ADDER, 1), (MULT1, 0), (MULT2, 0), (COMP1,0)
   int i = 0;
   line = trim(line);
   map<string,struct function_unit>::iterator it = fuList.begin();
   bool isMatchFound = false;
   int begin_index = -1, end_index = 0;
   int count = 0;
   
   while(end_index < strlen(line.c_str())-1) 
   {
	isMatchFound = false;
	begin_index = line.find_first_of('(', begin_index + 1);
	end_index = line.find_first_of(')', end_index + 1);
	string sgment = line.substr(begin_index + 1, end_index - begin_index - 1);
	char* cstr=const_cast<char*>(sgment.c_str());
	string fuName= trim(strtok(cstr,","));
	//search the fuList with this fuName and populate "bits_selectLines" field in the struct function_unit
	if(!fuName.empty())
	{
		string bits = trim(strtok(NULL,","));
		if(!bits.empty() && isNum(bits))
		{
			int no_bits_fu = atoi(bits.c_str());
			if(fuList.find(fuName) != fuList.end())
			{
				fuList[fuName].bits_selectLines = no_bits_fu;
				if(no_bits_fu > 0)
				{
					fuList[fuName].selectLineName = "fu" + NumberToString(count++)+"_select";
				}
			}
			else
			{
				cout << "FU Name " << fuName << " is not found in <Specify FU> section" << endl;
				exit(0);
			}
		}
		else
		{
			cout << "no of bits for " << fuName << " is not an integer" << endl;
		}
		
   	}
   }
}

//If the mux dont have 2^n registers, fill the remaining with impedence
void populateImpedenceIfEmpty(string muxName)
{
	if(muxList.find(muxName) != muxList.end())
	{
		int bits = muxList[muxName].bits_selectLines;
		map<string, string> input_cs_map = muxList[muxName].input_cs;
		int max = pow(2, bits);
		for(int i = 0; i < max; i++)
		{
			string binary = getBinary(i, bits);
			//cout << "binary: " << max << endl;
			if(input_cs_map.find(binary) == input_cs_map.end())
			{
				//cout << "High impedence in " << binary << endl;
				muxList[muxName].input_cs.insert(pair<string,string>(binary,"1'bz"));
			}
		}
	}
}

//populate no of select lines required to identify which input is selected
void populateBitsInMux(string line)
{
   //PATTERN: (M1, 2), (M2, 1), (M3, 1), (M4, 1), (M5, 2), (M6, 2), (M7, 1)
   int i = 0;
   line = trim(line);
   map<string,struct mux>::iterator it = muxList.begin();
   bool isMatchFound = false;
   while(i < strlen(line.c_str())-1) 
   {
	isMatchFound = false;
	while(line[i] != '(')
	{
		i++;
	}	
	int begin_index = ++i;
	while(line[i] != ')')
	{
		i++;
	}
	int end_index = i;
	string sgment = line.substr(begin_index, end_index - begin_index);
	char* cstr=const_cast<char*>(sgment.c_str());
	string muxName= trim(strtok(cstr,","));
	if(!muxName.empty())
	{
		string bits = trim(strtok(NULL,","));
		if(!bits.empty() && isNum(bits))
		{
			int no_bits_mux = atoi(bits.c_str());
			if(muxList.find(muxName) != muxList.end())
			{
				muxList[muxName].bits_selectLines = no_bits_mux;
				cout << "Creating mux" << endl;
				if(is_mux_in_file.find(no_bits_mux) == is_mux_in_file.end())
				{
					cout << "Created mux" << endl;
					is_mux_in_file.insert(pair<int,int>(no_bits_mux, 1));
					createMux(no_bits_mux);
				}
				populateImpedenceIfEmpty(muxName);
			}
			else
			{
				cout << "MUX Name " << muxName << " is not found in <Specify MUX> section" << endl;
				exit(0);
			}
		}
		else
		{
			cout << "no of bits for " << muxName << " is not an integer" << endl;
		}
		
   	}
   }
}

//parses the control access pattern of each block and stores their value in a map <MUX select line name, value>
map<string,string> parseBlockMuxSelectLines(string line)
{
	map<string, string> obj;
	int i = 0;
	for(int j = 0; j < muxSeqList.size(); j++)
	{
		struct mux temp = muxList[muxSeqList[j]];
		string token = NumberToString(temp.bits_selectLines)+"'b"+line.substr(i, temp.bits_selectLines);
		i += temp.bits_selectLines;
		obj.insert(pair<string,string>(temp.selectLineName,token));
	}
	return obj;
}


//parses the control access pattern of each block to check which reg is enabled to write
map<string,string>  parseBlockRegSelectLines(string line)
{
	map<string,string> obj;
	int i = 0;
	for(int j = 0; j < regSeqList.size(); j++)
	{
		string token = line.substr(i, 1);
		i++;
		obj.insert(pair<string,string>(regSeqList[j],token));
	}
	return obj;
}
 

//parses the control access pattern of each block and stores their value in a map <FU select line name, value>
map<string,string> parseBlockFUSelectLines(string line)
{
	map<string,string> obj;
	int i = 0;
	
	for(int j = 0; j < fuSeqList.size(); j++)
	{
		struct function_unit temp = fuList[fuSeqList[j]];
		if(temp.bits_selectLines > 0)
		{
			string token = NumberToString(temp.bits_selectLines) + "'b"+ line.substr(i, temp.bits_selectLines);
			i += 	temp.bits_selectLines;		
			obj.insert(pair<string,string>(temp.selectLineName,token));
		}
	}
	return obj;
}

/*
<CAP>
	B11: (0000000000)(001100000)(0)
*/
void populateCAP(string line)
{	
	string stateName, pattern;
	int blockNumber = 3;//no of () blocks
	int begin_index = -1, end_index = 0;
	int i = 0;//Counter for () blocks we have encountered.
	vector<string> ItemVector;
	map<string,struct state>::iterator it = stateList.begin();
	bool isMatchFound = false;
	char* cstr=const_cast<char*>(line.c_str());
	stateName= strtok(cstr,":");
	if(!stateName.empty())
	{
		pattern= trim(strtok(NULL," "));		
	}

	//stores the strings enclosed in () brackets in ITEM VECTOR
	while( i < blockNumber )
	{
	    begin_index = pattern.find_first_of('(', begin_index + 1);
	    end_index = pattern.find_first_of(')', end_index + 1);
	    ItemVector.push_back(trim(pattern.substr(begin_index + 1, end_index - begin_index -1)));
	    i++;
	}
	for (it=stateList.begin(); it!=stateList.end(); ++it)
	{
		if(strcmp((it->first).c_str(), stateName.c_str()) == 0)
		{		
			isMatchFound = true;
			(it->second).mux_select = parseBlockMuxSelectLines(ItemVector[0]);
			(it->second).reg_select = parseBlockRegSelectLines(ItemVector[1]);
			(it->second).fu_select = parseBlockFUSelectLines(ItemVector[2]);
			break;
		}		
	}
	if(!isMatchFound)
	{
		cout << "State Name " << stateName << " is not found in <Control FSM> section" << endl;
		exit(0); 
	}
	
}

//insert or update port with max width
void insertPort(string port, int width, bool isInput)
{
	if(portWidth.find(port) != portWidth.end())
	{
		struct portInfo temp = portWidth[port];
		if(temp.width < width)//check for max width
		{
			portWidth[port].width = width;
		}
	}
	else
	{
		struct portInfo temp = {width, isInput};
		portWidth.insert(pair<string, struct portInfo>(port, temp));
	}
}

//C1, 1, A, (R9, 1),  (R3 , 17),  (COMP1, LT), (R5, 16)
void functionUnitRegLinker(string line)
{
	char* cstr=const_cast<char*>(line.c_str());
	string blockName = trim(strtok(cstr,","));
	string timestep = trim(strtok(NULL,","));
	string state = blockName + timestep;
	
	string operation = trim(strtok(NULL,","));
	int i = 0;
	int begin_index = -1, end_index = 0;
	vector<string> ItemVector;
	struct rtOperation rtOperObj;	
	if(strcmp(operation.c_str(), "A") == 0 || strcmp(operation.c_str(), "a") == 0)
	{
		int blockNumber = 4;//no of () blocks
		//stores the strings enclosed in () brackets in ITEM VECTOR
		while( i < blockNumber )
		{
		    begin_index = line.find_first_of('(', begin_index + 1);
		    end_index = line.find_first_of(')', end_index + 1);
		    ItemVector.push_back(trim(line.substr(begin_index + 1, end_index - begin_index -1)));
		    i++;
		}
		vector<string> outputVector = getStringVectorListCommaSeparatedValues(ItemVector[0]);
		vector<string> reg1Vector = getStringVectorListCommaSeparatedValues(ItemVector[1]);
		vector<string> fuOperationsVector = getStringVectorListCommaSeparatedValues(ItemVector[2]);
		vector<string> reg2Vector = getStringVectorListCommaSeparatedValues(ItemVector[3]);
		struct regFuLink temp;
		temp.input1WireName = reg1Vector[0];
		temp.input2WireName = reg2Vector[0];
		regFuLinkList.insert(pair<string, struct regFuLink>(fuOperationsVector[0],temp));
		rtOperObj.fu = fuOperationsVector[0];
		rtOperObj.fuOutputWire = fuList[fuOperationsVector[0]].outputWireName;
		rtOperObj.inputReg1 = reg1Vector[0];
		rtOperObj.inputReg2 = reg2Vector[0];
		rtOperObj.inputWidth1 = atoi(reg1Vector[1].c_str());
		rtOperObj.inputWidth2 = atoi(reg2Vector[1].c_str());
		rtOperObj.operationType = operation;
		rtOperObj.outputReg = outputVector[0];
		rtOperObj.outputWidth = atoi(outputVector[1].c_str());
		rtOperObj.state = state;
	}
	else if(strcmp(operation.c_str(), "R") == 0 || strcmp(operation.c_str(), "r") == 0)
	{
		int blockNumber = 1;//no of () blocks
		//stores the strings enclosed in () brackets in ITEM VECTOR
		while( i < blockNumber )
		{
		    begin_index = line.find_first_of('(', begin_index + 1);
		    end_index = line.find_first_of(')', end_index + 1);
		    ItemVector.push_back(trim(line.substr(begin_index + 1, end_index - begin_index -1)));
		    i++;
		}
		vector<string> items = getStringVectorListCommaSeparatedValues(ItemVector[0]);
		string portName = items[0];
		string readToreg = items[1];
		if( strncmp(portName.c_str(), port_identifier.c_str(), strlen(port_identifier.c_str())) == 0)
		{
			int portWidth = atoi(items[2].c_str());
			insertPort(portName, portWidth, true);
		}
			rtOperObj.state = state;
			rtOperObj.operationType = operation;
			rtOperObj.inputPortName = portName;
			rtOperObj.readToReg = readToreg;
	}
	else if(strcmp(operation.c_str(), "O") == 0 || strcmp(operation.c_str(), "o") == 0)
	{
		int blockNumber = 1;//no of () blocks
		//stores the strings enclosed in () brackets in ITEM VECTOR
		while( i < blockNumber )
		{
		    begin_index = line.find_first_of('(', begin_index + 1);
		    end_index = line.find_first_of(')', end_index + 1);
		    ItemVector.push_back(trim(line.substr(begin_index + 1, end_index - begin_index -1)));
		    i++;
		}
		vector<string> items = getStringVectorListCommaSeparatedValues(ItemVector[0]);
		string portName = items[0];
		string writeToreg = items[1];
		int portWidth = atoi(items[2].c_str());
		insertPort(portName, portWidth, false);
		
		
		rtOperObj.state = state;
		rtOperObj.operationType = operation;
		rtOperObj.outputPortName = portName;
		rtOperObj.outputReg = writeToreg;
	}
	rtOperationsList.push_back(rtOperObj);
}

//returns false if line is empty
bool parseLine(string line)
{
	line = trim(line);
	if(strlen(line.c_str()) != 0)
	{
		if(isRegReadEnabled)
		{
			populateRegisters(line);
		}
		else if(isMuxReadEnabled)
		{
			populateMux(line);
		}
		else if(isFUReadEnabled)
		{
			populateFu(line);
		}
		else if(isControllerFSMEnabled)
		{
			populatefSM(line);
		}
		else if(isCSMuxEnabled)
		{
			populateBitsInMux(line);
		}
		else if(isCSFUEnabled)
		{
			populateBitsInFU(line);
		}
		else if(isCapEnabled)
		{
			populateCAP(line);
		}
		else if(isRTOperationsEnabled)
		{
		     functionUnitRegLinker(line);
		}
		return true;
	}	
	else
	{
		return false;
	}
}

//opens the files to write
void openOfStream()
{
	fsmfile.open(fsmFileName.c_str());
}

//closes all the file streams
void closeOfStream()
{	
	fsmfile.close();
}

//writes mux select variables with their width
void writeSelectToFsmFile()
{
	map<string,struct mux>::iterator it = muxList.begin();
	for (it=muxList.begin(); it!=muxList.end(); ++it)
	{
		struct mux temp = it->second;
		fsmfile << TAB << REG << "[" << temp.bits_selectLines - 1 << ":0] " << temp.selectLineName << SEMI_COLON << endl; 
	}
	map<string,struct function_unit>::iterator itr = fuList.begin();
	for (itr=fuList.begin(); itr!=fuList.end(); ++itr)
	{
		struct function_unit temp = itr->second;
		if(temp.bits_selectLines > 0)
			fsmfile << TAB << REG << "[" << temp.bits_selectLines - 1 << ":0] " << temp.selectLineName << SEMI_COLON << endl; 
	}
	fsmfile << endl;
}

//writes reg variables with their width
void writeRegtoFSMFile()
{
	for(int i = 0; i < regSeqList.size(); i++)
	{
		fsmfile << TAB << REG <<"[" << registers[regSeqList[i]] - 1 <<":0] " << regSeqList[i] << SEMI_COLON  << endl ;
	}
	fsmfile << endl << endl;
}

void writePortsToFSMFile()
{
	map<string, struct portInfo>:: iterator itr = portWidth.begin();
	for(; itr != portWidth.end(); itr++)
	{
		struct portInfo temp = itr->second;
		if(temp.isInput)
			fsmfile << TAB << INPUT <<"[" << temp.width - 1 <<":0] " << itr->first << SEMI_COLON  << endl ;
		else
			fsmfile << TAB << OUTPUT << " " << REG <<"[" << temp.width - 1 <<":0] " << itr->first << SEMI_COLON  << endl ;
	}
	fsmfile << endl << endl;	
}
//creating objects of MUX
void instantiateMux()
{
	cout << "---------------------------------MUX Instantiation------------------------------------------------" << endl;
	fsmfile << TAB << "//MUX instantiation" << endl;
	map<string,struct mux>::iterator it = muxList.begin();
	for (it=muxList.begin(); it!=muxList.end(); ++it)
	{
		struct mux temp = it->second;
		int no_inputs = pow(2, temp.bits_selectLines);
		fsmfile << TAB << "mux_" << no_inputs << "x1 " << it->first << "(";
		int count = 0;
		while(count < no_inputs)
		{
			string binary = getBinary(count, temp.bits_selectLines);
			fsmfile << temp.input_cs[binary] << COMMA << " ";
			count++;		
		}
		fsmfile << temp.selectLineName << COMMA << temp.outputWireName << ")" << SEMI_COLON << endl;
	}
	
}

void createTabSpace(int tabCount)
{
	for(int i = 0; i < tabCount ; i++)
	{
			fsmfile << TAB;
	}
}

//creates objects of FU
void instantiateFU()
{
	cout << "---------------------------------FU Instantiation------------------------------------------------" << endl;
	fsmfile << TAB << "//FU instantiation" << endl;
	map<string,struct function_unit>::iterator it = fuList.begin();
	int count = 0;
	for (it=fuList.begin(); it!=fuList.end(); ++it)
	{
		string name = "fu" + NumberToString(count);
		count++;
		struct function_unit temp = it->second;
		fsmfile << TAB << it->first << " " << name << "(" << temp.outputWireName << ", " << temp.input1WireName << ", " << temp.input2WireName ;
		if(temp.bits_selectLines > 0)
		{
			fsmfile << ", " << temp.selectLineName;
		}
		fsmfile << ")" << SEMI_COLON << endl;
	}
}

void writeToFromReg(string currentState, int tabCount)
{
	for(int i = 0; i < rtOperationsList.size(); i++)
	{
		struct rtOperation temp = rtOperationsList[i];
		if(strcmp(temp.state.c_str(), currentState.c_str()) == 0)
		{
			if(strcmp(temp.operationType.c_str(),"R")==0 || strcmp(temp.operationType.c_str(),"r")==0)
			{
				createTabSpace(tabCount);
				fsmfile << temp.readToReg << " <= " << temp.inputPortName << SEMI_COLON <<  endl;
			}
			else if(strcmp(temp.operationType.c_str(),"A")==0 || strcmp(temp.operationType.c_str(),"a")==0)
			{
				cout << "Finding: " << temp.outputReg << endl;
				if(regWireList.find(temp.outputReg) != regWireList.end())
				{
					createTabSpace(tabCount);
					fsmfile << temp.outputReg << " <= " << regWireList[temp.outputReg] << SEMI_COLON <<  endl;		
				}
				else
				{
				    createTabSpace(tabCount);
				    fsmfile << temp.outputReg << " <= " << temp.fuOutputWire << SEMI_COLON <<  endl;
				}
		}
			else if(strcmp(temp.operationType.c_str(),"O")==0 || strcmp(temp.operationType.c_str(),"o")==0)
			{
				createTabSpace(tabCount);
				fsmfile  << temp.outputPortName << " <= " << temp.outputReg << SEMI_COLON <<  endl;
			}
		}
	}
	
}

//fills the next state select lines and state values in the FSM
void writeStateInfo(string state, int tabCount)
{
	struct state temp = stateList[state];
	createTabSpace(tabCount);
	fsmfile << "state <= " << temp.binary_rep << SEMI_COLON <<  endl; 
	map<string, string> muxMap = temp.mux_select;
	map<string,string>::iterator it = muxMap.begin();
	for (it=muxMap.begin(); it!=muxMap.end(); ++it)
	{
		createTabSpace(tabCount);
		fsmfile << it->first << " <= " << it->second << SEMI_COLON << endl;
	} 
	map<string, string> fumap = temp.fu_select;
	map<string,string>::iterator fuItr = fumap.begin();
	for (fuItr = fumap.begin(); fuItr != fumap.end(); ++fuItr)
	{
			createTabSpace(tabCount);
			fsmfile << fuItr->first << " <= " << fuItr->second << SEMI_COLON << endl;
	}
}

//If there is a mux to save the value into register, write assign reg = wirename;
void writeMuxOutputToreg(int tabCount)
{
	map<string, string>:: iterator itr = regWireList.begin();
	for(; itr != regWireList.end(); itr++)
	{
		createTabSpace(tabCount);
		fsmfile << ASSIGN << " "  << itr->first << " = " << itr->second << SEMI_COLON <<  endl;
	}
	fsmfile << endl;
}
//initiates the required values for fsm
void initiateRequiredValues()
{
	struct state temp = stateList[stateSeqList[0]];
	fsmfile << TAB << INITIAL << endl;
	fsmfile << TAB << TAB << BEGIN << endl;
	if(temp.no_ways == 1)
	{ 
		writeStateInfo(stateSeqList[0], 3);
		writeToFromReg(stateSeqList[0], 3);
		writeMuxOutputToreg(3);
	}
	fsmfile << TAB << END << endl;
}

//code for FSM creation
void writeFSM()
{
	string initial_state = "";
	struct state temp = stateList[stateSeqList[0]];
	initial_state = temp.binary_rep;
	
	fsmfile << MODULE << " fsm( clk, rst " ;//<< SEMI_COLON << endl;
	map<string,struct portInfo>::iterator itr = portWidth.begin();
	for(; itr != portWidth.end(); itr++) // reg inputs to the fsm
	{
		fsmfile << ", " << itr->first ;
	}
	
	fsmfile << ")" << SEMI_COLON << endl;
	fsmfile << TAB << INPUT << " clk, rst" << SEMI_COLON << endl << endl;
	writePortsToFSMFile();
	writeRegtoFSMFile();
	writeSelectToFsmFile();
	writeFuInputsToFsmFile();
	writeFuOutputsToFsmFile();
	writeStateTofsmFile();
	instantiateFU();
	instantiateMux();
	//initiateRequiredValues();
	fsmfile << endl;
	
	//module declaration
	fsmfile << TAB << ALWAYS << "(" << POSEDGE << " clk or "<< POSEDGE << " rst )" << endl;
	fsmfile << TAB << TAB << BEGIN << endl;
	
	//if reset go to initial state
   	fsmfile << TAB << TAB << TAB << IF << "( rst )" << endl;
	fsmfile << TAB << TAB << TAB << TAB << BEGIN << endl;
	if(temp.no_ways == 1)
	{
		writeStateInfo(stateSeqList[0], 6);
		writeToFromReg(stateSeqList[0], 6);
	}
	fsmfile << TAB << TAB << TAB << TAB <<END << endl;
  	fsmfile << TAB << TAB << TAB << ELSE << endl;
  	fsmfile << TAB << TAB << TAB << BEGIN << endl;
    fsmfile << TAB << TAB << TAB << TAB << CASE << "(state)" << endl;
	map<string,struct state>::iterator it = stateList.begin();
	for (it=stateList.begin(); it!= stateList.end(); ++it)
	{	
	    string currentState = it->first;
		struct state temp = it->second;
		fsmfile << TAB << TAB << TAB << TAB << TAB << temp.binary_rep << COLON << " " << BEGIN  << "//" << it->first << endl;
		if(temp.no_ways == 1)// if there is only 1 way
		{
          	writeStateInfo(temp.nextState, 6);	
			writeToFromReg(currentState, 6);	
		}
		else if(temp.no_ways == 2)//if the block is control block
		{
			map<string, string> regSelect = temp.reg_select;
			for(int i = 0; i < regSeqList.size(); i++)
			{
				if(strcmp(regSelect[regSeqList[i]].c_str(),"1")==0)
				{
					writeToFromReg(currentState, 6);
					fsmfile << TAB << TAB << TAB << TAB << TAB << TAB << IF <<"(" << regSeqList[i] << " == 0)" << endl;
					fsmfile << TAB << TAB << TAB << TAB << TAB << TAB << TAB << BEGIN << endl;
					writeStateInfo(temp.falseNextState, 8);	
					fsmfile << TAB << TAB << TAB << TAB << TAB << TAB << TAB << END << endl;
					fsmfile << TAB << TAB << TAB << TAB << TAB << TAB << ELSE << endl;
					fsmfile << TAB << TAB << TAB << TAB << TAB << TAB << TAB << BEGIN << endl;
					writeStateInfo(temp.trueNextState, 8);
					fsmfile << TAB << TAB << TAB << TAB << TAB << TAB << TAB << END << endl;
					break;
				}
			}
		}
		else if(temp.no_ways == 0)//if it is the end state
		{
			fsmfile << TAB << TAB << TAB << TAB << TAB << TAB << "state <= " << initial_state << SEMI_COLON << endl;	
			//fsmfile << TAB << TAB << TAB << TAB << TAB << TAB <<ASSIGN << " " << "state = " << initial_state << SEMI_COLON << endl;	
		}
		
		fsmfile << TAB << TAB << TAB << TAB << TAB << TAB <<END << endl;
	}
	fsmfile << TAB << TAB << TAB << TAB << ENDCASE << endl;
	fsmfile  << TAB << TAB << END << endl;
	fsmfile << TAB << END << endl;
	fsmfile << ENDMODULE << endl;
}

//write the state variable to verilog file
void writeStateTofsmFile()
{
	fsmfile << TAB << REG << "[" << bits_req-1 << ":0] state" << SEMI_COLON << endl << endl;
}

//declare the wires which give input to FU in the verilog file
void writeFuInputsToFsmFile()
{
	
	for(int i = 0; i < muxSeqList.size(); i++)
	{
		fsmfile << TAB << WIRE << "[" << muxList[muxSeqList[i]].outputWireWidth -1 << ":0] " << muxList[muxSeqList[i]].outputWireName << SEMI_COLON<< endl;
	}
	fsmfile << endl;
}

//write the wire variables which are output of the function unit
void writeFuOutputsToFsmFile()
{
	
	for(int i = 0; i < fuSeqList.size(); i++)
	{
		struct function_unit temp = fuList[fuSeqList[i]];
		fsmfile << TAB << WIRE << "[" << temp.output_width -1 << ":0] " << temp.outputWireName << SEMI_COLON<< endl;
	}
	fsmfile << endl;
}

//generate Verilog file
void generateVerilogFile()
{
	generateFunctionModule();
	writeFSM();	
	closeOfStream();
}

//checks if the token passed is number
bool isNum(string token)
{
	 return !token.empty() && token.find_first_not_of("0123456789") == string::npos;
}

//returns the vector of comma separated values of string list passed
vector<string> getStringVectorListCommaSeparatedValues(string list)
{
	vector<string> ItemVector;
	char* cstr=const_cast<char*>(list.c_str());
	char* current= strtok(cstr,",");
	while(current!=NULL)
	{
		ItemVector.push_back(trim(current));
		current= strtok(NULL,",");
	}	
	return ItemVector;
}

//returns the vector of comma separated values of int list
vector<int> getIntVectorListCommaSeparatedValues(string list)
{
	vector<int> ItemVector;
	char* cstr=const_cast<char*>(list.c_str());
	char* current= strtok(cstr,",");
	while(current != NULL)
	{
	    cout << "current token: " << current << endl;
		string trimToken = trim(current);	
		if(isNum(trimToken))
		{
			ItemVector.push_back(atoi(trimToken.c_str()));
			current=strtok(NULL,",");
		}
		else
		{
			cout << "The entry in this list " << list << " is not a number" << endl;
			exit(0);
		}
	}	
	return ItemVector;
}

//prints the mux list
void printMUXList()
{
	cout << "---------------------------------MUX's defined------------------------------------------------" << endl;
	map<string,struct mux>::iterator it = muxList.begin();
	for (it=muxList.begin(); it!=muxList.end(); ++it)
	{
    		cout << it->first << " => " << endl << "<";	
		struct mux temp = it->second;
		
		map<string,string>::iterator input_itr = temp.input_cs.begin();
		for (input_itr=temp.input_cs.begin(); input_itr!=temp.input_cs.end(); ++input_itr)
		{
			cout << input_itr->first << ": " << input_itr->second << ", ";
		}
		cout << ">< " << temp.output << ">";
		cout << "<bits:" << temp.bits_selectLines << ">" << endl;
	}
	cout << endl << endl;
}

//prints the mux list
void printFUList()
{
	cout << "---------------------------------FU defined------------------------------------------------" << endl;
	map<string,struct function_unit>::iterator it = fuList.begin();
	for (it=fuList.begin(); it!= fuList.end(); ++it)
	{	
		struct function_unit temp = it->second;	
		cout << "<" << it->first << "><";
		int j = 0;
		for(j = 0; j < temp.operations.size() - 1; j++)
		{
			cout << temp.operations[j] << ",";
		}
		cout << temp.operations[j] << "><" << temp.output_width << " " << temp.inputWidth1 << " " << temp.inputWidth2 << ">" << "<" << temp.bits_selectLines << ">" << endl;
	}
	
	cout << endl << endl;
}

//prints the fsm
void printFSM()
{
	cout << "---------------------------------FSM defined------------------------------------------------" << endl;
	map<string,struct state>::iterator it = stateList.begin();
	for (it=stateList.begin(); it!= stateList.end(); ++it)
	{	
		struct state temp = it->second;
		cout << "<" << it->first << "> <" << temp.no_ways << "> " ;
		if(temp.no_ways == 1)
		{
			cout << "<" << temp.nextState << "> ";
		}
		else if(temp.no_ways == 2)
		{
			cout << "<T: " << temp.trueNextState << "> <F: " << temp.falseNextState << ">";
		}
		//print MUX select Lines
		map<string,string>::iterator it = temp.mux_select.begin();
		for (it=temp.mux_select.begin(); it!=temp.mux_select.end(); ++it)
		{
			cout << it->first << " - "  << it->second << ", ";
		}
		cout << endl;
		//print Reg select Lines
		it = temp.reg_select.begin();
		for (it=temp.reg_select.begin(); it!=temp.reg_select.end(); ++it)
		{
			cout << it->first << " - "  << it->second << ", ";
		}
		cout << endl;
		//print FU select Lines
		it = temp.fu_select.begin();
		for (it=temp.fu_select.begin(); it!=temp.fu_select.end(); ++it)
		{
			cout << it->first << " - "  << it->second << ", ";
		}
		cout << "< state rep: " << temp.binary_rep << ">" << endl;
	}
}

//get binary value of the number of size "bits"
string getBinary(int number, int bits)
{
	int temp = number;
	string binary = "";
	if(bits > 0)
	{
		for (int c = 0; c <  bits; c++)
		    {
			if(temp % 2 == 1)
			{
				binary = NumberToString(1) + binary;
			}
			else
			{
				binary = NumberToString(0) + binary;
			}
			temp = temp >> 1;
		    } 
	}
	return binary;
}

//populate the binary representation of states
void populateVerilogStateRepresentations()
{
	//calculate no of bits 
	int num_states = no_states;
	while(num_states != 0)
	{
		num_states = num_states >> 1;
		bits_req++;
	}
	
	int count = 0;
	cout << "---------------------------------FSM State Representations------------------------------------------------" << endl;
	map<string,struct state>::iterator it = stateList.begin();
	for (it=stateList.begin(); it!= stateList.end(); ++it)
	{
		(it->second).binary_rep = NumberToString(bits_req)+"'b"+ getBinary(count,bits_req);
		count++;
	}		
}

//check if reg and MUX are already linked to FU
bool isRegMuxLinkedToFU(string fu, string reg)
{
	for(int i = 0; i < muxSeqList.size(); i++)
	{
		vector<string> regFu = muxLinks[muxSeqList[i]];
		if (std::find(regFu.begin(), regFu.end(),fu) != regFu.end())
		{
			if (std::find(regFu.begin(), regFu.end(),reg) != regFu.end())
			{
				return true;
			}	
		}
	}
	return false;
}

//populates the wires if there is no mux connection in between reg and Functional unit
void populateRegFuLinkNames()
{
	for(int i = 0 ; i< fuSeqList.size(); i++)
	{
		string fuName = fuSeqList[i];
		struct function_unit temp = fuList[fuName];
		cout << fuName << " " << temp.input1WireName << " " << temp.input2WireName << endl;
		if(temp.input1WireName.empty() && temp.input2WireName.empty())
		{
			cout << "Both are empty" << endl;
			 fuList[fuName].input1WireName = regFuLinkList[fuName].input1WireName;
			 fuList[fuName].input2WireName = regFuLinkList[fuName].input2WireName;
		} 
		else if(temp.input2WireName.empty())
		{
			if(!isRegMuxLinkedToFU(fuName, regFuLinkList[fuName].input1WireName))//check if reg is linked to mux. If not populate wire2 of FU with reg
			{
				fuList[fuName].input2WireName = regFuLinkList[fuName].input1WireName;
			}
			else if(!isRegMuxLinkedToFU(fuName, regFuLinkList[fuName].input2WireName))//check if reg is linked to mux. If not populate wire2 of FU with reg
			{
				fuList[fuName].input2WireName = regFuLinkList[fuName].input2WireName;
			}
		}
		cout << fuName << " " <<	fuList[fuName].input1WireName << " " << 	fuList[fuName].input2WireName << endl;
	}
}

//populates wire names for Mux and FU link MUX-> wire -> FU
void populateMuxFuLinkNames()
{
	for(int i = 0; i < muxSeqList.size() ; i++)
	{
		string fuOrReg = muxList[muxSeqList[i]].output;//output can be register or FU
		string muxName = muxSeqList[i];
		string wireName = muxName + fuOrReg;
		muxList[muxSeqList[i]].outputWireName = wireName;
		cout << "mux fu name: " << muxList[muxSeqList[i]].outputWireName << endl;
		
		bool found = false;
		if(fuList.find(fuOrReg) != fuList.end()) //check in registers
		{
			muxList[muxSeqList[i]].isoutputFU  = true;
			if(fuList[fuOrReg].input1WireName.empty())
			{
					fuList[fuOrReg].input1WireName = wireName;
					muxList[muxSeqList[i]].outputWireWidth = fuList[fuOrReg].inputWidth1;
			}
			else if((fuList[fuOrReg].input2WireName).empty())
			{
					fuList[fuOrReg].input2WireName = wireName;
					muxList[muxSeqList[i]].outputWireWidth = fuList[fuOrReg].inputWidth2;
			}
			
		} 
		else if(registers.find(fuOrReg) != registers.end())
		{
			string regWireName = fuOrReg + "_wire";
			regWireList.insert(pair<string, string>(fuOrReg, regWireName));
			muxList[muxSeqList[i]].outputWireName = regWireName;  
			muxList[muxSeqList[i]].outputWireWidth = registers[fuOrReg];
			muxList[muxSeqList[i]].isoutputFU = false;
			struct mux temp = muxList[muxSeqList[i]];
			map<string, string> inputs = temp.input_cs;
			map<string, string>::iterator itr = inputs.begin();
			for(; itr!= inputs.end(); itr++)
			{
				string inputName = itr->second;
				if(fuList.find(inputName) != fuList.end())
				{ 
					muxList[muxSeqList[i]].input_cs[itr->first] = fuList[inputName].outputWireName;
				}
			}
		}
	}
}

//checks whether there are
void generateFunctionModule()
{
	cout << "__________________________________generating function module___________________________________" << endl;
	for(int i = 0; i < fuSeqList.size() ; i++)
	{
		string module_name = fuSeqList[i];
		function_unit temp = fuList[module_name];
		
		const char* operation = temp.operations[0].c_str();
		cout << "bits: " << temp.bits_selectLines << " operation: " << operation << endl;
		if(temp.bits_selectLines == 0)
		{
			//generate Single modules
			if(strcmp(operation, C_ADD) == 0 || strcmp(operation, S_ADD) == 0)
			{
				generateSingleModule(module_name, temp.inputWidth1 , temp.inputWidth2, temp.output_width, string(ADD));
			}
			else if(strcmp(operation, C_SUB) == 0 || strcmp(operation, S_SUB) == 0)
			{
				generateSingleModule(module_name, temp.inputWidth1 , temp.inputWidth2, temp.output_width, SUB);
			}
			else if(strcmp(operation, C_MULT) == 0 || strcmp(operation, S_MULT) == 0)
			{
				generateSingleModule(module_name, temp.inputWidth1 , temp.inputWidth2, temp.output_width, MULT);
			}
			else if(strcmp(operation, C_DIV) == 0 || strcmp(operation, S_DIV) == 0)
			{
				generateSingleModule(module_name, temp.inputWidth1 , temp.inputWidth2, temp.output_width, DIV);	
			}
			else if(strcmp(operation, C_LT) == 0 || strcmp(operation, S_LT) == 0)
			{
				generateSingleModule(module_name, temp.inputWidth1 , temp.inputWidth2, temp.output_width, LT);
			}
			else if(strcmp(operation, C_GT) == 0 || strcmp(operation, S_GT) == 0)
			{
				generateSingleModule(module_name, temp.inputWidth1 , temp.inputWidth2, temp.output_width, GT);
			}
			else if(strcmp(operation, C_LTE) == 0 || strcmp(operation, S_LTE) == 0)
			{
				generateSingleModule(module_name, temp.inputWidth1 , temp.inputWidth2, temp.output_width, LTE);
			}
			else if(strcmp(operation, C_GTE) == 0 || strcmp(operation, S_GTE) == 0)
			{
				generateSingleModule(module_name, temp.inputWidth1 , temp.inputWidth2, temp.output_width, GTE);
			}
			else if(strcmp(operation, C_EQ) == 0 || strcmp(operation, S_EQ) == 0)
			{
				generateSingleModule(module_name, temp.inputWidth1 , temp.inputWidth2, temp.output_width, EQUAL);
			}
			else if(strcmp(operation, C_RS) == 0 || strcmp(operation, S_RS) == 0)
			{
				generateSingleModule(module_name, temp.inputWidth1 , temp.inputWidth2, temp.output_width, RIGHT_SHIFT);
			}
			else if(strcmp(operation, C_LS) == 0 || strcmp(operation, S_LS) == 0)
			{
				generateSingleModule(module_name, temp.inputWidth1 , temp.inputWidth2, temp.output_width, LEFT_SHIFT);
			}
			
		}
		else if(temp.bits_selectLines > 0)
		{
			int bits = temp.bits_selectLines;
			string binary = "";
			fsmfile << MODULE << " " << module_name << "( out, A, B , select )" << SEMI_COLON <<  endl;
			fsmfile << TAB << INPUT << "[" << temp.inputWidth1 - 1 << ":0] A" << SEMI_COLON << endl;
			fsmfile << TAB << INPUT << "[" << temp.inputWidth2 - 1 << ":0] B" << SEMI_COLON << endl;
			fsmfile << TAB << OUTPUT << " " << REG << "[" << temp.output_width - 1 << ":0]" << " out" << SEMI_COLON << endl;
			fsmfile << TAB << INPUT << " select " << SEMI_COLON << endl;
			fsmfile << TAB << ALWAYS << "(select or A or B)" << endl;
			fsmfile << TAB << BEGIN << endl;
			fsmfile << TAB << TAB <<  CASE << "(select)" << endl;
			vector<string> operations = temp.operations;
			int count = 0;
			for(int j =0; j < operations.size(); j++)
			{
				operation = operations[j].c_str();
				if(strcmp(operation, C_ADD) == 0 || strcmp(operation, S_ADD) == 0)
				{
					binary = getBinary(count, bits);
					count++;
					fsmfile << TAB << TAB << TAB << bits << "'b" << binary <<" : out = A " << ADD << " B" << SEMI_COLON << endl;
				}
				if(strcmp(operation, C_SUB) == 0 || strcmp(operation, S_SUB) == 0)
				{
					binary = getBinary(count, bits);
					count++;
					fsmfile << TAB << TAB << TAB << bits << "'b" << binary <<" : out = A " << SUB << " B" << SEMI_COLON << endl;
				}
				if(strcmp(operation, C_MULT) == 0 || strcmp(operation, S_MULT) == 0)
				{
					binary = getBinary(count, bits);
					count++;
					fsmfile << TAB << TAB << TAB << bits << "'b" << binary <<" : out = A " << MULT << " B" << SEMI_COLON << endl;
				}
				if(strcmp(operation, C_DIV) == 0 || strcmp(operation, S_DIV) == 0)
				{
					binary = getBinary(count, bits);
					count++;
					fsmfile << TAB << TAB << TAB << bits << "'b" << binary <<" : out = A " << DIV << " B" << SEMI_COLON << endl;	
				}
				if(strcmp(operation, C_LT) == 0 || strcmp(operation, S_LT) == 0)
				{
					binary = getBinary(count, bits);
					count++;
					fsmfile << TAB << TAB << TAB << bits << "'b" << binary <<" : out = A " << LT << " B" << SEMI_COLON << endl;
				}
				if(strcmp(operation, C_GT) == 0 || strcmp(operation, S_GT) == 0)
				{
					binary = getBinary(count, bits);
					count++;
					fsmfile << TAB << TAB << TAB << bits << "'b" << binary <<" : out = A " << GT << " B" << SEMI_COLON << endl;
				}
				if(strcmp(operation, C_LTE) == 0 || strcmp(operation, S_LTE) == 0)
				{
					binary = getBinary(count, bits);
					count++;
					fsmfile << TAB << TAB << TAB << bits << "'b" << binary <<" : out = A " << LTE << " B" << SEMI_COLON << endl;
				}
				if(strcmp(operation, C_GTE) == 0 || strcmp(operation, S_GTE) == 0)
				{
					binary = getBinary(count, bits);
					count++;
					fsmfile << TAB << TAB << TAB << bits << "'b" << binary <<" : out = A " << GTE << " B" << SEMI_COLON << endl;
				}
				else if(strcmp(operation, C_EQ) == 0 || strcmp(operation, S_EQ) == 0)
				{
					cout << "eual" << endl;
					binary = getBinary(count, bits);
					count++;
					fsmfile << TAB << TAB << TAB << bits << "'b" << binary <<" : out = (A " << EQUAL << " B)" << SEMI_COLON << endl;
				
				}
				else if(strcmp(operation, C_RS) == 0 || strcmp(operation, S_RS) == 0)
				{
					binary = getBinary(count, bits);
					count++;
					fsmfile << TAB << TAB << TAB << bits << "'b" << binary <<" : out = A " << RIGHT_SHIFT << " B" << SEMI_COLON << endl;
				
				}
				else if(strcmp(operation, C_LS) == 0 || strcmp(operation, S_LS) == 0)
				{
					binary = getBinary(count, bits);
					count++;
					fsmfile << TAB << TAB << TAB << bits << "'b" << binary <<" : out = A " << LEFT_SHIFT << " B" << SEMI_COLON << endl;
				}
			}
			fsmfile << TAB << ENDCASE << endl;
			fsmfile << TAB << END << endl;
			fsmfile << ENDMODULE << endl << endl;
		
		}
		else 
		{
			cout << "Invalid Operation count for " << module_name << endl;
			exit(0);
		}
	}
}

//for Fu's with single operation without case
void generateSingleModule(string module_name, int inputWidth1, int inputWidth2, int outputWidth, string operation)//0 - add, 1- sub
{
	fsmfile << MODULE << " " << module_name << "( out, A, B)" << SEMI_COLON <<  endl;
	fsmfile << TAB << INPUT << "[" << inputWidth1 - 1 << ":0] A" << SEMI_COLON << endl;
	fsmfile << TAB << INPUT << "[" << inputWidth2 - 1 << ":0] B" << SEMI_COLON << endl;
	fsmfile << TAB << OUTPUT << " " << REG << "[" << outputWidth-1 << ":0]" << " out" << SEMI_COLON << endl;
	fsmfile << TAB << ALWAYS << "(A or B)" << endl;
	fsmfile << TAB << BEGIN << endl;
	fsmfile <<  TAB << TAB << "out = A " << operation << " B" << SEMI_COLON << endl;
	fsmfile << TAB << END << endl;
	fsmfile << ENDMODULE << endl << endl;	
}

//gives inputs to mux
void writeMuxInput(string delimeter, int no_inputs)
{
	int count = 0;
	int no_delimeters = no_inputs - 1;
	while(count < no_inputs)
	{
		fsmfile << "I" <<count;
		if(count < no_delimeters)
		{	
			if(strcmp(delimeter.c_str(), ",")==0)
			{
				fsmfile << delimeter ;
			}
			else
			{
				fsmfile << " " << delimeter;
			}
		}
		fsmfile << " " ;
		count++;
	}
}


//gives cases
void writeMuxSelectOperation(int select_lines, int tabcount)
{
	int count = 0;
	int no_inputs = pow(2,select_lines);
	while(count < no_inputs)
	{
		createTabSpace(tabcount);
		fsmfile << select_lines << "'b" << getBinary(count, select_lines) << ":y = " << "I" <<count << SEMI_COLON << endl;
		count++;
	}
}

/*create generic mux*/
void createMux(int select_lines)
{
	int no_inputs = pow(2,select_lines);
	fsmfile << MODULE << " mux_" << no_inputs << "x1(" ;
	writeMuxInput(",", no_inputs);
	fsmfile << ", select, y)" << SEMI_COLON << endl;
	fsmfile << TAB << INPUT << "[199:0] " ;
	writeMuxInput(",", no_inputs);//giving inputs
	fsmfile << SEMI_COLON << endl;
	fsmfile << TAB << INPUT << " " << SELECT << SEMI_COLON << endl;
	fsmfile << TAB << OUTPUT << " " << REG << "[199:0] y"  << SEMI_COLON << endl;
	fsmfile << TAB << ALWAYS << " (" ;
	writeMuxInput("or", no_inputs);
	fsmfile << " or select)" << endl;
	fsmfile << TAB << TAB << BEGIN << endl;
	fsmfile << TAB << TAB << TAB << CASE << "(select)" << endl;
	writeMuxSelectOperation(select_lines, 4);//write cases here
	fsmfile << TAB << TAB << TAB << ENDCASE << endl;
	fsmfile << TAB << TAB << END << endl;
	fsmfile << ENDMODULE << endl << endl;
}
