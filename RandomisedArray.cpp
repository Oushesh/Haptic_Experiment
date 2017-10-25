/***********************************************************************************
Author: Oushesh Haradhun
/************************************************************************************
Reference: https://stackoverflow.com/questions/19338702/random-shuffle-using-vector
Random Generated List from trial_array.bin sorted out for testing to check for probable 
repetitive numbers http://sortmylist.com/
For Random Trial Number Generation
for 400 cases whereby 200 random Generated Trial Numbers would not 
have external Fore Effect and the other remaining ones would
************************************************************************************/
#include "RandomisedArray.h"
#include "stdio.h"
#include "Windows.h"
#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include<time.h>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <functional>
#include <algorithm>

using namespace std;

int RandomDigit(int i);
void RandomisedArray::Shuffle(int smallestInteger, int highestInteger,string filename, int ArraySize)
{
	streampos begin, end;
	vector<int> Trial_Numbers; //Create a Dynamic Array to hold the values to be read afterwards in case "trial_array.txt" is available
	vector<int> Read_Numbers;
	ifstream Trial_Array_Read(filename, ios::binary);
	begin = Trial_Array_Read.tellg();
	Trial_Array_Read.seekg(0, ios::end);
	end = Trial_Array_Read.tellg();
	cout << end - begin << "bytes";
	if ((begin - end) == 0)//if the file is of 0bytes it means its empty
	{
		ofstream fs;
		fs.open(filename); // create and open the .csv file
		for (int i = smallestInteger; i < highestInteger; ++i)
		{
			Trial_Numbers.push_back(i);
			random_shuffle(Trial_Numbers.begin(), Trial_Numbers.end());
			random_shuffle(Trial_Numbers.begin(), Trial_Numbers.end(), RandomDigit);
		}
		if (fs.is_open())
		{
			for (int i = 0; i < Trial_Numbers.size(); ++i)
			{
				fs << Trial_Numbers[i] << '\t' << std::endl;
			}
			fs.close();
			std::cout << filename << "Array Created" << '\n';
		}
	}
}
int RandomDigit(int i)
{
	srand(time(NULL)); //not to forget to reset the Time Seed. Otherwise each time the Generated Numbers will be exactly the same
	return rand() % i; //generate random  number
}
//This Function will also be used to create a randomised array in the case of the time iNtervals between (580-880)ms in step of 10ms
//Argument: Number of Array elements: 30; stringfilename generated: randomised_time_interval.bin
//Argument to be taken also for i: smallestinteger=1 (case of trial_array.bin) =58(case of Randomised integer) , highestinteger=401 (case of trial_array.bin),
//=88(case of randomised integer)
string UpdateFilename(int Trial_number)
{
	std::string name = "PosForceWrite.csv";
	std::string result;
	char numstr[21]; // enough to hold all numbers up to 64-bits
	sprintf(numstr, "%d", Trial_number);
	result = numstr + name;
	return result;
}