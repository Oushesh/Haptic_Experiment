//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Oushesh Haradhun
// External function reading the trial_array.bin and giving out 2x1 dimensional vector/array
// containing the current trial number as the 1st element of the vector (i.e. current trial number) and the 2nd element 
// being the ith position/index value of that element reached in the INTEGER VECTOR of 
// elements containing all the trial numbers shuffled up.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Read.h"
#include <iostream>
#include <math.h>
#include <cmath>
#include <iomanip> 
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <assert.h>
#include <functional>
#include <algorithm>
#include <string>

int Read_Trial(); // initialisation of the function Read_Trial()

vector<int> Read::Read_Function(int smallestInteger, int highestInteger,string filename, int ArraySize)
{
	std::vector<int> output(2); // two-dimensional vector
	output[0] = Read_Trial(); // Reads the current_trial number from an external file by using the function Read_Trial() and assigns the value to the local variable Current_Trial (you find the definition and an explanation of the function below)
	vector<int>vector; // Integer vector that will be used to read in the elements of Trial_Array (i.e., what does Trial_Array include, what is it for and where is it used?) ('vector' is not the best filename)
	RandomisedArray* Trial_instance = new RandomisedArray(); // Instantiation of Trial Pointer (you find the definition and an explanation of class Trial in Trial.h and Trial.cpp)
	//Trial_instance->Shuffle(1, 401, filename, ArraySize);
	Trial_instance->Shuffle(smallestInteger, highestInteger, filename, ArraySize);
	ifstream Read; // Declaration of ifstream. ifstream is the inputstream to read the content of the external file called trial_array.bin
	Read.open(filename); // Opens file called filename
	if (Read.is_open())
	{
		int number;
		//for (number = 0; number < 400; number++)
		for (number = smallestInteger-1; number < highestInteger-1; number++)
		{
			vector.push_back(number); // Saves all the elements of the file called filename into the vector called vector
		}
	}
	Read.close(); // Closes the file when reading and saving of the content of the file called filename is finished
	if (highestInteger < 400) //For the case we are dealing with "RandomisedTimeInterval.bin" whereby we have only 30 elements and 400 Trial Values; highest integer 89, lowest integer 58
	{
		double approx = (output[0] / 30);
		int intpart = (int)approx;
		output[0] = 58 + output[0] - (intpart * 30);
	}
	int j;
	for (j = 0; output[0] != vector[j]; j++);
	output[1] = vector[j];
	return output; //output [1]: returns the first element as the current Trial Number, second element gives the array element value
	//corresponding to the index where the Trial NUmber is found 
}
int Read_Trial()
{
	int trial_number;
	ifstream read_trial_log;
	read_trial_log.open("trial_number.txt"); //open log file
	if (read_trial_log.is_open())
	{
		read_trial_log >> trial_number; //read trial number from log file
		if (read_trial_log.eof())
		{
			trial_number = 0; //if log file is empty assign value 0 to trial_number
		}
		read_trial_log >> trial_number;
		read_trial_log.close();
	}
	// else cout << "Unable to open file";
	return trial_number;
}