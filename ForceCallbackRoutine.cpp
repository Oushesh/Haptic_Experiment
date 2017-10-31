#include "ForceCallbackRoutine.h"
#include "Force.cpp"
#include "PosForceWrite.cpp"

#include <QHHeadersWin32.h>
#include <HDU/hduMatrix.h>
#include <HL/hl.h>
#include <HDU/hduMatrix.h>
#include <HDU/hduError.h>
#include <HLU/hlu.h>

/****************************************************************************************
Servo loop thread callback.  Computes a force effect. This callback defines the motion
of the purple skull and calculates the force based on the "real-time" Proxy position
in Device space. (1khz HD servo loop)
***************************************************************************************/
void ForceCallbackRoutine::ForceCallbackRoutineFunction(int white_grey, int STOPPosForceUpdate, int Timer, int Ten, int TrialNumber, HDdouble force[3], HLcache *cache, void *userdata, bool ForcePresence, bool Headerwrite)
{
	hduVector3Dd centreposition(0.0, 0.0, 0.0);//Declaration of centre position 
	hduVector3Dd proxyPosition;//Position of the proxy/Cursor in device space
	hduVector3Dd forceVec;
	static int counter1 = 0;
	hlCacheGetDoublev(cache, HL_PROXY_POSITION, proxyPosition);//Get the position of the proxy in Device Coordinates 												
	/////////////////////////////////////////////////////////////////////////////
    //Determining whether External Perturbating Force should be present or not!
	//Only for the First 200 elements would the Force be present
	/////////////////////////////////////////////////////////////////////////////
	Force* Force_instance = new Force();
	switch (ForcePresence)
	{
	case 0:
		forceVec = Force_instance->forceField(proxyPosition, centreposition, 100.0, 10.0);//force between the 2 positions;100-Electrostatic constant,10-Tolerance Radius
		delete Force_instance;
		break;
	case 1:
		forceVec = hduVector3Dd(0.0, 0.0, 0.0);
		break;
	}
	if (white_grey > 0 && STOPPosForceUpdate != 0)//only write proxyposition+Force to .csv file once in 10ms and only during Transition Phase;
	{
		if (Timer <= 10)
		{
			Timer += 1;
		}
		else
		{
			Timer = 0; Ten += 1;
			PosForceWrite* PosForceWrite_Instance = new PosForceWrite(); //new instance of the class PosForceWrite to call function for .csv file 
			int TrialSet = 1;
			PosForceWrite_Instance->PosForceWriteFunction(TrialSet, Headerwrite, TrialNumber, Ten, proxyPosition, forceVec); //Takes in Argument ProxyPosition as hduVector3Dd
			Headerwrite = 1;
			//delete PosForceWrite_Instance; //destructor when instance is not used
		}
	}
	else
		Ten = 0;
	counter1++;
	if (counter1>2000)//Make the force start after 2 seconds of program start. This is because the servo loop thread executes before the graphics thread. 
	{
		force[0] = forceVec[0];
		force[1] = forceVec[1];
		force[2] = forceVec[2];
		counter1 = 2001;
	}
	else
	{
		force[0] = 0.0;
		force[1] = 0.0;
		force[2] = 0.0;
	}
}