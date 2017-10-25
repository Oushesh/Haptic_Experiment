/*************************************************************************************
Author: Oushesh Haradhun
Function to take value returned from Read_Trial and updates the number; increase it by 1
An update implies program did not crash while executing main
/**************************************************************************************/
#include "Update.h"
string UpdateFilename(int Trial_number);
void Update::UpdateFunction(int trial_number)
{
	if (trial_number > 400)
	{
		trial_number = 1; //of trial number is greater than 400, 1 set is completed; reset trial_number to 1
		remove("RandomisedTimeInterval.bin"); //Delete the set of Randomise Time Intervals... for new 400 experiment trials will begin and the set is rerandomised and recreated
		remove("trial_array.bin"); //Delete the set of Randomise Time Intervals... for new 400 experiment trials will begin and the set is rerandomised and recreated
	}
	else
		trial_number += 1; //increase Trial number by 1

	ofstream trial_log;
	trial_log.open("trial_number.txt"); //open the log file .txt
	trial_log << trial_number << "\n";//inserts a sentence into it in the same way we are used to do with cout, but using the file stream myfile instead
	trial_log.close(); //close log File

	ofstream DataUpdate;
	string CurrentFilename = UpdateFilename(trial_number);
	DataUpdate.open(CurrentFilename.c_str(), ofstream::app); //open file in append mode
															 //DataUpdate.open("PosForceWrite.csv", ofstream::app);
	DataUpdate << trial_number << ";" << "th Trial Ended." << ";" <<
		trial_number << ";" << "th Trial Ended." << ";" <<
		trial_number << ";" << "th Trial Ended." << ";" <<
		trial_number << ";" << "th Trial Ended." << ";" <<
		trial_number << ";" << "th Trial Ended." << ";" << "\n"; //Text Denoting Current Trial Ended
	DataUpdate.close();		
}
//Function which returns the name of the .csv file depending on the Current_Trial Number
//Note: This function could be written otherwise with fastformat libraries which
//are unavailable for VS 2010

