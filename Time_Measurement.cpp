/***********************************************************************************
/*Author: Oushesh Haradhun
/************************************************************************************
Code to measure the time in Microseconds
************************************************************************************/
#include "Time_Measurement.h"
#include <iostream>
#include "stdio.h"
#include "Windows.h"

using namespace std;

//LARGE_INTEGER Time_Measurement::Time_function()
__int64 Time_Measurement::Time_function()
{
	LARGE_INTEGER Frequency;  // ticks per second
	LARGE_INTEGER StartingTime, EndingTime;  // ticks
	__int64  ElapsedMicrosec;
	//double elapsedTime;
	static int counter = 0;
	// get ticks per second
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartingTime);
	//Activity to be Timed
	while (counter <= 1500000) //Tested and counter adjusted such that the value between starting and ending couter is ~10ms
	{
		counter += 1;
	}
	QueryPerformanceCounter(&EndingTime);
	//ElapsedMicrosec.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
	ElapsedMicrosec = EndingTime.QuadPart - StartingTime.QuadPart;
	// elapsed number of ticks, along with the
	// number of ticks-per-second. We use these values
	// to convert to the number of elapsed microseconds.
	// To guard against loss-of-precision, we convert
	// to microseconds *before* dividing by ticks-per-second.
	//ElapsedMicrosec.QuadPart *= 1000000;
	//ElapsedMicrosec.QuadPart /= Frequency.QuadPart;
	ElapsedMicrosec *= 1000000;
	ElapsedMicrosec /= Frequency.QuadPart;
	return ElapsedMicrosec;
}
