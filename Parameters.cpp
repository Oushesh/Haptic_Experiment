/*************************************************************************************
Author: Oushesh Haradhun
/**************************************************************************************
EXPERIMENT PARAMETERS
ParameterWrite in .csv file Format as following with each cell on the same row:
1. Current Trial Number
2. Time at which Event occurs: Steps of 10
3. Random Position of Black Movement Cue
4. Rand_3E; 
6. Rand_not_adjacent
7. Comment on the Event Occurred
*******************************************************************************/
#include "Parameters.h"
string ParametersFilename(int TrialSet,int Trial_number);
int Parameters::ParameterOutput(int E3Abfrage,int TimeStamp, int SIXHundredTimeStamp,int TrialSet, int Ten, int Effect_8, int Helper, int white_grey, int Trial_number, int Random_Digit, int Rand_3E, int Rand_not_adjacent)
{
	ofstream Param;
	string CurrentFilename= ParametersFilename(TrialSet,Trial_number);
	/*Additionally Event Updates*****************************************************/
	if (white_grey == 4) 
	{
		//ofstream Param;
		Param.open(CurrentFilename.c_str(), ofstream::app); //open file in append mode
		Param << Trial_number << ";" << Ten * 10 << ";" << Random_Digit << ";" << Rand_3E << ";"
			<< Rand_not_adjacent << ";" << TimeStamp << ";" << SIXHundredTimeStamp <<";" << "Black Arrow/" << ";" << "Movement Cue" << ";" << "Appeared" << "\n"; Param.close();
	}
	if (white_grey == 5 && Helper == 4)
	//if (white_grey == 10 && Helper == 9)
	{
		//ofstream Param;
		Param.open(CurrentFilename.c_str(), ofstream::app); //open file in append mode
		Param << Trial_number << ";" << Ten * 10 << ";" << Random_Digit << ";" << Rand_3E << ";"
			<< Rand_not_adjacent << ";" << TimeStamp << ";" << SIXHundredTimeStamp << ";" << "Characters" << "; " << "2 or 5" << "; " << "Appeared" << "\n"; Param.close();
	}
	//if(white_grey== 1003 && Helper == 1002 && Effect_8==1)
	if (white_grey == 151 && Helper == 150 && Effect_8 == 1)
	{
		//ofstream Param;
		Param.open(CurrentFilename.c_str(), ofstream::app); //open file in append mode
		Param << Trial_number << ";" << Ten * 10 << ";" << Random_Digit << ";" << Rand_3E << ";"
			<< Rand_not_adjacent << ";" << TimeStamp << ";" << SIXHundredTimeStamp << ";" << "Characters" << ";" << "8" << ";" << "Appeared" << "\n"; Param.close();
	}
	//Writing Experiment Results whether User saw 3 or E
	if (E3Abfrage == 1)
	{
		Param.open(CurrentFilename.c_str(), ofstream::app); //open file in append mode
		Param << "User saw 3" << "\n";
		Param.close();
	}
	if (E3Abfrage == 0)
	{
		Param.open(CurrentFilename.c_str(), ofstream::app); //open file in append mode
		Param << "User saw E" << "\n";
		Param.close();
	}
	return 0;
}
//1st Column Randomise Time chosen between 580-880ms (TODO): 
//Function which returns the name of the .csv file depending on the Current_Trial Number
//Note This function could be written otherwise with fastformat librariers which
//are unavailable for VS 2010
string ParametersFilename(int TrialSet,int Trial_number)
{
	std::string name = "Parameters.csv";
	std::string result;
	std::string TrialString;
	std::string vp="VP"; //VP: stands for VersuchPersonne
	char numstr[21]; //enough to hold all numbers up to 64-bits
	char TrialPrefix[21]; 
	sprintf(numstr, "%d", Trial_number);
	TrialString = numstr + name;
	sprintf(TrialPrefix, "%d", TrialSet);
	result = TrialPrefix + vp +  TrialString; //VersuchspersonneNummer + VP + TrialNumber + Parameters.csv
	return result;
}
