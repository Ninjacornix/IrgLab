#pragma once

#include <sstream>
#include <iostream>
#include <chrono>
#include <thread>

//#include "Helper.h"
#ifndef __glfw_h_
	#include "GLFW/glfw3.h" // We need GLFW for this, so let's check for it- although it'd be a doddle to convert to non-GLFW using code.
#endif


class FPSManager
{

private:
	GLFWwindow *window;

	double frameStartTime;         // Frame start time
	double frameEndTime;           // Frame end time
	double frameDuration;          // How many milliseconds between the last frame and this frame

	double targetFps;              // The desired FPS to run at (i.e. maxFPS)
	double currentFps;             // The current FPS value
	int    frameCount;             // How many frames have been drawn since last update
	int	   totalFrameCount;        // How many frames have been drawn since the program start

	double targetFrameDuration;    // How many milliseconds each frame should take to hit a target FPS value (i.e. 60fps = 1.0 / 60 = 0.016ms)
	double sleepDuration;          // How long to sleep if we're exceeding the target frame rate duration

	double lastReportTime;         // The timestamp of when we last reported
	double reportInterval;         // How often to update the FPS value

	std::string windowTitle;       // Window title to update view GLFW

	bool verbose;                  // Whether or not to output FPS details to the console or update the window

								   // Limit the minimum and maximum target FPS value to relatively sane values
	static const double MIN_TARGET_FPS;
	static const double MAX_TARGET_FPS; // If you set this above the refresh of your monitor and enable VSync it'll break! Be aware!

											   // Private method to set relatively sane defaults. Called by constructors before overwriting with more specific values as required.
	void init(GLFWwindow *Window, double theTargetFps, bool theVerboseSetting);

public:

	// Single parameter constructor - just set a desired framerate and let it go.
	// Note: No FPS reporting by default, although you can turn it on or off later with the setVerbose(true/false) method
	FPSManager(GLFWwindow *window, int theTargetFps);
	
	// Two parameter constructor which sets a desired framerate and a reporting interval in seconds
	FPSManager(GLFWwindow *window, int theTargetFps, double theReportInterval);
	
	// Three parameter constructor which sets a desired framerate, how often to report, and the window title to append the FPS to
	FPSManager(GLFWwindow *window, int theTargetFps, float theReportInterval, std::string theWindowTitle);
	

	// Getter and setter for the verbose property
	bool getVerbose();
	void setVerbose(bool theVerboseValue);

	// Getter and setter for the targetFps property
	int getTargetFps();
	void setTargetFps(int theFpsLimit);

	// Returns the time it took to complete the last frame in milliseconds
	double getFrameDuration(); 

	// Setter for the report interval (how often the FPS is reported) - santises input.
	void setReportInterval(float theReportInterval);
	
	// Method to force our application to stick to a given frame rate and return how long it took to process a frame
	double enforceFPS(bool shouldSleep);
	
	int getFrameCount();

};