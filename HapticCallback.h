#pragma once
#include <QHHeadersWin32.h>
#include <HDU/hduMatrix.h>
#include <HL/hl.h>
#include <HDU/hduMatrix.h>
#include <HDU/hduError.h>
#include <HLU/hlu.h>
#include <string.h>
class HapticCallback
{
public:
	int touchCallbackFunction(int white_grey,int TOUCH,static int Ten,int TrialNumber,char* Name[12],HDstring WhiteTex, HDstring GreyTex, string name, unsigned int ShapeID);
};