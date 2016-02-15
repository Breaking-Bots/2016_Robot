#ifndef CHRISTOPH_SHOOTER_H
#define CHRISTOPH_SHOOTER_H

uul_extern{

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

#endif