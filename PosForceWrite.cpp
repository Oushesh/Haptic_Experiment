/******************************************************************************
Author: Oushesh Haradhun PosForceWrite.cpp
/*******************************************************************************
PosForceWrite Function: writes the Cursor Position + forceVector in 3D onto Log File
at each 10ms interval with the servoLopp Frequency at 1kHz + modified according
to accomodate the Format: Trial Number, Timing=10*number at 1kHz update Frequency, 
ProxyPosition, ForceVec
/*****************************************************************************/
#ifdef  _WIN64
#pragma warning (disable:4996)
#endif

#include <stdio.h>
#include <assert.h>

#if defined(WIN32)
# include <windows.h>
#endif

#include "PosForceWrite.h"
#include <HD/hd.h>
#include <HDU/hduError.h>
#include <HDU/hduVector.h>

string Filename(int TrialSet,int Trial_number);
void PosForceWrite::PosForceWriteFunction(int TrialSet,bool Headerwrite,int Trial_number,int Ten,hduVector3Dd proxyPosition, hduVector3Dd forceVec)
{
	string CurrentFilename = Filename(TrialSet, Trial_number);
	//Writing Column HeaderFile
	ofstream Header;
	if (Headerwrite == 0)
	{
		Header.open(CurrentFilename.c_str()); //Open File not in append mode !!! since the Header is written only once
		Header << "TrialNumber" << ";" << "TimeStamp/10ms" << ";" << "X-CursorPos" << ";" << "Y-CursorPos" << ";" << "Z-CursorPos"
			<< ";" << "X-ForceVec" << "Y-ForceVec" << "Z-ForceVec" << "\n";
		Header.close();
	}
	ofstream CursorLog;
	//Param.open("Parameters.csv", ofstream::app); //open file in append mode
	CursorLog.open(CurrentFilename.c_str(), ofstream::app); //open file in append mode
	CursorLog << Trial_number << ";" << Ten*10 << ";"<< proxyPosition[0] << ";" << proxyPosition[1] << ";" << proxyPosition[2] << ";"
		<< forceVec[0] << ";" << forceVec[1] << ";" << forceVec[2] << "\n";  //Writing Cursor 3D Position + 3D Force Vector
	CursorLog.close(); //close log File
}
//Function which returns the name of the .csv file depending on the Current_Trial Number
//Note This function could be written otherwise with fastformat libraries which would make the processing
//are unavailable for VS 2010. 
string Filename(int TrialSet, int Trial_number)
{
	std::string name = "PosForceWrite.csv";
	std::string result;
	std::string TrialString;
	std::string vp = "VP"; //VP: stands for VersuchPersonne
	char numstr[21]; //enough to hold all numbers up to 64-bits
	char TrialPrefix[21];
	sprintf(numstr, "%d", Trial_number);
	TrialString = numstr + name;
	sprintf(TrialPrefix, "%d", TrialSet);
	result = TrialPrefix + vp + TrialString; //VersuchspersonneNummer+ VP + TrialNumber + PosForceWrite.csv
	return result;
}

