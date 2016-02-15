
/* Chassis */

Victor* motors[CHASSIS_NUM_MOTORS];
Encoder* leftEncoder;
Encoder* rightEncoder;

void InitializeChassis(){
	motors[0] = new Victor(CHASSIS_PORT_FL);
	motors[1] = new Victor(CHASSIS_PORT_BL);
	motors[2] = new Victor(CHASSIS_PORT_FR);
	motors[3] = new Victor(CHASSIS_PORT_BR);
	leftEncoder = new Encoder(LEFT_ENCODER_PORT_A, LEFT_ENCODER_PORT_B, 
				  false, Encoder::EncodingType::k4X);
	rightEncoder = new Encoder(RIGHT_ENCODER_PORT_A, RIGHT_ENCODER_PORT_B, 
				   false, Encoder::EncodingType::k4X);
	Cout("Chassis Initialized");
}

void UpdateChassis(CHRISTOPHMemory* memory){
	CHRISTOPHState* christophState = scast<CHRISTOPHState*>(memory->permanentStorage);
	ChassisState* state = &(christophState->chassisState);

	if(!state->isInitialized){
		state->isInitialized = True;
		state->nMotors = CHASSIS_NUM_MOTORS;
		
		state->invertedMotors[0] = 1;
		state->invertedMotors[1] = 1;
		state->invertedMotors[2] = 1;
		state->invertedMotors[3] = 1;

		state->sensitivity = 0.5f;
		state->chassisMagnitude = DEF_SPEED;
		state->chassisEnabled = True;
		leftEncoder->Reset();
		rightEncoder->Reset();
	}

	for(U32 i = 0; i < state->nMotors; i++){
		motors[i]->Set(state->motorValues[i] * state->invertedMotors[i]);
	}

	S32 leftEncoderVal = leftEncoder->Get();
	S32 rightEncoderVal = rightEncoder->Get();
	state->dLeftEncoder = leftEncoderVal - state->leftEncoder;
	state->leftEncoder = leftEncoderVal;
	state->dRightEncoder = rightEncoderVal - state->rightEncoder;
	state->rightEncoder = rightEncoderVal;

}

void TerminateChassis(){
	delete leftEncoder;
	delete rightEncoder;
	for(U32 i = 0; i < CHASSIS_NUM_MOTORS; i++){
		delete motors[i];
	}
	//deleteMutex(chassisGyroLock);
	//deleteMutex(chassisMotorLock);
}

/* Shooter */

VictorSP* outerIntakeMotor;
Victor* innerLowerIntakeMotor;
Victor* lowerShooterMotor;
Victor* innerUpperIntakeMotor;
Victor* upperShooterMotor;

void InitializeShooter(){
	outerIntakeMotor = new VictorSP(SHOOTER_PORT_OI);
	innerLowerIntakeMotor = new Victor(SHOOTER_PORT_ILI);
	lowerShooterMotor = new Victor(SHOOTER_PORT_LS);
	innerUpperIntakeMotor = new Victor(SHOOTER_PORT_IUI);
	upperShooterMotor = new Victor(SHOOTER_PORT_US);
	Cout("Shooter Initialized");
}

void UpdateShooter(CHRISTOPHMemory* memory){
	CHRISTOPHState* christophState = scast<CHRISTOPHState*>(memory->permanentStorage);
	ShooterState* state = &(christophState->shooterState);

	if(!state->isInitialized){
		state->isInitialized = True;
		state->nMotors = SHOOTER_NUM_MOTORS;
		
		state->outerIntakeInversion = -1;
		state->innerLowerIntakeInversion = 1;
		state->innerUpperIntakeInversion = 1;
		state->lowerShooterInversion = 1;
		state->upperShooterInversion = 1;
	}

	outerIntakeMotor->Set(state->outerIntakeValue * state->outerIntakeInversion);
	innerLowerIntakeMotor->Set(state->innerLowerIntakeValue * 
							   state->innerLowerIntakeInversion);
	innerUpperIntakeMotor->Set(state->innerUpperIntakeValue * 
							   state->innerUpperIntakeInversion);
	lowerShooterMotor->Set(state->lowerShooterValue * state->lowerShooterInversion);
	upperShooterMotor->Set(state->upperShooterValue * state->upperShooterInversion);
#if 0
	Cout("%.4f || %.4f || %.4f || %.4f || %.4f", state->outerIntakeValue, 
		 state->innerLowerIntakeValue, state->innerUpperIntakeValue,
		 state->lowerShooterValue, state->upperShooterValue);
#endif
}

void TerminateShooter(){
	delete outerIntakeMotor;
	delete innerLowerIntakeMotor;
	delete innerUpperIntakeMotor;
	delete lowerShooterMotor;
	delete upperShooterMotor;
}