//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Oushesh Haradhun Randomise.cpp
// Reference http://www.cplusplus.com/reference/cstdlib/rand/
// Function is able to randomise integer numbers between 1 and 12 corresponding to the positions of the characters in the clock
// Function will be called in the main main.cpp and Randomise.h has to be written
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Randomise.h"
#include <iostream>
#include <random>
#include <stdlib.h>
#include <time.h>

int Randomise::function_randomise(int lower_limit, int upper_limit)
{
	//srand(time(NULL)); // Initialise Random Seed => srand is used in other cpp-files. srand is needed so that every time function_randomise is called a new randomised number is generated
	return rand() % upper_limit + lower_limit;
}



