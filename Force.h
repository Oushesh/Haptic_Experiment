/*************************************************************************************
Author: Oushesh Haradhun
/**************************************************************************************
Class Definition of Class Class Defintion of Class Force containing Function to 
calculate the Perturbation Force
********************************************************************************/
#pragma once
class Force
{
public:
	hduVector3Dd forceField(hduVector3Dd Pos1, hduVector3Dd Pos2, HDdouble Multiplier, HLdouble Radius);
};
