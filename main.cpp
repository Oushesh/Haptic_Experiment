///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Oushesh Haradhun based on Prog. Guide, Prog. Reference and Open Haptic Examples for LMU Neuropsychological Experiments
// Program renders the frames as described in the file Phantom_Experiment_Procedure.pdf.
// See PDF for exeperiment Details. C:\OpenHaptics\Developer\3.4.0\Quickhaptics\examples\ComplexScene\PhantomExp\Phantom_Experiment_Procedure.pdf
// Additional parameters of the experiment are written in .txt/.csv file and then saved.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Including of external .h- and external .cpp-files
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "RandomisedArray.h" // 10.10.17: first revision (not for logical errors, if there are any)
#include "RandomisedArray.cpp" // 11.10.17: first revision (not for logical errors, if there are any)
#include "Randomise.h" // 10.10.17: first revision (not for logical errors, if there are any)
#include "Randomise.cpp"  // 10.10.17: first revision (not for logical errors, if there are any)
#include "Time_Measurement.h" // 10.10.17: first revision (not for logical errors, if there are any)
#include "Time_Measurement.h"
#include "Time_Measurement.cpp"
#include "PosForceWrite.h"
#include "PosForceWrite.cpp"
#include "Parameters.h"
#include "Parameters.cpp"
#include "Update.h"
#include "Update.cpp"
#include "Read.h"
#include "Read.cpp"
#include "RotationPosition.cpp"
#include "RotationPosition.h"
#include "Characters.h"
#include "Characters.cpp"
#include "HapticCallback.h"
#include "HapticCallback.cpp"
#include "Force.h"
#include "Force.cpp"

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

