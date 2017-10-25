/*************************************************************************************
Author: Oushesh Haradhun
/**************************************************************************************
/************************************************************************************************************
Position shown by movement Cue is calculated and written out on log File
Random Position as Rand_Positioncalculated and written in log_file "Random_digit.txt"
Rand_Position to define the rotation of the movement cue
In the beginning all the Clock Numbers will be loaded then afterwards the one at which
the arrow is gonna be rotated to point at the target

In 50% of the cases a number 3 or E appears at the position shown by the EndTip Pointer
umber 3 or E has a probablity of 50 % in this case as well
1. Binary Random variable Rand_3E used with either 1 or 0 to express this Distribution
Case 0: E/3 appears where the movement cue shows
Case 1: E/3 appears at any other position apart from the position showed by movement cue or adjacent to the
movement Cue
/***********************************************************************************************************/
#include "RotationPosition.h"
#include <QHHeadersWin32.h>
#include <HDU/hduMatrix.h>
#include <HL/hl.h>
#include <HDU/hduMatrix.h>
#include <HDU/hduError.h>
#include <HLU/hlu.h>
#include <string.h>
//#include "Randomise.h"
//#include "Randomise.cpp"
																			
int RotationPosition::Rotate(HDstring EightTex,hduMatrix WorldToDevice, int Rand_3E, float degInRad, hduVector3Dd EndPosition, Box* Object,QHWin32* Display)
{
	int lower_limit_12 = 1; //Randomise the elements to be presented with lower limit of integer to 
	int upper_limit_12 = 12;  //be randomised as number between 1 and 12.
	srand(time(NULL));
	int globalRand_Position;
	Randomise* currentRandomisation = new Randomise();
	int Rand_Position = currentRandomisation->function_randomise(lower_limit_12, upper_limit_12); //using the pointer to call the function in Randomise.cpp
	globalRand_Position = Rand_Position; //Global RandPosition is going to be used later for the rotating effect of the movementcue in Function Transistions
	switch (Rand_3E)
	{
		case 0:
		{
			Object->setTexture(EightTex); //For the First Frame
			Object->setHapticVisibility(false);//Make it haptically invisible
			Object->setName("Number_3E");//Give it a name
			Object->setTranslation(-EndPosition[0] - (72 * 500 / 1024 * cos(Rand_Position * degInRad)*WorldToDevice[0][0] + 72 * 500 / 1024 * sin(Rand_Position * degInRad)*WorldToDevice[1][0])
				, -EndPosition[1] - (72 * 500 / 1024 * cos(Rand_Position * degInRad)*WorldToDevice[0][1] + 72 * 500 / 1024 * sin(Rand_Position * degInRad)*WorldToDevice[1][1]), -EndPosition[2]);
			Display->tell(Object);
		}
		break;
		
		case 1:
		{
			int lower_limit = 1;
			int upper_limit = 12;

			int Rand_not_adjacent = currentRandomisation->function_randomise(lower_limit, upper_limit);
			while (Rand_not_adjacent == Rand_Position || Rand_not_adjacent == Rand_Position + 1 || Rand_not_adjacent == Rand_Position - 1)
			{
				int Rand_not_adjacent = currentRandomisation->function_randomise(lower_limit, upper_limit);
			}
			globalRand_Position = Rand_not_adjacent; //Global RandPosition is going to be used later for the rotating effect of the movementcue in Function Transistions; Effect2_5
			Object->setTexture(EightTex);
			Object->setHapticVisibility(false);//Make it haptically invisible
			Object->setName("Number_3E");//Give it a name
			//Periodic Fuction cosine and sine-> zB. wenn Rand_Position+1=13 ist; dann cos(13*30)=cos(1*30). Gleich gilt für sin Funktion
			Object->setTranslation(-EndPosition[0] - (72 * 500 / 1024 * cos(Rand_not_adjacent * degInRad)*WorldToDevice[0][0] + 72 * 500 / 1024 * sin(Rand_not_adjacent * degInRad)*WorldToDevice[1][0])
				, -EndPosition[1] - (72 * 500 / 1024 * cos(Rand_not_adjacent * degInRad)*WorldToDevice[0][1] + 72 * 500 / 1024 * sin(Rand_not_adjacent * degInRad)*WorldToDevice[1][1]), -EndPosition[2]);
			Display->tell(Object);
		}
		break;
	}
	delete currentRandomisation;

	return globalRand_Position;
}
