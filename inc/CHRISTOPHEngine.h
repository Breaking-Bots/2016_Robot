#ifndef CHRISTOPH_ENGINE_H
#define CHRISTOPH_ENGINE_H

/* Properties */

#define DISABLE_CORE_THREAD 0
#define DISABLE_FAST_THREAD 1

#define CORE_THREAD_HZ 50
#define FAST_THREAD_HZ 200

#define NUM_GAMEPADS 1

#define CHASSIS_NUM_MOTORS 4
#define CHASSIS_PORT_FL 5
#define CHASSIS_PORT_BL 6
#define CHASSIS_PORT_FR 7
#define CHASSIS_PORT_BR 8
#define CHASSIS_PORTS CHASSIS_PORT_FL, CHASSIS_PORT_BL, CHASSIS_PORT_FR, CHASSIS_PORT_BR

#define SHOOTER_NUM_MOTORS 5
#define SHOOTER_PORT_OI 4
#define SHOOTER_PORT_ILI 3
#define SHOOTER_PORT_LS 2
#define SHOOTER_PORT_IUI 0
#define SHOOTER_PORT_US 1

#define LEFT_ENCODER_PORT_A 0
#define LEFT_ENCODER_PORT_B 1

#define RIGHT_ENCODER_PORT_A 2
#define RIGHT_ENCODER_PORT_B 3

#define MIN_SPEED (0.35f)
#define DEF_SPEED (0.65f)
#define MAX_SPEED (1.00f)

#define OUTER_INTAKE_SPEED (1.0f)
#define INNER_INTAKE_SPEED (0.8f)
#define SHOOTER_INTAKE_SPEED (-0.3f)
#define DRAWBACK_SPEED (-0.7f)
#define DRAWBACK_TIME (0.3f)
#define SHOOTER_SPEED (1.0f)
#define SPIN_UP_TIME (0.5f)
#define FOLLOW_THROUGH_TIME (0.5f)

#define NUM_TIMERS 16

#define CHRISTOPHEngineBinary ("/home/lvuser/libCHRISTOPH.so")
#define CHRISTOPHEngineTempBinary ("/home/lvuser/libCHRISTOPH_temp.so")

#define _A 0
#define _B 1
#define _X 2
#define _Y 3
#define _LB 4
#define _RB 5
#define _BACK 6
#define _START 7
#define _L3 8
#define _R3 9
#define NUM_BUTTONS 10

#define _LX 0
#define _LY 1
#define _LT 2
#define _RT 3
#define _RX 4
#define _RY 5
#define NUM_AXES 6

#define _UP 0
#define _DOWN 1
#define _LEFT 2
#define _RIGHT 3
#define NUM_DPAD 4

#define DEADZONE (0.25f)
#define DEADZONE_SQ (DEADZONE * DEADZONE)
#define TRIGGER_DEADZONE (0.06f)

/* Util */

#include <cstdint>
#include <cfloat>
#include <cmath>
#include <string>

#ifdef __cplusplus
#define uul_extern extern "C"
#else
#define uul_extern extern
#endif

#define lscope static
#define glob static
#define intern static
#define scast static_cast
#define rcast reinterpret_cast

typedef uint32_t B32;

typedef int8_t S8;
typedef int16_t S16;
typedef int32_t S32;
typedef int64_t S64;

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

typedef float F32;
typedef double F64;

#define S8_MIN INT8_MIN
#define S16_MIN INT16_MIN
#define S32_MIN INT32_MIN
#define S64_MIN INT64_MIN
#define F32_MIN FLT_MIN
#define F64_MIN DBL_MIN

#define S8_MAX INT8_MAX
#define S16_MAX INT16_MAX
#define S32_MAX INT32_MAX
#define S64_MAX INT64_MAX
#define U8_MAX UINT8_MAX
#define U16_MAX UINT16_MAX
#define U32_MAX UINT32_MAX
#define U64_MAX UINT64_MAX
#define F32_MAX FLT_MAX
#define F64_MAX DBL_MAX
#define PI (3.1415926535897932f)
#define TAU (6.2831853071795865f)
#define E (2.71828182845904523526f)
#define RAD_TO_DEG (57.2957795f)
#define DEG_TO_RAD (0.0174532925f)
#define True (0xFFFFFFFF)
#define False (0x00000000)
#define EPSILON (0.00001f)
#define EZERO (0.000000001f)

#define sizeofArr(arr) (sizeof((arr)) / sizeof((arr)[0]))

#define AlignPo2(value, alignment) ((value + ((alignment) - 1)) & ~((alignment) - 1))
#define Align4(value) ((value + 3) & ~3)
#define Align8(value) ((value + 7) & ~7)
#define Align16(value) ((value + 15) & ~15)

