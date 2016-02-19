#ifndef CHRISTOPH_MEMORY_H
#define CHRISTOPH_MEMORY_H

struct CHRISTOPHMemory;
struct CHRISTOPHState;
struct ChassisState;

typedef struct CHRISTOPHMemory{
	B32 isInitialized;
	U32 permanentStorageSize;
	void* permanentStorage; //REQUIRED to be cleared to zero at startup
	U32 transientStorageSize;
	void* transientStorage; //REQUIRED to be cleared to zero at startup
	U32 autonomousIndex;

	LoggingCallback* Cout;
	LoggingCallback* Cerr;
	SystemTimeCallback* SystemTime;
	F32CallbackF32F32F32* Clamp;
	F32CallbackF32* Clamp01;
	F32CallbackF32* ClampN;
	F32CallbackF32F32* Max;
	F32CallbackF32* Sq;
	F32CallbackF32* Cu;
	F32CallbackF32* Qu;
	S32CallbackF32* Sgn;
	F32CallbackF32* NormalizeAlpha;
	F32CallbackF32F32F32* Lerp;
	F32CallbackF32F32F32* Coserp;
	F32CallbackF32F32F32F32* SystemMagnitudeInterpolation;
	F32CallbackF32* PrincipleAngleDeg;
	F32CallbackF32* PrincipleAngleRad;
	F32CallbackF32* MinDistAngleDeg;
	F32CallbackF32* MinDistAngleRad;
	F32CallbackF32F32* AngularDistDeg;
	F32CallbackF32F32* AngularDistRad;
} CHRISTOPHMemory;

typedef struct ChassisState{
	F32 motorValues[CHASSIS_NUM_MOTORS]; //Array of motor speed values
	F32 stepMotorValue;
	F32 chassisMagnitude; //Magnitude of chassis speed
	F32 sensitivity; //Sensitivity of RawDrive
	U32 nMotors = CHASSIS_NUM_MOTORS; //Number of used motors
	B32 isInitialized; //Initialization flag
	B32 chassisEnabled; //Allow chassis control flag
	S32 invertedMotors[CHASSIS_NUM_MOTORS]; //Array of motor inversions
	S32 invertedStepMotor;
	B32 reverse;
	B32 tankDrive;
	S32 leftEncoder;
	S32 rightEncoder;
	S32 dLeftEncoder;
	S32 dRightEncoder;
} ChassisState;

typedef struct ShooterState{
	union{
		F32 motorValues[SHOOTER_NUM_MOTORS];
		struct{
			F32 outerIntakeValue;
			F32 innerLowerIntakeValue;
			F32 lowerShooterValue;
			F32 innerUpperIntakeValue;
			F32 upperShooterValue;
		};
	};
	union{
		S32 invertedMotors[SHOOTER_NUM_MOTORS];
		struct{
			S32 outerIntakeInversion;
			S32 innerLowerIntakeInversion;
			S32 lowerShooterInversion;
			S32 innerUpperIntakeInversion;
			S32 upperShooterInversion;
		};
	};
	U32 nMotors = SHOOTER_NUM_MOTORS;
	B32 isInitialized;
} ShooterState;

typedef struct CHRISTOPHState{
	ChassisState chassisState;
	ShooterState shooterState;
	CHRISTOPHTimer timers[NUM_TIMERS];
} CHRISTOPHState;

#endif