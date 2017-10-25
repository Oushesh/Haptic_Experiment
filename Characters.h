#pragma once
#include <QHHeadersWin32.h>
#include <HDU/hduMatrix.h>
#include <HL/hl.h>
#include <HDU/hduMatrix.h>
#include <HDU/hduError.h>
#include <HLU/hlu.h>
#include <string.h>
class Characters
{
public:
	void CharactersSetting(int Box_length, int Box_width,float degInRad, hduVector3Dd EndPosition, hduMatrix WorldToDevice, HDstring EightTex, Box* Box_Array[], char* Name[], QHWin32* Display);
};

