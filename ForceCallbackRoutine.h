#pragma once
#include <QHHeadersWin32.h>
#include <HDU/hduMatrix.h>
#include <HL/hl.h>
#include <HDU/hduMatrix.h>
#include <HDU/hduError.h>
#include <HLU/hlu.h>
class ForceCallbackRoutine
{
public:
	void ForceCallbackRoutineFunction(int white_grey, int STOPPosForceUpdate, int Timer, int Ten,int TrialNumber, HDdouble force[3], HLcache *cache, void *userdata, bool ForcePresence, bool Headerwrite);
};