#include <QHHeadersWin32.h>
#include <QHHeadersGLUT.h>
#include <HDU/hduMatrix.h>
#include <HL/hl.h>
#include <HDU/hduMatrix.h>
#include <HDU/hduError.h>
#include <HLU/hlu.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class declarations
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DataTransportClass // This class contains the pointers to initialize geometric objects by means of the OpenHaptics Toolkit
{
public:
	Box* EndTip;  // Pointer towards a "box" that is presented at the centre of the screen (i.e., grey rectangle, white rectangle, movement cue)
	Box* pointing_plane; // Pointer towards the pointing plane (i.e., "Tischplatte")
	Box* E_3; // Pointer towards a "box" with a texture of a target (i.e., E or 3)
   //ANMERKUNG: Wenn du eine Box "E_3" deklarierst, könntest du auch die anderen Objekte z.B.für die 8, 2, 5 hier deklarieren und nicht weiter unten im Code ==> werden als dynamische Klasse weiter unten definiert
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function declaration
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int ShapeID; // see below
void touchCallback(unsigned int ShapeID); // Touch callback routine for the transition from grey rectangle to movement cue (what is a Touch callback routine? Please explain what the function does or tell the user where all the necessary information about the function can be found, e.g. at the definition of the function below)
void Transitions(void); // Function needed to implement the chronological sequence of events in the experimental procedure (white to grey rectangle to movement cue to 2/5 characters and 3/E Character to presentation of 8 characters)
void HLCALLBACK computeForceCB(HDdouble force[3], HLcache *cache, void *userdata);// Servo loop callback (MAIN CALLBACK ROUTINE as argument for startServoLoopCallback: z.314)
void HLCALLBACK startEffectCB(HLcache *cache, void *userdata);// Servo loop callback (startServoLoopCallback from OpenHaptics needs an argument for Effect Declaration to start before the main loop z.314)
void HLCALLBACK stopEffectCB(HLcache *cache, void *userdata);//  Servo loop callback (stopServoLoopCallback from OpenHaptics needs an argument for Effect Declaration to start after the main loop z.314)
int E_3UserConfirmation(int CharacterConfirmation);
int Instruction();
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Variable initialisation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Box_length; // Length of Box
int Box_width; // Width of Box
hduVector3Dd forceVec; // forceVec contains the force vector which is needed to save the force vector of the robot arm during the experiment continuously in an external file
float degInRad = 30 * 3.14159 / 180; // Degree to radians conversion in order to position 12 stimuli (i.e. 2s, 5s, 8s, E or 3) on an invisible circle with the same distance (i.e. every 360/12 degrees) to each other
hduMatrix WorldToDevice; // Transformation from world to device matrix (reference to programming guide?)
hduMatrix DeviceToWorld; // Transformation from device to world matrix (reference to programming guide?)
hduVector3Dd EndPosition(0.0, 0.0, 0.0); // Cursor position
int Rand_not_adjacent; // In the case 3/E does not appear at the movement Cue Position it appears at all the other places but not adjacent!!
int globalRand_Position; // Stores the global value of the randomised number where the movement cue has to rotate to be used later in function Effect2_5 (global value? value of a global variable?)
float DefaultTOMax = 500 / 1024; // WINAPI opens a window with 500 x 500 px; DefaultTOMax is rescaling the size as such that at max. size of 1024 we have the required size (required size of what?)
char* Name[12] = {"Box1","Box2","Box3","Box4","Box5","Box6","Box7","Box8","Box9","Box10","Box11","Box12"}; // Character Array with Box Names for each of the 12 Charac.
int Helper = 0; // Used as a helping variable to activate the delay for Effect2_5 only after the grey rectangle is replaced by the movement cue
int white_grey = 0; // Used as helping variable to activate change from white rectangle to grey rectangle
int Effect_8 = 0; // Used as helping variable to activate the frame of 8s
int TOUCH = 0; //  Comment missing
int Timer = 0; // Global int to control PosForceWrite; when to update
static int Ten = 0; // Control The writing for the Timer column in PosForceWrite.csv; Time=Ten*10
int Rand_3E; // Control whether 3/E appears at Cursor position or at other positions other than the neighbouring ones
int Position_Touched; // Global integer variable that stores the position of character pointed by the ENDUSER (as discussed, it is not necessary to store this information, but you can do it as additional information; additional information is always a good thing in experiments)
bool Headerwrite = 0; // Control when to write Column Header File
int STOPPosForceUpdate = -1; // Used in Parameters.cpp
int USERDISCRIMINATION= -1; // Used in Parameters.cpp


std::string name = "Parameters.csv"; // Name of file for Parameters .csv File
std::string filename = "trial_array.bin"; // Name of File for array of 400 shuffled trial number elements
std::string RandomiseIntervalFilenname = "RandomisedTimeInterval.bin"; // Name of File of array of 30 shuffled randomised time interval values (58 - 88, time intervall 580 - 880 ms)
std::string SIXHundredIntervalFilname = "SIXHundredInterval.bin"; // Name of File of array of 600 shuffled randomised time interval values (1-600, time intervall 0 - 600 ms)
bool ForcePresence; // Boolian Variable to control if a force in a specific trial is present or not; the variable is used in the force callback function of the Openhaptics Toolkit to control the presence of perturbation force
int ArraySizeTrial = 400; // Argument to be fed in the Trial.cpp function representing the size of the array of shuffled array elements
int ArraySizeRandomiseTimeInterval = 30; // steps in ms of the time intervall
int ArraySizeSIXHundredTimeInterval = 600; // size of time interval 1-600

HDstring WhiteTex("models/textures/button_white_circle.png"); // This variable stores an external .jpg file as a texture for a virtual object of the OpenHapticsToolkit (i.e., a 'box'). Boxes (with a depth of 0 px) are used in this program to present 2D stimuli in 3D space.
HDstring GreyTex("models/textures/button_grey_circle.png"); // This variable stores an external .jpg file as a texture for a virtual object of the OpenHapticsToolkit (i.e., a 'box'). Boxes (with a depth of 0 px) are used in this program to present 2D stimuli in 3D space.
HDstring ArrowTex("models/textures/movement_cue.png"); // This variable stores an external .jpg file as a texture of for virtual object of the OpenHapticsToolkit (i.e., a 'box'). Boxes (with a depth of 0 px) are used in this program to present 2D stimuli in 3D space.
HDstring EightTex("models/textures/8.png"); //This variable stores external path of the 8.png
HDstring TwoTex("models/textures/2.png");
HDstring FiveTex("models/textures/5.png");
HDstring ETex("models/textures/E.png");
HDstring ThreeTex("models/textures/3.png");
HDstring PointingPlaneTex("models/textures/pointing_plane.png");
HDstring StartTex("models/start.jpg");
Box* gStartButton = NULL;

Randomise* Randomise_instance = new Randomise();

Read* Read_instance = new Read();
vector<int>output(Read_instance->Read_Function(1, 401,filename, ArraySizeTrial)); // Calling function to read randomised array of trial numbers as integer vector array (i.e., 400 elements of array read by function read)

Read* RandomiseIntervalinstance = new Read();  
vector<int> OutputRandomisedInterval(RandomiseIntervalinstance->Read_Function(58, 89,RandomiseIntervalFilenname, ArraySizeRandomiseTimeInterval)); // OutputRandomisedInterval contains the randomised time interval shuffled (i.e., randomised time intervall 580-880 ms)

Read* SIXHundredTimeIntervalInstance = new Read();
vector<int> SIXHundredTimeInterval(SIXHundredTimeIntervalInstance->Read_Function(1, 601, SIXHundredIntervalFilname, ArraySizeSIXHundredTimeInterval)); // OutputRandomisedInterval contains the randomised time interval shuffled (i.e., randomised time intervall 1-600 ms)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main function
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE	hInstance, // Instance
	HINSTANCE	hPrevInstance,             // Previous Instance
	LPSTR		lpCmdLine,				   // Command Line Parameters
	int		nCmdShow)				       // Window Show State
{	  
				  
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Here: Call of new external function "empty window"
	// Erstellen einer externen Funktion „empty_window“ in neuem .h & .cpp-file:
	// Die externe Funktion „empty_window“ ist für alle Einstellungen des Fensters zuständig (Größe des Fensters : 1024 x 768 Pixel; Hintergrundfarbe: grau; Vollbild)
	// Beim Start des C++ - Programms soll diese Funktion in WinMain in main.cpp aufgerufen werden und damit das Fenster geöffnet werden		  
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Here: Prompt experimenter: Specifications of the experiment (resolution of the screen, visual angle etc. (or: read external txt-file with specifications)
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Here: Prompt participants (keyboard): demographic variables (age, sex, handiness etc.), Versuchspersonennummer
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Here: Instructions: 1. slide with text; user continues with space bar
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Here: Instructions: 2. slide with text; user starts experiment with space bar
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Initialisation of the window and Phantom
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	QHWin32* DisplayObject = new QHWin32; // This creates a the display window
	DataTransportClass* dataObject = new DataTransportClass(); // Initializes an object to transport data into the servoloop callback of the Phantom (instantiation of DataTransportClass? please explain it a little more detailed)
	DeviceSpace* Omni = new DeviceSpace; // Phantom device called Omni
	DisplayObject->setBackgroundColor(0.50, 0.50, 0.50); // Colour gray in RGB channel
	DisplayObject->setName("Experiment");
	DisplayObject->setWindowTitle("Experiment");
	DisplayObject->tell(Omni); // Tell Quickhaptics that Omni exists

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Instruction Text (TO FURTHER EXTEND)
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Text* text = new Text(10.0, "Instruction1", 0.25, 0.9);
	text->setName("Instructions");
	DisplayObject->tell(text);// Tell QuickHaptics that the text has been created
	Text* text2 = new Text(10.0, "Instruction2", 0.25, 0.9);
	text2->setName("Instructions2");
	text2->setGraphicVisibility(false);
	DisplayObject->tell(text2);// Tell QuickHaptics that the text has been created


	Cursor* OmniCursor = new Cursor;// Declare a cursor
	OmniCursor->setName("devCursor");// Give Cursor a Name
	DisplayObject->tell(OmniCursor);// Tell QuickHaptics that the cursor exists

	WorldToDevice = DisplayObject->getWorldToDeviceTransform(); //get Transformation from World Coordinate System to Phantom Coordinate System
	DeviceToWorld = WorldToDevice.getInverse(); //get Transformation from Phantom Coordinate System to World Coordinate System
	
	int EndTip_length = 15 * 500 / (1024 * 0.7875) *WorldToDevice[0][0] + 25 * 500 / (1024 * 0.7875)*WorldToDevice[1][0]; // settings of the central objects EndTip
	dataObject->EndTip = new Box(EndTip_length, EndTip_length, 0.5); // Depth is important such that we have a 3D Object not intersecting the Pointing plane
	dataObject->EndTip->setName("EndTip");// Give it a name
	dataObject->EndTip->setTranslation(-EndPosition); // Position the EndTip in the middle
	dataObject->EndTip->setTexture(WhiteTex); //Set a texture on it; black arrow picture
	dataObject->EndTip->setHapticVisibility(true); // If set FALSE then the Event of changing from White Cross to Grey Cross CANNOT HAPPEN
	DisplayObject->tell(dataObject->EndTip); // tell Quickhaptics

	Box_width = 9 * WorldToDevice[0][0] + 14 * WorldToDevice[1][0]; // Use world to Device Transformation matrix to do elementwise multiplication to get the Box_width corresponding to 0.9 cm
	Box_length = 9 * WorldToDevice[0][1] + 14 * WorldToDevice[1][1]; // Use world to Device Transformation matrix to do elementwise multiplication to get the Box_length corresponding to 1.4 cm
	dataObject->E_3 = new Box(Box_width * 500 / 1024, Box_length * 500 / 1024, 0); // Initialise Box for element E or 3	
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	dataObject->pointing_plane = new Box(144, 144, 0); //initialise Box for Sitmulus Feedback plane
	dataObject->pointing_plane->setFriction(); //setFriction to plane ; Default Value 0.5
	dataObject->pointing_plane->setTexture(PointingPlaneTex);
	dataObject->pointing_plane->setGraphicVisibility(true); //set to invisible such that the EndTip appears
	// 09.10.2017 // dataObject->pointing_plane>setHapticVisibility(true);
	DisplayObject->tell(dataObject->pointing_plane);//Tell Quickhaptics that Sphere exists
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 1.For each 12 elements Declare empty Object Box instance and assign to Box pointer 
	// 2.Then pointer elements to each of the 12 Objects in another array
	// 3. Iterative initialisation of the 12 elements and each of their settings
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Box* ClockNumber1 = new Box(Box_width, Box_length, 0);
	Box* ClockNumber2 = new Box(Box_width, Box_length, 0);
	Box* ClockNumber3 = new Box(Box_width, Box_length, 0);
    Box* ClockNumber4 = new Box(Box_width, Box_length, 0);
	Box* ClockNumber5 = new Box(Box_width, Box_length, 0);
	Box* ClockNumber6 = new Box(Box_width, Box_length, 0);
	Box* ClockNumber7 = new Box(Box_width, Box_length, 0);
	Box* ClockNumber8 = new Box(Box_width, Box_length, 0);
	Box* ClockNumber9 = new Box(Box_width, Box_length, 0);
	Box* ClockNumber10 = new Box(Box_width, Box_length, 0);
	Box* ClockNumber11 = new Box(Box_width, Box_length, 0);
	Box* ClockNumber12 = new Box(Box_width, Box_length, 0);
	
	Box* Box_Array[12] = {ClockNumber1,ClockNumber2,ClockNumber3,ClockNumber4,ClockNumber5,ClockNumber6,ClockNumber7,ClockNumber8,ClockNumber9,ClockNumber10,ClockNumber11,ClockNumber12};

	Characters* CharactersSettingInstance = new Characters(); // settings of boxes of Box_Array, see Characters.cpp
	CharactersSettingInstance->CharactersSetting(Box_length,Box_width,degInRad, EndPosition,WorldToDevice,EightTex, Box_Array,Name, DisplayObject);

	RotationPosition* RotationPositionInstance = new RotationPosition(); // returns the movement cue position (i.e. 1-12), presents E/3 in dependence of case 0/1 of the function RotationPosition, see RotationPosition.cpp: 
	globalRand_Position =RotationPositionInstance->Rotate(EightTex, WorldToDevice, Rand_3E, degInRad, EndPosition, dataObject->E_3, DisplayObject);
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Look for the current trial number value in the vector array of shuffled trial numbers
	// and output its index. If index >=201 out of 400 there is perturbation force otherwise not
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	(output[1] >= 201) ? (ForcePresence = 0) : (ForcePresence = 1);
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// OpenHaptics Callback functions; 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Omni->startServoLoopCallback(startEffectCB, computeForceCB, stopEffectCB, &dataObject); // Register the servoloop callback; The first argument starts the ServoLoop, the second the main Servoloop, and the third stops the Servoloop
	//Omni->touchCallback(touchCallback, dataObject->EndTip); // touchCallback for the transition from white button to grey button
	DisplayObject->preDrawCallback(Transitions); // grafische CallbackLoop; Transitions is defined below
	Omni->touchCallback(touchCallback, gStartButton);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Old option to save the clock position that is touched by the cursor
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i<12; i++)
	{
		//Omni->button1UpCallback(button1UpCallback, Box_Array[i]);//touchCallback for the 12 Characters
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// For a definition of Update, see Update.h and Update.cpp;
	// By calling Update with the trial number (= output[0]) as an argument, the trial number will be increased by 1 and written in the file Parameters.csv
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Update* Update_instance = new Update();
	Update_instance->UpdateFunction(output[0]); delete Update_instance;
	//Update_instance->UpdateFunction(Current_Trial); delete Update_instance;
	
	Instruction(); //Dialogbox for Instruction. TO IMPROVE FURTHER

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Maximise the window
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	HWND hwnd = FindWindow(NULL, (LPCWSTR)"Experiment"); // Finding window handle with the window name: "Experiment"
	// HWND hwnd = GetForegroundWindow(); // This line is an alternative. TODO: SHIFTING contents from left bottom edge to center of display WINAPI32 Window (hwnd = hwindowhandle)
	ShowWindow(hwnd, SW_SHOWMAXIMIZED);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// OpenHaptics: start everything
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	qhStart();//Set everything in motion
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Servo loop thread callback. Computes a force effect between the current cursor position ("real-time" Proxy position) and the center position in device space (1khz HD servo loop).
// Every 10th time when the force is computed (= 10 ms), the force vector is saved in PosForceWrite.csv by using the class/function PosForceWrite.cpp.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HLCALLBACK computeForceCB(HDdouble force[3], HLcache *cache, void *userdata)
{
	if (white_grey == 0 || STOPPosForceUpdate == 0) // Force only comes into the event as from the second frame (= after the movement cue occurs) until the trial has ended (= until the participant presses E/3).
		return;
	hduVector3Dd centreposition(0.0, 0.0, 0.0); // Declaration of centre position 
	hduVector3Dd proxyPosition; // Position of the proxy/cursor in device space
	static int counter1 = 0; // see below
	hlCacheGetDoublev(cache, HL_PROXY_POSITION, proxyPosition); // Get the position of the proxy in Device Coordinates 												
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Determining whether External Perturbating Force should be present or not!
	// Only for the First 200 elements would the Force be present
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Instantiation of class Force (computes/starts the force in dependence of the difference between the proxyPosition and centreposition; It also assigns the current force to force vec)
	Force* Force_instance = new Force();
	
	switch (ForcePresence)
	{
	case 0: // no force
			forceVec = Force_instance->forceField(proxyPosition, centreposition, 10.0, 100.0);//force between the 2 positions;100-Electrostatic constant,10-Tolerance Radius
			delete Force_instance; // optional to free memory
			break;
	case 1: // force present
		forceVec = hduVector3Dd(0.0, 0.0, 0.0);
		break;
	}
	
	// Timer will be updated everytime the function will be called from moving the cursor to the white button until E/3 is pressed (= STOPPosForceUpdate == 0)
	if (white_grey > 0 && STOPPosForceUpdate != 0) //only write proxyposition+Force to .csv file once in 10ms and only during Transition Phase
	{
		if (Timer <= 10)
		{
			Timer += 1;
		}
		else
		{
			Timer = 0; Ten += 1;
			PosForceWrite* PosForceWrite_Instance = new PosForceWrite(); //new instance of the class PosForceWrite to call function for .csv file 
			int TrialSet=1;
			PosForceWrite_Instance->PosForceWriteFunction(TrialSet,Headerwrite,output[0], Ten, proxyPosition, forceVec); //Takes in Argument ProxyPosition as hduVector3Dd
			Headerwrite = 1;
			delete PosForceWrite_Instance; //destructor when instance is not used
		}
	}
	else
		Ten = 0;
	
	counter1++;
	
	if (counter1>2000) // Make the force start after 2 seconds of program start. This is because the servo loop thread executes before the graphics thread. 
	{
		// The force vector is assinged to forceVec
		force[0] = forceVec[0];
		force[1] = forceVec[1];
		force[2] = forceVec[2];
		counter1 = 2001;
	}
	else
	{
		force[0] = 0.0;
		force[1] = 0.0;
		force[2] = 0.0;
	}
}

void HLCALLBACK startEffectCB(HLcache *cache, void *userdata){} // see above: "Omni->startServoLoopCallback(startEffectCB, computeForceCB, stopEffectCB, &dataObject);"
void HLCALLBACK stopEffectCB(HLcache *cache, void *userdata){} // see above: "Omni->startServoLoopCallback(startEffectCB, computeForceCB, stopEffectCB, &dataObject);"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Servo loop thread callback called when white button is touched: Pointer, der zentrales Objekt repräsentiert (i.e., EndTip) wird zurückgegeben, wenn Endeffekter (i.e., DevCursor) das zentrale Objekt berührt
// see above: "Omni->touchCallback(touchCallback, dataObject->EndTip);"
// When the white button is touched, HapticCallback is called (= loading a new texture, grey button, saving the time of the event, returns "touchcontrol" (= only 1 when event occured)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void touchCallback(unsigned int ShapeID)
{
	HapticCallback* HapticCallback_Instance = new HapticCallback();
	int TransitionControl = HapticCallback_Instance->touchCallbackFunction(white_grey, TOUCH, Ten, output[0], Name, WhiteTex, GreyTex, name, ShapeID);
	if (TransitionControl == -1){return;} 
	else{white_grey =TransitionControl;}
	delete HapticCallback_Instance;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Servo loop thread callback called for The Transition Effects 
//30 Hz QuickHaptics graphics loop
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Transitions(void)
{
	QHWin32* localDisplayObject = QHWin32::searchWindow("Experiment");//Get a Pointer to the display object
	int SIXHundredTimeStamp = SIXHundredTimeInterval[1]; // = randomised number of time intervall 0-600 (0-60) ms in 10 ms steps.
	int TimeStamp = OutputRandomisedInterval[1]*10; // = randomised number of time intervall 580-880 ms (58-88) in 10 ms steps.

	if (white_grey > 4 && white_grey <= 148) // white_grey > 4 == probe display was already displayed; loops 147 times, which is about 80 ms
	{
		white_grey += 1; Helper += 1;
		return;
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// If QHWin32 is found and white button is not touched, nothing happens.
	// Otherwise: see below (before every if)
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (localDisplayObject == NULL || white_grey == 0)
		return;
	else
	{
			Box* BlackArrow = Box::searchBox("EndTip"); // Get a local pointer to the cursor on which the cross appears
			if (BlackArrow == NULL) //id the pointer is NULl then return
				return;
			else
			{
		
				if (white_grey == 2) // loads movement cue
				{
					Time_Measurement* Timing_instance = new Time_Measurement();
					for (int timer = 0; timer < TimeStamp; timer++) //TimeStamp returns 10ms Factor value to get the Timestamp 580-880ms
					{
						
						Timing_instance->Time_function();
					}
					delete Timing_instance;
					//Effect Rotate Movementcue
					BlackArrow->setRotation(hduVector3Dd(0.0, 0.0, 1.0), globalRand_Position*30.0);// Rotation of normal/orthogonal vector to XY plane; i.e. rotating about the z-unit vector (i.e., clock position shown by movement cue)
					BlackArrow->setTexture(ArrowTex);
				}
				white_grey += 1; Helper += 1; // Update the Variables to coordinate the Transitions

				if (white_grey == 4 && Helper == 3) // loads probe display
				{
					//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Effect2_5
					//After Testing with Breakpoint at Helper_2 approx. 600 ms is gone with value of timer set at 3000000
					//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					int lower_limit = 0;
					int upper_limit = 2;
					
					Time_Measurement* Timing_instance = new Time_Measurement();
					for (int timer = 0; timer < SIXHundredTimeStamp; timer++)
					{
						Timing_instance->Time_function();	
					}
					delete Timing_instance;
					
					//Simultaneously, the remaining 11 mask characters were replaced with equal(50%) prob. by 1 of 2 kinds of distractors (either 2 or 5)
					srand(time(NULL)); // initialise random seed
					for (int idx = 0; idx < 12; idx++) // Loops for every 12 characters (the position where the E/3 is displayed is replaced later by the texture of E/3)
					{
						Box* Box2_5 = Box::searchBox(Name[idx]); //Search the Box with name corresponding to the element in the Name Array defined at the beginning
						//if (Box2_5 == NULL)//The pointer is NULl then return back. The pointer was not found
							//return;
						int Rand_2_5 = Randomise_instance->function_randomise(lower_limit, upper_limit);//Call the randomise function
						switch (Rand_2_5) //switch between the 2 values of Rand_2_5;either 0 or 1
						{
						case 0: //where 2 has to appear
							Box2_5->setTexture(TwoTex);
							break;
						case 1:
							Box2_5->setTexture(FiveTex);
							break; //where 5 has to appear
						}
					}
					//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Effect 3_E Presentation of the Character 3 or E
					//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					Box* Find_E_3 = Box::searchBox("Number_3E");//get a local pointer to the Cursor on which the cross appears; movement Cue
					//if (Find_E_3 == NULL)//if the pointer is NULL then return
						//return;
					lower_limit = 0;
					upper_limit = 2;
					srand(time(NULL));
					int E_or_3 = Randomise_instance->function_randomise(lower_limit, upper_limit); //calling Randomise function to decide whether E or 3 has to be presented
					(E_or_3 == 0) ? Find_E_3->setTexture(ThreeTex) : Find_E_3->setTexture(ETex);
					Effect_8 += 1; //Update Effect_8 by 1 to start it afterwards
				}

				// INCREASE values in the following condition to allow user to get time to move to the destination with the cue
				if (white_grey == 150 && Helper == 149 && Effect_8 == 1)
				{
					//Update the characters to 8 after Delay 80ms
					/*
					for (int timer = 0; timer <8; timer++)
					{
						Time_Measurement* Timing_instance = new Time_Measurement();
						Timing_instance->Time_function();
					}
					*/

					// Find position of 3/E and replace it by 8
					Box* ResetE_3TO_8 = Box::searchBox("Number_3E");//For the last Frame; needs to reset the E_3 box as well to 8 
					//if (ResetE_3TO_8 == NULL)//if the pointer is NULl then return
						//return;
					ResetE_3TO_8->setTexture(EightTex);
					
					// Find position of 2s/5s and replace it by 8
					for (int idx = 0; idx < 12; idx++)
					{
						Box* Mask = Box::searchBox(Name[idx]);//get a local pointer to each of the 12 Characters
						//if (Mask == NULL)//if the pointer is NULl then return
							//return;
						Mask->setTexture(EightTex); //Set the Texture for the Last Frame to 8
					}

					//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Event Update in Parameters.csv; state of variables changes every time is saved
					//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					Parameters* Parameters_Instance = new Parameters(); //new instance of the class PosForceWrite to call function for .csv file
					int TrialSet=1; //TrialSet gives the nth set of 400 Trials which the user has to input
					Parameters_Instance->ParameterOutput(USERDISCRIMINATION /* button press 3/E */
						,TimeStamp  /* randomised time between 580-880 */
						, SIXHundredTimeStamp /* randomised time between 0-600 */
						,TrialSet /* = 1 for the first 400 trials, = 2 for the next 400 trials etc. */
						,Ten /* int timer increased every 10 ms */
						, Effect_8 /* = 0 before event occurs, = 1 after event occurs */
						, Helper /* not necessary anymore */
						, white_grey /* = 0 before event occurs, = 1 after event occurs */
						, output[0] /* current trial number */
						, 12 - globalRand_Position /* randomised number between 0-11 of the movement cue aim on the clock  */
						, Rand_3E /* see RotationPosition.cpp */
						, 12 - Rand_not_adjacent); /* aim of the movement cue */

					delete Parameters_Instance; delete Read_instance; delete RandomiseIntervalinstance; delete SIXHundredTimeIntervalInstance; // destructor when instance is not used
				}

				if (white_grey == 152 && Helper == 151 && Effect_8 == 1) // discrimination task
				{
					USERDISCRIMINATION = E_3UserConfirmation(USERDISCRIMINATION); // see function definition below
					//Reset white_grey and Helper to their original Values to completely Transitiion Effects + Effect_8 to 1 to the Transition to Last Frame
					white_grey = 0; Helper = 0; Effect_8 = 0; TOUCH = 1; STOPPosForceUpdate = 0;
				}
			}

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Event Update in Parameters.csv to update the header of the file
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			Parameters* Parameters_Instance = new Parameters(); //new instance of the class PosForceWrite to call function for .csv file
			int TrialSet=1; //TrialSet gives the nth set of 400 Trials which the user has to input
			Parameters_Instance->ParameterOutput(USERDISCRIMINATION,TimeStamp, SIXHundredTimeStamp, TrialSet,Ten, Effect_8, Helper, white_grey, output[0], 12 - globalRand_Position, Rand_3E, 12 - Rand_not_adjacent);
			delete Parameters_Instance; //destructor when instance is not used
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Touch Callback Function to know which Character was touched by 
// ENDUSER return the posiion of the Character 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void button1UpCallback(unsigned int BoxID) // optional; returns on which number the user has pointed
{
	for (int BoxNumber = 0; BoxNumber < 12; BoxNumber++)
	{
		Box* BoxTouched = Box::searchBox(Name[BoxNumber]);//get a local pointer to the Box Touched by user
		if (BoxTouched == NULL && white_grey == 4 && Helper == 3)//if the pointer isdr Null then return
			return;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Discrimination task
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int E_3UserConfirmation(int CharacterConfirmation)
{
	int msgboxID = MessageBox(
		NULL,
		L"Click on JA for E\n"
		L"Click on NEIN for 3",
		L"",
		MB_ICONEXCLAMATION | MB_YESNO);
	if (msgboxID == IDYES)
	{
		CharacterConfirmation = 1;
	}
	else
	{
		CharacterConfirmation = 0;
	}
	return CharacterConfirmation;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Instructions at the beginning of the experiment 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Instruction()
{
	int InstructionID;
	Text* SearchedText = Text::searchText("Instructions");
	Text* SearchedText2 = Text::searchText("Instructions2");
	if (SearchedText == NULL || SearchedText2==NULL)
		return -1;
	
	int msgboxID = MessageBox(
		NULL,
		L"Instruction 1\n"
		L"Click on YES to see further",
		L"",
		MB_ICONEXCLAMATION | MB_YESNO);
	if (msgboxID == IDYES)
	{
		InstructionID = 1;
		SearchedText->setGraphicVisibility(false);
		SearchedText2->setGraphicVisibility(true);
	}
	else
	{
		InstructionID = 0;
		SearchedText->setGraphicVisibility(true);
		SearchedText2->setGraphicVisibility(false);
	}
	return InstructionID;
}
//Rotation of Workspace with hlufitworkspace()
//Position of character 1-12 user reached with cursor
//Reference to Guide where Information were taken; Refer to Code Lines: which part of code does what in the Experiment Procedure (DOCUMENTATION)

// TODO 09.10.17: Kraft erst ab 2. Frame (i.e., when the movement cue is presented)
// TODO 09.10.17: Enable the haptic feedback of the pointing plane again
// TODO 10.10.17: Rename variable, arguments, function names as named in the method part of the text (Phantom Paper)
// TODO 12.10.17: "Helper" nicht notwendig