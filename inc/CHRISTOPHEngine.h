#ifndef CHRISTOPH_ENGINE_H
#define CHRISTOPH_ENGINE_H

/* Properties */

#define RECORDED_AUTON 0
#define LOOP_AUTON_PLAYBACK 0
#define RECORDED_AUTON_FILENAME ("/home/lvuser/Auton.cir")

#define DRIVE_AND_SHOOT_SPYBOT_AUTON 1
#define SHOOT_SPYBOT_AUTON 0
#define CHEVAL_DE_FRISE_AUTON 0
#define PORTICULLIS_AUTON 0
#define ROCK_WALL_AUTON 0

#define ROUGH_TERRAIN_AUTON 0
#define REACH_DEFENSE_AUTON 0

#define DISABLE_CORE_THREAD 0
#define DISABLE_FAST_THREAD 1

#define CORE_THREAD_HZ 50
#define FAST_THREAD_HZ 60

#define NUM_GAMEPADS 2
#define DRIVER_GAMEPAD 0
#define SHOOTER_GAMEPAD 1

#define CHASSIS_NUM_MOTORS 2
#define CHASSIS_PORT_L 0
#define CHASSIS_PORT_R 1
#define CHASSIS_PORT_ST 1
#define CHASSIS_PORTS CHASSIS_PORT_FL, CHASSIS_PORT_BL, CHASSIS_PORT_FR, CHASSIS_PORT_BR

#define SHOOTER_NUM_MOTORS 5
#define SHOOTER_PORT_OI 0
#define SHOOTER_PORT_ILI 2
#define SHOOTER_PORT_LS 4
#define SHOOTER_PORT_IUI 3
#define SHOOTER_PORT_US 5

#define LEFT_ENCODER_PORT_A 0
#define LEFT_ENCODER_PORT_B 1

#define RIGHT_ENCODER_PORT_A 2
#define RIGHT_ENCODER_PORT_B 3

#define MIN_SPEED (0.5f)
#define DEF_SPEED (0.85f)
#define MAX_SPEED (1.0f)
#define STEP_SPEED (0.3f)

#define OUTER_INTAKE_SPEED (1.0f)
#define INNER_INTAKE_SPEED (1.0f)
#define SHOOTER_INTAKE_SPEED (-0.4f)
#define DRAWBACK_SPEED (-1.0f)
#define DRAWBACK_TIME (0.75f)
#define SHOOTER_SPEED (1.0f)
#define SPIN_UP_TIME (0.75f)
#define FOLLOW_THROUGH_TIME (1.25f)
#define AUTON_DRIVE_SPEED (0.65f)
#define AUTON_WAIT_TIME (1.0f)
#define AUTON_PAUSE_TIME (1.0f)
#define AUTON_DRIVE_TIME (5.0f)
#define STEP_HAMMER_DROP_TIME (1.0f)

#define NUM_TIMERS 16
#define NUM_AUTON_STATES 64

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

#include "CHRISTOPHUtil.h"

/* Input */

#include "CHRISTOPHInput.h"

/* Memory */

#include "CHRISTOPHMemory.h"

/* Chassis */

#include "CHRISTOPHChassis.h"

/* Shooter */

#include "CHRISTOPHShooter.h"

/* CHRISTOPH Engine */

#define CHRISTOPH_CALLBACK(name) void name(CHRISTOPHMemory* memory, Input* input, F32 dt)
typedef CHRISTOPH_CALLBACK(CHRISTOPHCallback);


#endif