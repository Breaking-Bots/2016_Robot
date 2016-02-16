#ifndef CHRISTOPH_ENGINE_H
#define CHRISTOPH_ENGINE_H

/* Properties */

#define DISABLE_CORE_THREAD 0
#define DISABLE_FAST_THREAD 1

#define CORE_THREAD_HZ 50
#define FAST_THREAD_HZ 200

#define NUM_GAMEPADS 2

#define CHASSIS_NUM_MOTORS 4
#define CHASSIS_PORT_FL 4
#define CHASSIS_PORT_BL 3
#define CHASSIS_PORT_FR 7
#define CHASSIS_PORT_BR 6
#define CHASSIS_PORTS CHASSIS_PORT_FL, CHASSIS_PORT_BL, CHASSIS_PORT_FR, CHASSIS_PORT_BR

#define SHOOTER_NUM_MOTORS 5
#define SHOOTER_PORT_OI 5
#define SHOOTER_PORT_ILI 0
#define SHOOTER_PORT_LS 1
#define SHOOTER_PORT_IUI 8
#define SHOOTER_PORT_US 2

#define LEFT_ENCODER_PORT_A 0
#define LEFT_ENCODER_PORT_B 1

#define RIGHT_ENCODER_PORT_A 2
#define RIGHT_ENCODER_PORT_B 3

#define MIN_SPEED (0.35f)
#define DEF_SPEED (0.65f)
#define MAX_SPEED (1.00f)

#define OUTER_INTAKE_SPEED (1.0f)
#define INNER_INTAKE_SPEED (1.0f)
#define SHOOTER_INTAKE_SPEED (-0.3f)
#define DRAWBACK_SPEED (-0.9f)
#define DRAWBACK_TIME (0.5f)
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