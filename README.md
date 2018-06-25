# Haptic_Experiment

The Goal of this experiment was to build a setup to determine the delay between visual attention and finger movement.
In this Haptic Experiment Stimuli(identical characters) were presented onto the screen. 
Haptic Experiment whereby Stimuli are presented onto the screen for the user to see and a discriminative task defined
such that the USER has to locate the desired character(target) among others and use the Haptic Device(in this case Phantom 1.5) to reach the target. 
Several Frames presented for infinitesimal time periods in an attempt to test how fast does the brain can distinguish the target among others even before the user attempted a movement. This delay is actually timed from the moment the new picture frame appears to the time user uses the device's arm and successfully points to the character on the screen.


HARDWARE: 3D Systems Phantom Haptic Device 1.5
https://de.3dsystems.com/haptics-devices/3d-systems-phantom-premium
Languages: C++ 11

IDE USED: Visaul Studio 2015

Software Structure:
# Character.cpp
Initialises 3D Boxes with a customised image as texture on top of it positions the 12 characters displayed on the circumference of a circle

# CustomWindow.cpp
Opens a display window given the window Handle ID, window width & window heigth

# Force.cpp
Calculates the Force between the centre of the circle and the position of the Haptic Device End Effector modelled as Force resulting between 2 point charges in an "inverse squared field"

# ForceCallbackRoutine.cpp
Handles the presence of Force within different events during the Experiment

# HapticCallback.cpp
Handles the presence of Haptic Feeback(felt by the enduser when touching the EndEffector) within different events occurring during the Experiment

# Parameters.cpp
Takes in the parameters of the Experiment like time interval and occurrence time of different events in the course of the experiment and writes them as .csv file

# PosForceWrite.cpp
PosForceWrite Function: writes the Cursor Position + forceVector in 3D onto Log File
at each 10ms interval with the servoLopp Frequency at 1kHz + modified according
to accomodate the Format: Trial Number, Timing=10*number at 1kHz update Frequency, 
ProxyPosition, ForceVec


