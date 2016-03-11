
/* Chassis */

VictorSP* motors[CHASSIS_NUM_MOTORS];
CanTalonSRX* stepMotor;
Encoder* leftEncoder;
Encoder* rightEncoder;

void InitializeChassis(){
	motors[0] = new VictorSP(CHASSIS_PORT_L);
	motors[1] = new VictorSP(CHASSIS_PORT_R);
	stepMotor = new CanTalonSRX(CHASSIS_PORT_ST);
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
		
		state->invertedMotors[0] = -1;
		state->invertedMotors[1] = -1;
		state->invertedStepMotor = 1;
		state->reverse = True;

		state->sensitivity = 0.5f;
		state->chassisMagnitude = DEF_SPEED;
		state->chassisEnabled = True;
		leftEncoder->Reset();
		rightEncoder->Reset();
	}

	for(U32 i = 0; i < state->nMotors; i++){
		motors[i]->Set(state->motorValues[i] * state->invertedMotors[i]);
	}
	stepMotor->Set(state->stepMotorValue * state->invertedStepMotor);

	S32 leftEncoderVal = leftEncoder->Get();
	S32 rightEncoderVal = rightEncoder->Get();
	state->dLeftEncoder = leftEncoderVal - state->leftEncoder;
	state->leftEncoder = leftEncoderVal;
	state->dRightEncoder = rightEncoderVal - state->rightEncoder;
	state->rightEncoder = rightEncoderVal;

	//Cout("%.4f || %.4f || %.4f|| %.4f", state->motorValues[0], 
	//			 state->motorValues[1], state->motorValues[2], state->motorValues[3]);
	//Cout("%.4f", state->stepMotorValue * state->invertedStepMotor);
	SmartDashboard::PutNumber("FLD", state->motorValues[0] * state->invertedMotors[0]);
	SmartDashboard::PutNumber("BLD", state->motorValues[0] * state->invertedMotors[0]);
	SmartDashboard::PutNumber("FRD", state->motorValues[1] * state->invertedMotors[1]);
	SmartDashboard::PutNumber("BRD", state->motorValues[1] * state->invertedMotors[1]);
	SmartDashboard::PutNumber("STP", state->stepMotorValue * state->invertedStepMotor);
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

CanTalonSRX* outerIntakeMotor;
Talon* innerLowerIntakeMotor;
Victor* lowerShooterMotor;
Talon* innerUpperIntakeMotor;
Victor* upperShooterMotor;

void InitializeShooter(){
	outerIntakeMotor = new CanTalonSRX(SHOOTER_PORT_OI);
	innerLowerIntakeMotor = new Talon(SHOOTER_PORT_ILI);
	lowerShooterMotor = new Victor(SHOOTER_PORT_LS);
	innerUpperIntakeMotor = new Talon(SHOOTER_PORT_IUI);
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
		state->innerLowerIntakeInversion = -1;
		state->innerUpperIntakeInversion = -1;
		state->lowerShooterInversion = -1;
		state->upperShooterInversion = -1;
	}

	outerIntakeMotor->Set(state->outerIntakeValue * state->outerIntakeInversion);
	innerLowerIntakeMotor->Set(state->innerLowerIntakeValue * 
							   state->innerLowerIntakeInversion);
	innerUpperIntakeMotor->Set(state->innerUpperIntakeValue * 
							   state->innerUpperIntakeInversion);
	lowerShooterMotor->Set(state->lowerShooterValue * state->lowerShooterInversion);
	upperShooterMotor->Set(state->upperShooterValue * state->upperShooterInversion);
#if 0
	Cout("%.4f || %.4f || %.4f || %.4f || %.4f", state->outerIntakeValue *
		 state->outerIntakeInversion, state->innerLowerIntakeValue * 
		 state->innerLowerIntakeInversion, state->innerUpperIntakeValue * 
		 state->innerUpperIntakeInversion, state->lowerShooterValue * 
		 state->lowerShooterInversion, state->upperShooterValue * state->upperShooterInversion);
#endif
	SmartDashboard::PutNumber("OI", state->outerIntakeValue * state->outerIntakeInversion);
	SmartDashboard::PutNumber("ILI", state->innerLowerIntakeValue * 
							 state->innerLowerIntakeInversion);
	SmartDashboard::PutNumber("IUI", state->innerUpperIntakeValue * 
							 state->innerUpperIntakeInversion);
	SmartDashboard::PutNumber("LS", state->lowerShooterValue * state->lowerShooterInversion);
	SmartDashboard::PutNumber("US", state->upperShooterValue * state->upperShooterInversion);
}

void TerminateShooter(){
	delete outerIntakeMotor;
	delete innerLowerIntakeMotor;
	delete innerUpperIntakeMotor;
	delete lowerShooterMotor;
	delete upperShooterMotor;
}