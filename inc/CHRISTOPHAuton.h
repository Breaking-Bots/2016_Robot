#ifndef CHRISTOPH_AUTON_H
#define CHRISTOPH_AUTON_H

CHRISTOPH_CALLBACK(DriveAndShootSpybotAuton){
	CHRISTOPHState* state = scast<CHRISTOPHState*>(memory->permanentStorage);
	if(!state->autonState[0]){
		state->autonState[0] = True;
		StartTimer(memory, 3, STEP_HAMMER_DROP_TIME);
		SetStepHammerMotor(memory, -1.0f);
	}

	SufficientTimeElapsed(memory, 3);
	if(state->autonState[0] && TimerEnded(memory, 3) && !state->autonState[1]){
		state->autonState[1] = True;
		StartTimer(memory, 4, 5.0f);
		SetStepHammerMotor(memory, 0.0f);
		CHRISTOPHDrive(memory, 0.65f, 0);
	}

	SufficientTimeElapsed(memory, 4);
	if(state->autonState[1] && TimerEnded(memory, 4) && !state->autonState[2]){
		state->autonState[2] = True;
		StartTimer(memory, 5, 1.0f);
		CHRISTOPHDrive(memory, 0, 0);
	}

	SufficientTimeElapsed(memory, 5);
	if(state->autonState[2] && TimerEnded(memory, 5) && !state->autonState[3]){
		state->autonState[3] = True;
		StartTimer(memory, 6, DRAWBACK_TIME);
		SetShooterMotors(memory, OUTER_INTAKE_SPEED	, DRAWBACK_SPEED, DRAWBACK_SPEED);
	}

	SufficientTimeElapsed(memory, 6);
	if(state->autonState[3] && TimerEnded(memory, 6) && !state->autonState[4]){
		state->autonState[4] = True;
		StartTimer(memory, 7, SPIN_UP_TIME);
		SetShooterMotors(memory, 0.0f, 0.0f, SHOOTER_SPEED);
	}

	SufficientTimeElapsed(memory, 7);
	if(state->autonState[4] && TimerEnded(memory, 7) && !state->autonState[5]){
		state->autonState[5] = True;
		StartTimer(memory, 8, FOLLOW_THROUGH_TIME);
		SetShooterMotors(memory, 0.0f, SHOOTER_SPEED, SHOOTER_SPEED);
	}

	SufficientTimeElapsed(memory, 8);
	if(state->autonState[5] && TimerEnded(memory, 8) && !state->autonState[6]){
		state->autonState[6] = True;
		SetShooterMotors(memory, 0.0f, 0.0f, 0.0f);
	}
}

CHRISTOPH_CALLBACK(ShootSpybotAuton){
	CHRISTOPHState* state = scast<CHRISTOPHState*>(memory->permanentStorage);

	if(!state->autonState[0]){
		state->autonState[0] = True;
		StartTimer(memory, 3, STEP_HAMMER_DROP_TIME);
		SetStepHammerMotor(memory, -1.0f);
	}

	SufficientTimeElapsed(memory, 3);
	if(state->autonState[0] && TimerEnded(memory, 3) && !state->autonState[1]){
		state->autonState[1] = True;
		StartTimer(memory, 4, DRAWBACK_TIME);
		SetStepHammerMotor(memory, 0.0f);
		SetShooterMotors(memory, OUTER_INTAKE_SPEED	, DRAWBACK_SPEED, DRAWBACK_SPEED);
	}

	SufficientTimeElapsed(memory, 4);
	if(state->autonState[1] && TimerEnded(memory, 4) && !state->autonState[2]){
		state->autonState[2] = True;
		StartTimer(memory, 5, SPIN_UP_TIME);
		SetShooterMotors(memory, 0.0f, 0.0f, SHOOTER_SPEED);
	}

	SufficientTimeElapsed(memory, 5);
	if(state->autonState[2] && TimerEnded(memory, 5) && !state->autonState[3]){
		state->autonState[3] = True;
		StartTimer(memory, 6, FOLLOW_THROUGH_TIME);
		SetShooterMotors(memory, 0.0f, SHOOTER_SPEED, SHOOTER_SPEED);
	}

	SufficientTimeElapsed(memory, 6);
	if(state->autonState[3] && TimerEnded(memory, 6) && !state->autonState[4]){
		state->autonState[4] = True;
		SetShooterMotors(memory, 0.0f, 0.0f, 0.0f);
	}
}

