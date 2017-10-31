/*************************************************************************************
Author Oushesh Haradhun: 
/**************************************************************************************
Class Definition of Class PosForceWrite to write the 00YPosForceWrite.csv.File
with Y as the prefix value for Current_Trial Number; containing the 3D Force Vector+ 
3D Cursor Position Vector 
********************************************************************************/
#pragma once
#include <QHHeadersWin32.h>
#include <HDU/hduMatrix.h>
#include <HL/hl.h>
#include <HDU/hduMatrix.h>
#include <HDU/hduError.h>
#include <HLU/hlu.h>
class PosForceWrite
{
public:
	void PosForceWriteFunction(int TrialSet,bool Headerwrite,int Current_Trial,int Ten,hduVector3Dd proxyPosition, hduVector3Dd forceVec);
};