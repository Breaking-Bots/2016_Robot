#include "CHRISTOPHEngine.h"

/* CHRISTOPH Engine */

uul_extern {

#include "CHRISTOPHAuton.h"


CHRISTOPH_CALLBACK(SingleControllerInputControlledCallback){
	CHRISTOPHState* state = scast<CHRISTOPHState*>(memory->permanentStorage);
	ChassisState* chassisState = &(state->chassisState);
	ShooterState* shooterState = &(state->shooterState);

	if(!memory->isInitialized){
		for(U32 i = 0; i < NUM_TIMERS; i++){
			state->timers[i] = {};
		}
		chassisState->chassisMagnitude = DEF_SPEED;
		chassisState->reverse = False;
		memory->isInitialized = True;
	}

	Gamepad* gamepad = GetGamepad(input, 0);

	//Input processing
	F32 lx = Analog(gamepad, _LX);
	F32 ly = Analog(gamepad, _LY);
	F32 rx = Analog(gamepad, _RX);
	F32 ry = Analog(gamepad, _RY);
	F32 lt = Analog(gamepad, _LT);
	F32 rt = Analog(gamepad, _RT);

	S32 left = DPAD(gamepad, _LEFT);
	S32 right = DPAD(gamepad, _RIGHT);

	if(ButtonTapped(gamepad, _START)){
		chassisState->chassisEnabled = !chassisState->chassisEnabled;
	}

	if(ButtonTapped(gamepad, _B)){
		chassisState->tankDrive = !chassisState->tankDrive;
	}

	if(ButtonTapped(gamepad, _L3)){
		chassisState->reverse = !chassisState->reverse;
	}

	SetChassisMagnitude(memory, memory->SystemMagnitudeInterpolation(MIN_SPEED, 
						DEF_SPEED, MAX_SPEED, rt - lt));

	if(chassisState->tankDrive){
		if(chassisState->reverse){
			TankDrive(memory, -ly, -ry);
		}else{
			TankDrive(memory, ly, ry);
		}
	}else{
		if(chassisState->reverse){
			CHRISTOPHDrive(memory, -ly, rx);
		}else{
			CHRISTOPHDrive(memory, ly, rx);
		}
	}

	if(SufficientTimeElapsed(memory, 1) && SufficientTimeElapsed(memory, 2)){
		SetShooterMotors(memory, right - left, right - left, right - left);

		if(Button(gamepad, _RB)){
			SetShooterMotors(memory, OUTER_INTAKE_SPEED, INNER_INTAKE_SPEED, 
							 SHOOTER_INTAKE_SPEED);
		}
		if(ButtonTapped(gamepad, _LB)){
			StartTimer(memory, 0, DRAWBACK_TIME);
		}
		if(!SufficientTimeElapsed(memory, 0)){
			SetShooterMotors(memory, 0.0f, DRAWBACK_SPEED, DRAWBACK_SPEED);
			//memory->Cout("wow");
		}
	}
	if(SufficientTimeElapsed(memory, 0)){
		if(ButtonTapped(gamepad, _X) && SufficientTimeElapsed(memory, 2)){
			StartTimer(memory, 1, SPIN_UP_TIME);
		}	

		if(!SufficientTimeElapsed(memory, 1)){
			SetShooterMotors(memory, 0.0f, 0.0f, SHOOTER_SPEED);
		}

		if(TimerEnded(memory, 1)){
			StartTimer(memory, 2, FOLLOW_THROUGH_TIME);
		}

		if(!SufficientTimeElapsed(memory, 2)){
			SetShooterMotors(memory, 0.0f, SHOOTER_SPEED, SHOOTER_SPEED);
		}

	}
}

CHRISTOPH_CALLBACK(DoubleControllerInputControlledCallback){
	CHRISTOPHState* state = scast<CHRISTOPHState*>(memory->permanentStorage);
	ChassisState* chassisState = &(state->chassisState);
	ShooterState* shooterState = &(state->shooterState);

	if(!memory->isInitialized){
		for(U32 i = 0; i < NUM_TIMERS; i++){
			state->timers[i] = {};
		}
		chassisState->chassisMagnitude = DEF_SPEED;
		chassisState->reverse = False;
		memory->isInitialized = True;
	}

	Gamepad* driver = GetGamepad(input, DRIVER_GAMEPAD);
	Gamepad* shooter = GetGamepad(input, SHOOTER_GAMEPAD);

	//Input processing
	F32 dlx = Analog(driver, _LX);
	F32 dly = Analog(driver, _LY);
	F32 drx = Analog(driver, _RX);
	F32 dry = Analog(driver, _RY);
	F32 dlt = Analog(driver, _LT);
	F32 drt = Analog(driver, _RT);
	F32 slx = Analog(shooter, _LX);
	F32 sly = Analog(shooter, _LY);
	F32 srx = Analog(shooter, _RX);
	F32 sry = Analog(shooter, _RY);
	F32 slt = Analog(shooter, _LT);
	F32 srt = Analog(shooter, _RT);

	S32 da = Button(driver, _A);
	S32 dy = Button(driver, _Y);
	S32 sa = Button(shooter, _A);
	S32 sy = Button(shooter, _Y);

	S32 dl = DPAD(driver, _LEFT);
	S32 dr = DPAD(driver, _RIGHT);

	if(ButtonTapped(driver, _START)){
		//chassisState->chassisEnabled = !chassisState->chassisEnabled;
	}

	if(ButtonTapped(driver, _B)){
		//chassisState->tankDrive = !chassisState->tankDrive;
	}

	SetStepHammerMotor(memory, dy + sy - da - sa);

	if(ButtonTapped(driver, _L3)){
		chassisState->reverse = !chassisState->reverse;
	}

	SetChassisMagnitude(memory, memory->SystemMagnitudeInterpolation(MIN_SPEED,
						DEF_SPEED, MAX_SPEED, drt - dlt));


	if(chassisState->tankDrive){
		if(chassisState->reverse){
			TankDrive(memory, -dly, -dry);
		}else{
			TankDrive(memory, dly, dry);
		}
	}else{
		if(chassisState->reverse){
			CHRISTOPHDrive(memory, -dly, drx);
		}else{
			CHRISTOPHDrive(memory, dly, drx);
		}
	}

	//memory->Cout("%.4f || %.4f", slt, srt);	
	F32 shooterMotorControlTriplex = memory->ClampN(srt + dr - slt - dl);

	if(SufficientTimeElapsed(memory, 1) && SufficientTimeElapsed(memory, 2)){
		SetShooterMotors(memory, shooterMotorControlTriplex, 
						 shooterMotorControlTriplex, shooterMotorControlTriplex);

		//TODO: put shooter intake back
		if(Button(shooter, _RB) ||	Button(driver, _RB)){
			SetShooterMotors(memory, OUTER_INTAKE_SPEED, INNER_INTAKE_SPEED,
							 SHOOTER_INTAKE_SPEED);
		}
		if(ButtonTapped(shooter, _LB)){
			StartTimer(memory, 0, DRAWBACK_TIME);
		}
		if(!SufficientTimeElapsed(memory, 0)){
			SetShooterMotors(memory, OUTER_INTAKE_SPEED, 
							 DRAWBACK_SPEED, DRAWBACK_SPEED);
		}
	}
	if(SufficientTimeElapsed(memory, 0)){
		if(ButtonTapped(shooter, _X) && SufficientTimeElapsed(memory, 2)){
			StartTimer(memory, 1, SPIN_UP_TIME);
		}	

		if(!SufficientTimeElapsed(memory, 1)){
			SetShooterMotors(memory, 0.0f, 0.0f, SHOOTER_SPEED);
		}

		if(TimerEnded(memory, 1)){
			StartTimer(memory, 2, FOLLOW_THROUGH_TIME);
		}

		if(!SufficientTimeElapsed(memory, 2)){
			SetShooterMotors(memory, 0.0f, SHOOTER_SPEED, SHOOTER_SPEED);
		}

	}
#if 0
	memory->Cout("Chassis: %.4f || %.4f || %.4f || %.4f", 
				 chassisState->motorValues[0], 
				 chassisState->motorValues[1], chassisState->motorValues[2], 
				 chassisState->motorValues[3]);
	memory->Cout("Shooter: %.4f || %.4f || %.4f || %.4f || %.4f", 
				shooterState->outerIntakeValue, 
		 		shooterState->innerLowerIntakeValue, 
		 		shooterState->innerUpperIntakeValue,
		 		shooterState->lowerShooterValue, shooterState->upperShooterValue);
#endif
}

CHRISTOPH_CALLBACK(InitTeleop){
	memory->Cout("InitTeleop");
}

CHRISTOPH_CALLBACK(TeleopCallback){
	#if NUM_GAMEPADS == 1
		SingleControllerInputControlledCallback(memory, input, dt);
	#elif NUM_GAMEPADS == 2
		DoubleControllerInputControlledCallback(memory, input, dt);
	#endif
}

CHRISTOPH_CALLBACK(InitTest){
	memory->Cout("InitTest");
}

CHRISTOPH_CALLBACK(TestCallback){
	#if NUM_GAMEPADS == 1
		SingleControllerInputControlledCallback(memory, input, dt);
	#elif NUM_GAMEPADS == 2
		DoubleControllerInputControlledCallback(memory, input, dt);
	#endif
}

CHRISTOPH_CALLBACK(InitAutonomous){
	memory->Cout("InitAutonomous");
	CHRISTOPHState* state = scast<CHRISTOPHState*>(memory->permanentStorage);
	for(U32 i = 0; i < NUM_AUTON_STATES; i++){
		state->autonState[i] = False;
	}
}

CHRISTOPH_CALLBACK(AutonomousCallback){

#if RECORDED_AUTON

	#if NUM_GAMEPADS == 1
		SingleControllerInputControlledCallback(memory, input, dt);
	#elif NUM_GAMEPADS == 2
		DoubleControllerInputControlledCallback(memory, input, dt);
	#endif

#else 

	#if DRIVE_AND_SHOOT_SPYBOT_AUTON

		DriveAndShootSpybotAuton(memory, input, dt);

	#elif SHOOT_SPYBOT_AUTON

		ShootSpybotAuton(memory, input, dt);

	#endif

#endif

}

CHRISTOPH_CALLBACK(InitDisabled){
	memory->Cout("InitDisabled");
}

CHRISTOPH_CALLBACK(DisabledCallback){
	
}

/* Chassis */

intern void SetLeftRightMotorValues(CHRISTOPHMemory* memory, F32 leftMgntd, 
									F32 rightMgntd){
	CHRISTOPHState* christophState = scast<CHRISTOPHState*>(memory->permanentStorage);
	ChassisState* state = &christophState->chassisState;
	if(state->chassisEnabled){
		state->motorValues[0] = memory->ClampN(leftMgntd * state->chassisMagnitude);
		state->motorValues[1] = memory->ClampN(-rightMgntd * state->chassisMagnitude);
	}
}

intern void SetMotorValues(CHRISTOPHMemory* memory, F32 m0, F32 m1){
	CHRISTOPHState* christophState = scast<CHRISTOPHState*>(memory->permanentStorage);
	ChassisState* state = &christophState->chassisState;
	if(state->chassisEnabled){
		state->motorValues[0] = memory->ClampN(m0 * state->chassisMagnitude);
		state->motorValues[1] = memory->ClampN(m1 * state->chassisMagnitude);
	}
}

void SetStepHammerMotor(CHRISTOPHMemory* memory, F32 value){
	CHRISTOPHState* christophState = scast<CHRISTOPHState*>(memory->permanentStorage);
	ChassisState* state = &christophState->chassisState;
	state->stepMotorValue = STEP_SPEED * memory->ClampN(value);
}

void RawDrive(CHRISTOPHMemory* memory, F32 mgntd, F32 curve){
	CHRISTOPHState* christophState = scast<CHRISTOPHState*>(memory->permanentStorage);
	ChassisState* state = &christophState->chassisState;
	if(curve < 0){
		F32 value = logf(-curve);
		F32 ratio = (value - state->sensitivity) / (value + state->sensitivity);
		if(ratio == 0) {
			ratio = EZERO;
		}
		SetLeftRightMotorValues(memory, mgntd/ratio, mgntd);
	}else if(curve > 0){
		F32 value = logf(curve);
		F32 ratio = (value - state->sensitivity) / (value + state->sensitivity);
		if(ratio == 0){
			ratio = EZERO;
		}
		SetLeftRightMotorValues(memory, mgntd, mgntd/ratio);
	}else{
		SetLeftRightMotorValues(memory, mgntd, mgntd);
	}
}

void TankDrive(CHRISTOPHMemory* memory, F32 leftMgntd, F32 rightMgntd){
	SetLeftRightMotorValues(memory, leftMgntd, rightMgntd);
}

void CHRISTOPHDrive(CHRISTOPHMemory* memory, F32 fwdMgntd, F32 turnMgntd){
	fwdMgntd = memory->ClampN(fwdMgntd);
	turnMgntd = memory->ClampN(turnMgntd);
	if(fwdMgntd > 0.0f){
		if(turnMgntd > 0.0f){
			SetLeftRightMotorValues(memory, fwdMgntd - turnMgntd, 
									memory->Max(fwdMgntd, turnMgntd));
		}else{
			SetLeftRightMotorValues(memory, memory->Max(fwdMgntd, -turnMgntd),
									fwdMgntd + turnMgntd);
		}
	}else{
		if(turnMgntd > 0.0f){
			SetLeftRightMotorValues(memory, -memory->Max(-fwdMgntd, turnMgntd),
									fwdMgntd + turnMgntd);
		}else{
			SetLeftRightMotorValues(memory, fwdMgntd - turnMgntd, 
									-memory->Max(-fwdMgntd, -turnMgntd));
		}
	}
}

void SetChassisMagnitude(CHRISTOPHMemory* memory, F32 magnitude){
	CHRISTOPHState* christophState = scast<CHRISTOPHState*>(memory->permanentStorage);
	ChassisState* state = &christophState->chassisState;
	state->chassisMagnitude = magnitude;
}

void EnableChassis(CHRISTOPHMemory* memory, B32 enable){
	CHRISTOPHState* christophState = scast<CHRISTOPHState*>(memory->permanentStorage);
	ChassisState* state = &christophState->chassisState;
	state->chassisEnabled = enable;
}

B32 IsChassisEnabled(CHRISTOPHMemory* memory){
	CHRISTOPHState* christophState = scast<CHRISTOPHState*>(memory->permanentStorage);
	ChassisState* state = &christophState->chassisState;
	return state->chassisEnabled;
}

void StopMotors(CHRISTOPHMemory* memory){
	CHRISTOPHState* christophState = scast<CHRISTOPHState*>(memory->permanentStorage);
	ChassisState* state = &christophState->chassisState;
	if(state->isInitialized){
		for(U32 i = 0; i < state->nMotors; i++){
			state->motorValues[i] = 0.0f;
		}
	}
}

void InvertChassisMotor(CHRISTOPHMemory* memory, U32 motorPort){
	CHRISTOPHState* christophState = scast<CHRISTOPHState*>(memory->permanentStorage);
	ChassisState* state = &christophState->chassisState;
	if(state->isInitialized){
		state->invertedMotors[motorPort] *= -1;
	}else{
		memory->Cerr("Invalid port: %d.", motorPort);
	}
}

/* Shooter */

void SetShooterMotors(CHRISTOPHMemory* memory, F32 outSpeed, F32 inSpeed,
					  F32 shotSpeed){
	CHRISTOPHState* christophState = scast<CHRISTOPHState*>(memory->permanentStorage);
	ShooterState* state = &christophState->shooterState;
	if(state->isInitialized){
		state->outerIntakeValue = memory->ClampN(outSpeed);
		state->innerLowerIntakeValue = memory->ClampN(inSpeed);
		state->innerUpperIntakeValue = memory->ClampN(-inSpeed);
		state->lowerShooterValue = memory->ClampN(shotSpeed);
		state->upperShooterValue = memory->ClampN(-shotSpeed);
	}
}

void InvertShooterMotor(CHRISTOPHMemory* memory, U32 motorPort){
	CHRISTOPHState* christophState = scast<CHRISTOPHState*>(memory->permanentStorage);
	ShooterState* state = &christophState->shooterState;
	if(state->isInitialized){
		state->invertedMotors[motorPort] *= -1;
	}else{
		memory->Cerr("Invalid port: %d.", motorPort);
	}
}

/* Util */

void StartTimer(CHRISTOPHMemory* memory, U32 timerIndex, F32 targetSeconds){
	CHRISTOPHState* state = scast<CHRISTOPHState*>(memory->permanentStorage);
	if(!state->timers[timerIndex].isStarted){
		state->timers[timerIndex].startTime = memory->SystemTime();
		state->timers[timerIndex].targetSeconds = targetSeconds;
		state->timers[timerIndex].isStarted = True;
	}
}

B32 SufficientTimeElapsed(CHRISTOPHMemory* memory, U32 timerIndex){
	CHRISTOPHState* state = scast<CHRISTOPHState*>(memory->permanentStorage);
	if(state->timers[timerIndex].isStarted){
		if(state->timers[timerIndex].targetSeconds < 
		   (memory->SystemTime() - state->timers[timerIndex].startTime) / 1000.0f){
			state->timers[timerIndex].isStarted = False;
			state->timers[timerIndex].endPulse = True;
			return True;
		}else{
			return False;
		}
	}else{
		return True;
	}
}

B32 TimerEnded(CHRISTOPHMemory* memory, U32 timerIndex){
	CHRISTOPHState* state = scast<CHRISTOPHState*>(memory->permanentStorage);
	if(state->timers[timerIndex].endPulse){
		state->timers[timerIndex].endPulse = False;
		return true;
	}else{
		return False;
	}
}

/* Input */

U32 Buttons(Gamepad* gamepad){
	U32 buttons = 0;
	for(U32 i = 0; i < NUM_BUTTONS; i++){
		buttons |= buttons & (gamepad->buttons[i].endedDown << i);
	}
	return buttons;
}

B32 Button(Gamepad* gamepad, U32 buttonIndex){
	return (gamepad->buttons[buttonIndex].endedDown);
}

B32 ButtonTapped(Gamepad* gamepad, U32 buttonIndex){
	return (gamepad->buttons[buttonIndex].endedDown && 
			(gamepad->buttons[buttonIndex].halfTransitionCount));
}

B32 ButtonHeld(Gamepad* gamepad, U32 buttonIndex){
	return (gamepad->buttons[buttonIndex].endedDown && 
			!gamepad->buttons[buttonIndex].halfTransitionCount);
}

B32 ButtonReleased(Gamepad* gamepad, U32 buttonIndex){
	return (!gamepad->buttons[buttonIndex].endedDown &&
			(gamepad->buttons[buttonIndex].halfTransitionCount));
}

B32 ButtonPostRealeased(Gamepad* gamepad, U32 buttonIndex){
	return (!gamepad->buttons[buttonIndex].endedDown && 
			!gamepad->buttons[buttonIndex].halfTransitionCount);
}

B32 DPAD(Gamepad* gamepad, U32 dpadIndex){
	return (gamepad->dpad[dpadIndex].endedDown);
}

B32 DPADTapped(Gamepad* gamepad, U32 dpadIndex){
	return (gamepad->dpad[dpadIndex].endedDown && 
			(gamepad->dpad[dpadIndex].halfTransitionCount));
}

B32 DPADHeld(Gamepad* gamepad, U32 dpadIndex){
	return (gamepad->dpad[dpadIndex].endedDown && 
			!gamepad->dpad[dpadIndex].halfTransitionCount);
}

B32 DPADReleased(Gamepad* gamepad, U32 dpadIndex){
	return (!gamepad->dpad[dpadIndex].endedDown && 
			(gamepad->dpad[dpadIndex].halfTransitionCount));
}

B32 DPADPostRealeased(Gamepad* gamepad, U32 dpadIndex){
	return (!gamepad->dpad[dpadIndex].endedDown && 
			!gamepad->dpad[dpadIndex].halfTransitionCount);
}

F32 Analog(Gamepad* gamepad, U32 stickIndex) {
	return gamepad->analog[stickIndex];
}

/* Memory */



}