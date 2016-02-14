#ifndef HARDWARE_LAYER_H
#define HARDWARE_LAYER_H

/* 
 * HardwareLayer.h
 * 
 * Created on: Feb 7, 2016
 * Author: Zaeem
 *
 */

class Task;

typedef void* MODULE;

typedef S32 HANDLE;

uul_extern{

/* Util */

/**
 * Get FGPA Time in milliseconds
 */
SYSTEM_TIME_CALLBACK(SystemTime);

/**
 * Clamps value of a between b and c
 */
F32_CALLBACK_F32_F32_F32(Clamp);

/**
 * Clamps value of a between 0 and 1
 */
F32_CALLBACK_F32(Clamp01);

/**
 * Clamps value of a between -1 and 1
 */
F32_CALLBACK_F32(ClampN);

/**
 * Returns the greater value of a and b
 */
F32_CALLBACK_F32_F32(Max);

/**
 * Returns the value of x squared
 */
F32_CALLBACK_F32(Sq);

/**
 * Returns the value of x cubed
 */
F32_CALLBACK_F32(Cu);

/**
 * Returns the value of x quarted
 */
F32_CALLBACK_F32(Qu);

/**
 * Return sign of x
 */
S32_CALLBACK_F32(Sgn);

/**
 * Normalizes x of interval [-1,1] to interval [0,1]
 */
F32_CALLBACK_F32(NormalizeAlpha);

/**
 * Linearly interpolates between a and b in the scale of value c of interval [0,1]
 */
F32_CALLBACK_F32_F32_F32(Lerp);

/**
 * Calculates cosine interpolation between a and b in the scale of value c of interval [0,1]
 */
F32_CALLBACK_F32_F32_F32(Coserp);

/**
 * Calculates the system magnitude by interpolation
 * between a, b, c in the scale of d of interval [-1,1]
 */
F32_CALLBACK_F32_F32_F32_F32(SystemMagnitudeInterpolation);

/**
 * Calculates principle angle in degrees from
 * interval [-INFINITY,INFINITY] to interval [0,360]
 */
F32_CALLBACK_F32(PrincipleAngleDeg);

/**
 * Calculates principle angle in radians from
 * interval [-INFINITY,INFINITY] to interval [0,TAU]
 */
F32_CALLBACK_F32(PrincipleAngleRad);

/**
 * Calculate minimum distance angle in degrees from
 * interval [-INFINITY,INFINITY] to interval [-180,180]
 */
F32_CALLBACK_F32(MinDistAngleDeg);

/**
 * Calculate minimum distance angle in radians from
 * interval [-INFINITY,INFINITY] to interval [-PI,PI]
 */
F32_CALLBACK_F32(MinDistAngleRad);

/**
 * Calculates the shortest angular distance in degrees between 
 * two angles of interval [-INFINITY,INFINITY] to [-180,180]
 */
F32_CALLBACK_F32_F32(AngularDistDeg);

/**
 * Calculates the shortest angular distance in radians between 
 * two angles of interval [-INFINITY,INFINITY] to [-PI,PI]
 */
F32_CALLBACK_F32_F32(AngularDistRad);

/**
 * Represents file
 */
struct File{
	void* data;
	U32 size;
};

U32 GetFileSize(std::string filename, S32 fd = 0, B32 useFD = False, B32 ignoreFailure = False);

/**
 * Reads entire file and returns File struct
 */
File ReadEntireFile(std::string filename, B32 ignoreFailure = False);

/**
 * Writes memory to file of given filename, creates file if doesn't exist
 */
B32 WriteEntireFile(std::string filename, U32 memorySize, void* memory,
					B32 ignoreFailure = False);

/**
 * Copies one file to another
 * TODO: optimize or find better solution
 */
B32 CopyFile(std::string src, std::string dest, B32 ignoreFailure = False);

/**
 * Returns last time a file was written to
 */
S64 GetLastWriteTime(std::string filename, B32 ignoreFailure = False);

/* CHRISTOPH Engine Management */

struct CHRISTOPHEngine{
	MODULE module;
	S64 lastWriteTime;
	CHRISTOPHCallback* InitTeleop;
	CHRISTOPHCallback* TeleopCallback;
	CHRISTOPHCallback* InitTest;
	CHRISTOPHCallback* TestCallback;
	CHRISTOPHCallback* InitAutonomous;
	CHRISTOPHCallback* AutonomousCallback;
	CHRISTOPHCallback* InitDisabled;
	CHRISTOPHCallback* DisabledCallback;
	CHRISTOPHCallback* InitTemp;
	CHRISTOPHCallback* TempCallback;
	B32 isValid;
};

CHRISTOPH_CALLBACK(CHRISTOPHCallbackStub);

/**
 * Loads CHRISTOPHEngine
 */
CHRISTOPHEngine LoadCHRISTOPHEngine(std::string filename);

/**
 * Unloads CHRISTOPHEngine
 */
void UnloadCHRISTOPHEngine(CHRISTOPHEngine* engine);

/* Chassis */

/**
 * Initialize chassis and motors
 */
void InitializeChassis();

/**
 * Writes motor values to motor controllers
 */
void UpdateChassis(CHRISTOPHMemory* memory);

/**
 * Free chassis memory
 */
void TerminateChassis();

/* Shooter */

/**
 * Initialize shooter
 */
void InitializeShooter();

/**
 * Writes motor values to motor controllers
 */
void UpdateShooter(CHRISTOPHMemory* memory);

/*
 * Terminates shooter
 */
void TerminateShooter();

/* Input */

/**
 * Updates the gamepads from data recieved from driver station
 */
void UpdateInput(DriverStation* ds, Input* newInput, Input* oldInput);

}

/* Hardware Layer */

struct HLState{
	U32 totalSize;
	void* totalCHRISTOPHMemoryBlock;
};

/* CHRISTOPH Class */

class CHRISTOPH : public SampleRobot{
public:
	CHRISTOPH();

	void RobotInit();

	void RobotMain();

	~CHRISTOPH();
};


#endif