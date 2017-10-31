/*************************************************************************************
Author: Oushesh Haradhun
/**************************************************************************************
External Callback Function controlling event change when the Cursor is moved over to  
the central arrow.
*******************************************************************************/
//touchcallback external Function
//TouchCallbackFunction
#include "HapticCallback.h"
#include <QHHeadersWin32.h>
#include <HDU/hduMatrix.h>
#include <HL/hl.h>
#include <HDU/hduMatrix.h>
#include <HDU/hduError.h>
#include <HLU/hlu.h>
#include <string.h>
int HapticCallback::touchCallbackFunction(int TouchControl,int TOUCH,static int Ten,int TrialNumber,char* Name[12],HDstring WhiteTex, HDstring GreyTex, string name, unsigned int ShapeID)
{
	Box* touchedEndTip = Box::searchBox("EndTip");//get a local pointer to the Cursor on which the cross appears; movement Cue
	if (touchedEndTip == NULL)//id the pointer is NULl then return
		return -1;
	else
	{
		HDstring texture = (HDstring)touchedEndTip->getTextureFilename();//Query Current Texture on the Pointer
		if (texture == WhiteTex)
		{
			texture = GreyTex; //Compare the Texture with the Arrow; 
			TouchControl = 1; //Set white_grey to activate Transition from Grey Cross to Black Arrow/Movement Cue
			touchedEndTip->setHapticVisibility(false); //Set it now to Haptically inactive for smooth movement towards the goal character for ENDUSER
		}
		touchedEndTip->setTexture(texture);//Set a texture on it without DELAY
		//Write The Event in Parameters.csv: Cross Touched Transition:White-> Grey
		char numstr[21]; // enough to hold all numbers up to 64-bits for conversion of Current Trial as integer to string
		char TrialPrefix[21];
		std::string vp = "VP";
		int TrialSet = 1;
		sprintf(TrialPrefix, "%d", TrialSet);
		sprintf(numstr, "%d", TrialNumber);
		ofstream Param; Param.open((TrialPrefix + vp + numstr + name).c_str(), ofstream::app); //open file in append mode
		//Writing Column Headers for the Parameters
		Param << "Current Trial Number" << ";" << "TimeStamp/10ms" << ";" << "CuePosition" << ";" << "AtCueOROtherPositions" << ";" << "OtherPositions" << ";" << "RandTimeInt/ms" << ";" << "SIXHUNDREDTIMEINT/ms" << ";" << "EVENT" << "\n";
		Param << TrialNumber << ";" << Ten * 10 << ";" << ";" << ";" << "Cross Touched" << ";" << ";" << ";" << "White->Grey" << "\n"; Param.close();
	}
	///////////////////////////////////////////////////////////////////////////
	//Touch Callback Function to know which Character was touched by 
	// ENDUSER return the posiion of the Character TOCHECK: 31.08.2017
	///////////////////////////////////////////////////////////////////////////
	if (TOUCH == 1)
	{
		for (int idx = 0; idx < 12; idx++)
		{
			Box* BoxTouched = Box::searchBox(Name[idx]);//get a local pointer to the Box Touched by user

			//if (BoxTouched == NULL)//if the pointer is Null then return
				//return;
			//else
			{
				//13.09.2017. TO MERGE with the File Parameters.csv via class Parameters
				//Position_Touched = idx;
				fstream User_Touch;
				User_Touch.open("Position_Touched.txt");
				User_Touch << "\n" << idx;  //Writing Position shown by ENDUSER to file
				User_Touch.close();
			}
		}
	}
	return TouchControl;
}