uul_extern {

#define F32_CALLBACK_F32_F32_F32_F32(name) F32 name(F32 a, F32 b, F32 c, F32 d)
typedef F32_CALLBACK_F32_F32_F32_F32(F32CallbackF32F32F32F32);

#define F32_CALLBACK_F32_F32_F32(name) F32 name(F32 a, F32 b, F32 c)
typedef F32_CALLBACK_F32_F32_F32(F32CallbackF32F32F32);

#define F32_CALLBACK_F32_F32(name) F32 name(F32 a, F32 b)
typedef F32_CALLBACK_F32_F32(F32CallbackF32F32);

#define F32_CALLBACK_F32(name) F32 name(F32 x)
typedef F32_CALLBACK_F32(F32CallbackF32);

#define S32_CALLBACK_F32(name) S32 name(F32 x)
typedef S32_CALLBACK_F32(S32CallbackF32);

#define U64_CALLBACK_U32(name) U64 name(U32 x)
typedef U64_CALLBACK_U32(U64CallbackU32);

/**
 * Logging dytor
 */
#define LOGGING_CALLBACK(name) S32 name(const std::string& format, ...)
typedef LOGGING_CALLBACK(LoggingCallback);

/**
 * System Time dytor
 */
#define SYSTEM_TIME_CALLBACK(name) F64 name()
typedef SYSTEM_TIME_CALLBACK(SystemTimeCallback);

struct CHRISTOPHMemory;

typedef struct CHRISTOPHTimer{
	U64 startTime;
	F32 targetSeconds;
	B32 isStarted;
	B32 endPulse;
} CHRISTOPHTimer;

void StartTimer(CHRISTOPHMemory* memory, U32 timerIndex, F32 targetSeconds);

B32 SufficientTimeElapsed(CHRISTOPHMemory* memory, U32 timerIndex);

B32 TimerEnded(CHRISTOPHMemory* memory, U32 timerIndex);

/* Input */

/**
 * Gamepad button state
 */
typedef struct ButtonState{
	U32 halfTransitionCount;
	B32 endedDown;
} ButtonState;

/**
 * Gamepad struct holds all necessary analog and digital inputs
 */
typedef struct Gamepad{
	union{
		F32 analog[NUM_AXES];
		struct{
			F32 lx;
			F32 ly;
			F32 lt;
			F32 rt;
			F32 rx;
			F32 ry;

			//All analogs must be added above this line
			F32 analogTerminator;
		};
	};
	union{
		ButtonState buttons[NUM_BUTTONS];
		struct{
			ButtonState a;
			ButtonState b;
			ButtonState x;
			ButtonState y;
			ButtonState lb;
			ButtonState rb;
			ButtonState back;
			ButtonState start;
			ButtonState l3;
			ButtonState r3;

			//All buttons must be added above this line
			ButtonState buttonTerminator;
		};
	};
	union{
		ButtonState dpad[NUM_DPAD];
		struct{
			ButtonState up;
			ButtonState down;
			ButtonState left;
			ButtonState right;

			//All dpad buttons must be added above this line
			ButtonState dpadTerminator;
		};
	};

} Gamepad;

struct Input{
	Gamepad gamepads[NUM_GAMEPADS];
};

inline Gamepad* GetGamepad(Input* input, U32 port){
	if(port > sizeofArr(input->gamepads)){
		return &(input->gamepads[0]);
	}
	return &(input->gamepads[port]);
}


U32 Buttons(Gamepad* gamepad);

B32 Button(Gamepad* gamepad, U32 buttonIndex);

B32 ButtonTapped(Gamepad* gamepad, U32 buttonIndex);

B32 ButtonHeld(Gamepad* gamepad, U32 buttonIndex);

B32 ButtonReleased(Gamepad* gamepad, U32 buttonIndex);

B32 ButtonPostRealeased(Gamepad* gamepad, U32 buttonIndex);

B32 DPAD(Gamepad* gamepad, U32 dpadIndex);

B32 DPADTapped(Gamepad* gamepad, U32 dpadIndex);

B32 DPADHeld(Gamepad* gamepad, U32 dpadIndex);

B32 DPADReleased(Gamepad* gamepad, U32 dpadIndex);

B32 DPADPostRealeased(Gamepad* gamepad, U32 dpadIndex);

F32 Analog(Gamepad* gamepad, U32 stickIndex);



/* Memory */

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
	F32 chassisMagnitude; //Magnitude of chassis speed
	F32 sensitivity; //Sensitivity of RawDrive
	U32 nMotors = CHASSIS_NUM_MOTORS; //Number of used motors
	B32 isInitialized; //Initialization flag
	B32 chassisEnabled; //Allow chassis control flag
	S32 invertedMotors[CHASSIS_NUM_MOTORS]; //Array of motor inversions
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

/* Chassis */

/** Mainly used in Autonomous
 * mgntd - forward component of magnitude sent to motors
 * curve - rate of turn, constant for differing forward speeds
 */
void RawDrive(CHRISTOPHMemory* memory, F32 mgntd, F32 curve);

/**
 * leftMgntd - magnitude of left motors
 * rightMgntd - magnitude of right motors
 */
void TankDrive(CHRISTOPHMemory* memory, F32 leftMgntd, F32 rightMgntd);

/**
 * fwdMgntd - magnitude of robot moving forward
 * turnMgntd - magnitude of turning of robot
 */
void CHRISTOPHDrive(CHRISTOPHMemory* memory, F32 fwdMgntd, F32 turnMgntd);

/**
 * Sets global magnitude of all motors
 */
void SetChassisMagnitude(CHRISTOPHMemory* memory, F32 magnitude);

/**
 * Enables movement of chassis
 */
void EnableChassis(CHRISTOPHMemory* memory, B32 enable);

/**
 * Returns whether or not the chassis is enabled
 */
B32 IsChassisEnabled(CHRISTOPHMemory* memory);

/**
 * Stops all motors
 */
void StopMotors(CHRISTOPHMemory* memory);

/**
 * Inverts motor that has been inserted backwards
 */
void InvertChassisMotor(CHRISTOPHMemory* memory, U32 motorPort);

/* Shooter */

/**
 * Sets all the motors involved in the intake/shooter subsystem
 */
void SetShooterMotors(CHRISTOPHMemory* memory, F32 outSpeed, F32 inSpeed,
					  F32 shotSpeed);

/**
 * Inverts motor that has been inserted backwards
 */
void InvertShooterMotor(CHRISTOPHMemory* memory, U32 motorPort);

}

/* CHRISTOPH Engine */

#define CHRISTOPH_CALLBACK(name) void name(CHRISTOPHMemory* memory, Input* input, F32 dt)
typedef CHRISTOPH_CALLBACK(CHRISTOPHCallback);


#endif