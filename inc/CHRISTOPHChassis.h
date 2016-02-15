#ifndef CHRISTOPH_CHASSIS_H
#define CHRISTOPH_CHASSIS_H

uul_extern{

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

}

#endif