CHRISTOPH_CALLBACK(ChevalDeFriseAuton){
	CHRISTOPHState* state = scast<CHRISTOPHState*>(memory->permanentStorage);

	if(!state->autonState[0]){
		state->autonState[0] = True;
		StartTimer(memory, 3, 1.0f);
		CHRISTOPHDrive(memory, 0.65f, 0);
	}

	SufficientTimeElapsed(memory, 3);
	if(state->autonState[0] && TimerEnded(memory, 3) && !state->autonState[1]){
		state->autonState[1] = True;
		StartTimer(memory, 4, STEP_HAMMER_DROP_TIME);
		SetStepHammerMotor(memory, -1.0f);
		CHRISTOPHDrive(memory, 0.0f, 0.0f);
	}

	SufficientTimeElapsed(memory, 4);
	if(state->autonState[1] && TimerEnded(memory, 4) && !state->autonState[2]){
		state->autonState[2] = True;
		StartTimer(memory, 5, 5.0f);
		SetStepHammerMotor(memory, 0.5f);
	}

	SufficientTimeElapsed(memory, 5);
	if(state->autonState[2] && TimerEnded(memory, 5) && !state->autonState[3]){
		state->autonState[3] = True;
		CHRISTOPHDrive(memory, 0.5f, 0.0f);
		SetStepHammerMotor(memory, 0.0f);
		CHRISTOPHDrive(memory, 0.0f, 0.0f);
	}
}

CHRISTOPH_CALLBACK(PorticullisAuton){
	CHRISTOPHState* state = scast<CHRISTOPHState*>(memory->permanentStorage);

	if(!state->autonState[0]){
		state->autonState[0] = True;
		StartTimer(memory, 3, STEP_HAMMER_DROP_TIME);
		SetStepHammerMotor(memory, -1.0f);
		CHRISTOPHDrive(memory, 0.65f, 0);
	}

	SufficientTimeElapsed(memory, 3);
	if(state->autonState[0] && TimerEnded(memory, 3) && !state->autonState[1]){
		state->autonState[1] = True;
		StartTimer(memory, 4, STEP_HAMMER_DROP_TIME);
		CHRISTOPHDrive(memory, 0.0f, 0.0f);
	}

	SufficientTimeElapsed(memory, 4);
	if(state->autonState[1] && TimerEnded(memory, 4) && !state->autonState[2]){
		state->autonState[2] = True;
		StartTimer(memory, 5, 5.0f);
		SetStepHammerMotor(memory, 0.5f);
	}

	SufficientTimeElapsed(memory, 5);
	if(state->autonState[2] && TimerEnded(memory, 5) && !state->autonState[3]){
		state->autonState[3] = True;
		CHRISTOPHDrive(memory, 0.5f, 0.0f);
		SetStepHammerMotor(memory, 0.0f);
		CHRISTOPHDrive(memory, 0.0f, 0.0f);
	}
}

CHRISTOPH_CALLBACK(RockWallAuton){
	CHRISTOPHState* state = scast<CHRISTOPHState*>(memory->permanentStorage);

}

CHRISTOPH_CALLBACK(RoughTerrainAuton){
	CHRISTOPHState* state = scast<CHRISTOPHState*>(memory->permanentStorage);

}

CHRISTOPH_CALLBACK(ReachDefenseAuton){
	CHRISTOPHState* state = scast<CHRISTOPHState*>(memory->permanentStorage);

}

#endif