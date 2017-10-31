//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Oushesh Haradhun Randomise.cpp
// Reference http://www.cplusplus.com/reference/cstdlib/rand/
// Function is able to randomise integer numbers between 1 and 12 corresponding to the positions of the characters in the clock
// Function will be called in the main main.cpp and Randomise.h has to be written
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Characters.h"
#include <QHHeadersWin32.h>
#include <HDU/hduMatrix.h>
#include <HL/hl.h>
#include <HDU/hduMatrix.h>
#include <HDU/hduError.h>
#include <HLU/hlu.h>
void Characters::CharactersSetting(int Box_length, int Box_width,float degInRad, hduVector3Dd EndPosition,hduMatrix WorldToDevice, HDstring EightTex, Box* Box_Array[], char* Name[], QHWin32* Display)
{
	for (int j = 0; j<12; j++)
	{
		Box_Array[j] = new Box(Box_width * 500 / 1024, Box_length * 500 / 1024, 0);
		//Clock of Diameter 14.4cm; Symmetrically placed; i.e. angle between each number is 30 Degrees
		Box_Array[j]->setTranslation(-EndPosition[0] - (72 * 500 / 1024 * cos(j * degInRad)*WorldToDevice[0][0] + 72 * 500 / 1024 * sin(j * degInRad)*WorldToDevice[1][0])
			, -EndPosition[1] - (72 * 500 / 1024 * cos(j * degInRad)*WorldToDevice[0][1] + 72 * 500 / 1024 * sin(j * degInRad)*WorldToDevice[1][1]), -EndPosition[2]);
		//Settings for the 12 Numbers/Characters of the	Clock
		Box_Array[j]->setTexture(EightTex);//Set a texture on it->setTranslation(1.5, 0.0, 0.0);//Set the position of the Cone
		Box_Array[j]->setGraphicVisibility(true);
		Box_Array[j]->setHapticVisibility(true);//Make the characters haptically activ 
		Box_Array[j]->setName(Name[j]);//SetName to the each of the 12 characters. Will be needed later to search for them and do the local Effect2_5 Effect
		Display->tell(Box_Array[j]);//tell Quickhaptics
	}
}
