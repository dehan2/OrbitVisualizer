#pragma once

#include <ctime>
#include <cwchar>
#include "coreLib.h"
#include "orbitLib.h"
#include "rg_Point3D.h"
#include "Color3f.h"

#define DISABLE_COLLISION
#define CHECK_TLE

//enum ORBIT_SOURCE_TYPE { NONE, TLE };

//All parameters uses km for length, kg for mass, and second for time unit.

//const float GRAVITY_G = 6.6742E-11; // gravitational constant https://en.wikipedia.org/wiki/Gravitational_constant
//const float GRAVITY_M = 5.9722E24; // mass of the earth https://en.wikipedia.org/wiki/Earth_mass
const float GRAVITY_MU = 3.986E5;	//G*M, standard gravitational parameter https://en.wikipedia.org/wiki/Standard_gravitational_parameter

//const cJulian SIMULATION_EPOCH(2018, 3, 22, 0, 0, 0); //00:00:00 22. Mar. 2018 
//const cJulian SIMULATION_EPOCH(2018, 7, 12, 0, 0, 0); 
//const cJulian SIMULATION_EPOCH(2018, 7, 20, 6, 0, 0); 

const double SATELLITE_SIZE = 1.0E-10;
const int BASIC_NUM_APPROXIMATION =5;
const int MAX_APPROXIMATION_LEVEL = 30;

const int BEGINNING_ID_OF_ORBIT_BALL = 1;

const float ORBIT_BOUNDING_TETRAHEDRON_SCALE = 4;

const float INITIAL_SEARCH_INTERVAL = 1;
const int MAX_SEARCH_ITERATION = 100;
const float INTERVAL_EPSILON = 0.01;

const int TCA_ACTIVATION_THRESHOLD = 180;

class OrbitalBall;

struct SegmentTransitionEvent
{
	double time;
	OrbitalBall* ball;
};


struct compare_segment_transition_events_in_ascending_order {
	bool operator()(const SegmentTransitionEvent& lhs, const SegmentTransitionEvent& rhs)
	{
		return lhs.time > rhs.time;
	}
};


#ifndef _WIN32
#include <limits>
#define DBL_MAX std::numeric_limits<double>::max()
#endif

#define SCAN_MINIMAL_PROXIMITY

const int OPTIMAL_MANEUVER_SEARCH_RANGE = 10;
const int MANEUVER_ANGLE_INCREMENT = 1;

const double standardTimeIncrement = 10;

struct PredictionCommand
{
	string directory, tleFile;
	int numObject, numLineSegments, predictionTimeWindow;
	int year, month, day, hour, min, sec;
};


struct PredictedTCA
{
	string primaryRSO, secondaryRSO;
	int primaryID, secondaryID;
	double minDistance;
	int year, month, day, hour, min, sec;
};


struct ActiveTCA
{
	rg_Point3D pt1;
	rg_Point3D pt2;
	Color3f color;
};