/***********************************************************************************
/*Author: Oushesh Haradhun: Given the position of the two "charges" at positions 1, 
positions 2 in space,calculates the Electrostatic force at 1khz based on the 
computeForceCB Loop
************************************************************************************/
#include "Force.h"
#include <QHHeadersWin32.h>
#include <HDU/hduMatrix.h>
#include <HL/hl.h>
#include <HDU/hduMatrix.h>
#include <HDU/hduError.h>
#include <HLU/hlu.h>
#include <string.h>
hduVector3Dd Force::forceField(hduVector3Dd Pos1, hduVector3Dd Pos2, HDdouble Multiplier, HLdouble Radius)
{
	hduVector3Dd diffVec = Pos2 - Pos1;//Find the difference in position
	double dist = 0.0;
	hduVector3Dd forceVec(0, 0, 0);
	HDdouble nominalMaxContinuousForce;
	hdGetDoublev(HD_NOMINAL_MAX_CONTINUOUS_FORCE, &nominalMaxContinuousForce);//Find the max continuous for that the device is capable of
	dist = diffVec.magnitude(); //getting absolute value of distance
	if (dist < Radius*2.0) //If position difference is less than adjust the constant with the introduction of constant 4.0 to weaken the force as an attempt to avoid too large forces
	{
		diffVec.normalize(); //normalise vector to get only the Uni Vector
		forceVec = (Multiplier)* diffVec * dist / (4.0 * Radius * Radius); // Calculation of the magnitude of the force as forceVec
		static int i = 0;
	}
	else // normals Inverse square Field Force 
	{
		forceVec = Multiplier * diffVec / (dist*dist);
	}
	for (int i = 0; i<3; i++)//Limit force calculated to Max continuous. This is very important because force values exceeding this value can damage the device motors.
	{
		if (forceVec[i]>nominalMaxContinuousForce)
			forceVec[i] = nominalMaxContinuousForce;

		if (forceVec[i]<-nominalMaxContinuousForce)
			forceVec[i] = -nominalMaxContinuousForce;
	}
	return forceVec;
}
//OPTION For external Force ()
//Mapping hluFitWorkspace()
//REVERSE THE MAPPING between screen cursor movement and real 3D Cursor Movement

