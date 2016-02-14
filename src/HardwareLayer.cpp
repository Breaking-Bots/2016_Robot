/* 
 * HardwareLayer.cpp
 * 
 * Created on: Feb 7, 2016
 * Author: Zaeem
 *
 */

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <unistd.h>
#include <cstdio>
#include <cstdarg> 
#include "WPILib.h"
#include "CHRISTOPHEngine.h"
#include "HardwareLayer.h"

/* Util */

uul_extern{


void InitializeLogging(){

}

LOGGING_CALLBACK(Cout){
	const char* formattedCStr = format.c_str();
	char* fmt = new char[strlen(formattedCStr) + 15];
	strcpy(fmt, "[CHRISTOPH] ");
	strcpy(fmt + 12, formattedCStr);
	strcpy(fmt + strlen(fmt), "\n");
	va_list args;
	va_start(args, format);
	S32 result = vprintf(fmt, args);
	va_end(args);
	return result;
}

LOGGING_CALLBACK(Cerr){
	const char* formattedCStr = format.c_str();
	char* fmt = new char[strlen(formattedCStr) + 11];
	strcpy(fmt, "[ERROR] ");
	strcpy(fmt + 8, formattedCStr);
	strcpy(fmt + strlen(fmt), "\n");
	va_list args;
	va_start(args, format);
	S32 result = vprintf(fmt, args);
	va_end(args);
	return result;
}

void TerminateLogging(){

}

SYSTEM_TIME_CALLBACK(SystemTime){
	return GetFPGATime() / 1000.0;
}

F32_CALLBACK_F32_F32_F32(Clamp){
	if(a < b) return b;
	else if(a > c) return c;
	return a;
}

F32_CALLBACK_F32(Clamp01){
	if(x < 0.0f) return 0.0f;
	else if(x > 1.0f) return 1.0f;
	return x;
}

F32_CALLBACK_F32(ClampN){
	if(x < -1.0f) return -1.0f;
	else if(x > 1.0f) return 1.0f;
	return x;
}

F32_CALLBACK_F32_F32(Max){
	if(b > a) return b;
	return a;
}

F32_CALLBACK_F32(Sq){
	return x * x;
}

F32_CALLBACK_F32(Cu){
	return x * x * x;
}


F32_CALLBACK_F32(Qu){
	return x * x * x * x;
}

S32_CALLBACK_F32(Sgn){
	return (0 < x) - (x < 0);
}

F32_CALLBACK_F32(NormalizeAlpha){
	return 0.5f * x + 0.5f;
}

F32_CALLBACK_F32_F32_F32(Lerp){
	return (1.0f - c) * a + c * b;
}

F32_CALLBACK_F32_F32_F32(Coserp){
	F32 alpha = (1.0f - cosf(c * PI)) * 0.5f;
	return (1.0f - alpha) * a + alpha * b;
}

F32_CALLBACK_F32_F32_F32_F32(SystemMagnitudeInterpolation){
	if(d < 0.0f){
		d++;
		return Lerp(a, b, d);
	}else{
		return Lerp(b, c, d);
	}
}

F32_CALLBACK_F32(PrincipleAngleDeg){
	return x - (S32)(x/360) * 360; //TODO: Test
}

F32_CALLBACK_F32(PrincipleAngleRad){
	return x - (S32)(x/TAU) * TAU; //TODO: Test
}

F32_CALLBACK_F32(MinDistAngleDeg){
	return ((x - 180.0f) - (S32)((x - 180.0f)/360) * 360); //TODO: Test
}

F32_CALLBACK_F32(MinDistAngleRad){
	return (x - (S32)(x/TAU) * TAU) - PI; //TODO: Test
}

F32_CALLBACK_F32_F32(AngularDistDeg){
	return MinDistAngleDeg(b - a);
}

F32_CALLBACK_F32_F32(AngularDistRad){
	return MinDistAngleRad(b - a);
}

inline U32 KiB(U32 x){
	return x * 1024;
}

inline U32 MiB(U32 x){
	return KiB(x) * 1024;
}

inline U32 GiB(U32 x){
	return MiB(x) * 1024;
}

inline U32 TiB(U32 x){
	return GiB(x) * 1024;
}

U32 GetFileSize(std::string filename, HANDLE fd, B32 useFD, B32 ignoreFailure){
	struct stat statBuffer;
	U32 result = 0;
	if(!useFD){
		fd = open(filename.c_str(), O_RDONLY);
	}
	if(fd != -1){
		fstat(fd, &statBuffer);
		result = statBuffer.st_size;
		if(!useFD){
			close(fd);
		}
	}
	return result;
}

File ReadEntireFile(std::string filename, B32 ignoreFailure){
	File result = {};
	struct stat statBuffer;

	HANDLE fileDescriptor = open(filename.c_str(), O_RDONLY);
	if(fileDescriptor != -1){
		result.size = GetFileSize(filename, fileDescriptor, True, ignoreFailure);
		if(result.size){
			Cout("File opened: \"%s\"; Size: %lu", filename.c_str(), result.size);
			result.data = mmap(NULL, result.size, PROT_READ | PROT_WRITE, MAP_PRIVATE, 
							   fileDescriptor, 0);
			if(result.data != MAP_FAILED){
				U32 bytesRead = read(fileDescriptor, result.data, result.size);
				if(bytesRead != result.size){
					Cerr("File allocation failed: File: \"%s\"; Size %lu", filename.c_str(), 
						 result.size);
					munmap(result.data, result.size);
					result.data = NULL;
					result.size = 0;
				}
			}else{
				if(!ignoreFailure){
					Cerr("File request allocation failed: File \"%s\"; Size %lu", 
						 filename.c_str(), result.size);
				}
			}
		}else{
			if(!ignoreFailure){
				Cerr("File request failed with size of 0; File: \"%s\"", filename.c_str());
			}
		}
		close(fileDescriptor);
	}
	return result;
}

B32 WriteEntireFile(std::string filename, U32 memorySize, void* memory, B32 ignoreFailure){
	B32 result = false;

	HANDLE fileDescriptor = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | 
								 S_IRWXG | S_IRWXO);
	if(fileDescriptor != -1){
		U32 bytesWritten = write(fileDescriptor, memory, memorySize);
		result = bytesWritten == memorySize;
		close(fileDescriptor);
	}else{
		if(!ignoreFailure){
			S32 err = errno;
			Cerr("Could not create file: \"%s\"", filename.c_str());
			Cerr("Errno: %d", err);
		}
	}

	return result;
}

B32 CopyFile(std::string src, std::string dest, B32 ignoreFailure){
	File srcFile = ReadEntireFile(src, ignoreFailure);
	return WriteEntireFile(dest, srcFile.size, srcFile.data, ignoreFailure);
}

S64 GetLastWriteTime(std::string filename, B32 ignoreFailure){
	S64 result = 0;
	struct stat statBuffer;
	if(!stat(filename.c_str(), &statBuffer)){
		result = statBuffer.st_mtim.tv_sec;
	}else{
		if(!ignoreFailure){
			Cerr("Could not get stat of file: %s", filename.c_str());
		}
	}
	return result;
}

/* CHRISTOPH Engine Management */

CHRISTOPH_CALLBACK(CHRISTOPHCallbackStub){
	Cout("STUB");
}


CHRISTOPHEngine LoadCHRISTOPHEngine(std::string filename){
	CHRISTOPHEngine result = {};

	result.lastWriteTime = GetLastWriteTime(filename);
	if(CopyFile(filename, CHRISTOPHEngineTempBinary)){

		result.module = dlopen(CHRISTOPHEngineTempBinary, RTLD_NOW);
		if(result.module){
			//Load real functions
			result.InitTeleop = (CHRISTOPHCallback*)dlsym(result.module, "InitTeleop");
			result.TeleopCallback = (CHRISTOPHCallback*)dlsym(result.module, 
									"TeleopCallback");
			result.InitTest = (CHRISTOPHCallback*)dlsym(result.module, "InitTest");
			result.TestCallback = (CHRISTOPHCallback*)dlsym(result.module, 
								  "TestCallback");
			result.InitAutonomous = (CHRISTOPHCallback*)dlsym(result.module, 
									"InitAutonomous");
			result.AutonomousCallback = (CHRISTOPHCallback*)dlsym(result.module, 
										"AutonomousCallback");
			result.InitDisabled = (CHRISTOPHCallback*)dlsym(result.module, 
								  "InitDisabled");
			result.DisabledCallback = (CHRISTOPHCallback*)dlsym(result.module, 
									  "DisabledCallback");
			result.InitTemp = (CHRISTOPHCallback*)dlsym(result.module, "InitTemp");
			result.TempCallback = (CHRISTOPHCallback*)dlsym(result.module, "TempCallback");

			result.isValid = (result.InitTeleop && result.TeleopCallback && result.InitTest &&
							  result.TestCallback && result.InitAutonomous && 
							  result.AutonomousCallback && result.InitDisabled && 
							  result.DisabledCallback);
		}else{
			Cerr("Invalid CHRISTOPHEngine binary: \"%s\"", CHRISTOPHEngineBinary);
		}
	}else{
		Cerr("Could not copy \"%s\" into \"%s\"", CHRISTOPHEngineBinary, 
			 CHRISTOPHEngineTempBinary);
	}

	if(!(result.isValid)){
		//Initialize to stub functions
		result.InitTeleop = CHRISTOPHCallbackStub;
		result.TeleopCallback = CHRISTOPHCallbackStub;
		result.InitTest = CHRISTOPHCallbackStub;
		result.TestCallback = CHRISTOPHCallbackStub;
		result.InitAutonomous = CHRISTOPHCallbackStub;
		result.AutonomousCallback = CHRISTOPHCallbackStub;
		result.InitDisabled = CHRISTOPHCallbackStub;
		result.InitTemp = CHRISTOPHCallbackStub;
		result.TempCallback = CHRISTOPHCallbackStub;
	}else{
		Cout("CHRISTOPHEngine successfully loaded!");
	}

	return result;
}


void UnloadCHRISTOPHEngine(CHRISTOPHEngine* engine){
	if(engine->module){
		dlclose(engine->module);
	}

	engine->isValid = False;
	engine->InitTeleop = CHRISTOPHCallbackStub;
	engine->TeleopCallback = CHRISTOPHCallbackStub;
	engine->InitTest = CHRISTOPHCallbackStub;
	engine->TestCallback = CHRISTOPHCallbackStub;
	engine->InitAutonomous = CHRISTOPHCallbackStub;
	engine->AutonomousCallback = CHRISTOPHCallbackStub;
	engine->InitDisabled = CHRISTOPHCallbackStub;
	engine->InitTemp = CHRISTOPHCallbackStub;
	engine->TempCallback = CHRISTOPHCallbackStub;
}

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

/* Input */

intern void ProcessDigitalButton(U32 buttonBitSet, ButtonState* oldState, U32 buttonBit, 
								 ButtonState* newState){
	newState->endedDown = ((buttonBitSet & buttonBit) == buttonBit);
	newState->halfTransitionCount = (oldState->endedDown != newState->endedDown) ? 1 : 0;
}

intern void ProcessStickInput(F32* _x, F32* _y){
	F32 x = *_x;
	F32 y = *_y;
	F32 mgntdSquared = x*x+y*y;

	if(mgntdSquared < DEADZONE_SQ){
		x = 0.0f;
		y = 0.0f;
	}else{
		F32 mgntd = sqrtf(mgntdSquared);
		F32 nlxFactor = x / mgntd;
		F32 nlyFactor = y / mgntd;
		if(mgntd > 1.0f){
			mgntd = 1.0f;
		}
		mgntd -= DEADZONE;
		x = nlxFactor * mgntd;
		y = nlyFactor * mgntd;
		x = x / (1 - DEADZONE * nlxFactor);
		y = y / (1 - DEADZONE * nlyFactor);
	}

	*_x = x;
	*_y = y;
}

intern void ProcessTriggerInput(F32* _t){
	F32 t = *_t;
	if(t < TRIGGER_DEADZONE){
		t = 0.0f;
	}else{
		t = (t - TRIGGER_DEADZONE * Sgn(t))/(1.0f - TRIGGER_DEADZONE);
	}

	*_t = t;
}

void UpdateInput(DriverStation* ds, Input* newInput, Input* oldInput){


	for(U32 i = 0; i < NUM_GAMEPADS; i++){

		Gamepad* newGamepad = GetGamepad(newInput, i);
		Gamepad* oldGamepad = GetGamepad(oldInput, i);

		//Processing buttons
		U32 buttonBitSet = ds->GetStickButtons(i);
		//Cout("%d", buttonBitSet);

		for(U32 j = 0; j < NUM_BUTTONS; j++){
			int buttonBit = 1 << j;
			ProcessDigitalButton(buttonBitSet, &oldGamepad->buttons[j], buttonBit, 
								 &newGamepad->buttons[j]);
		}

		//Cout("%d", newGamepad->x.endedDown);

		//Processing DPAD values
		S32 dpad = ds->GetStickPOV(i, 0);

		if(dpad != -1){
			ProcessDigitalButton((dpad >= 315 || dpad <= 45)? 1 : 0, &oldGamepad->up, 1, 
								 &newGamepad->up);
			ProcessDigitalButton((dpad >= 45 && dpad <= 135)? 1 : 0, &oldGamepad->right, 1, 
								 &newGamepad->right);
			ProcessDigitalButton((dpad >= 135 && dpad <= 225)? 1 : 0, &oldGamepad->down, 1, 
								 &newGamepad->down);
			ProcessDigitalButton((dpad >= 225 && dpad <= 315)? 1 : 0, &oldGamepad->left, 1,
								 &newGamepad->left);
		}

		//Circular deadzone processing of left joystick
		F32 lx = ds->GetStickAxis(i, _LX);
		F32 ly = ds->GetStickAxis(i, _LY);

		ProcessStickInput(&lx, &ly);
		newGamepad->lx = lx;
		newGamepad->ly = ly;

		//Circular deadzone processing of right joystick
		F32 rx = ds->GetStickAxis(i, _RX);
		F32 ry = ds->GetStickAxis(i, _RY);

		ProcessStickInput(&rx, &ry);
		newGamepad->rx = rx;
		newGamepad->ry = ry;

		//Linear deadzone processing of left trigger
		F32 lt = ds->GetStickAxis(i, _LT);
		ProcessTriggerInput(&lt);
		newGamepad->lt = lt;

		//Linear deadzone processing of right trigger
		F32 rt = ds->GetStickAxis(i, _RT);
		ProcessTriggerInput(&rt);
		newGamepad->rt = rt;
	}
}

}

/* CHRISTOPH Class */

U32 CHRISTOPH_PERMANENT_STORAGE_SIZE = (MiB(1));
U32 CHRISTOPH_TRANSIENT_STORAGE_SIZE = (MiB(1));
U32 CHRISTOPH_TOTAL_STORAGE_SIZE  = (CHRISTOPH_PERMANENT_STORAGE_SIZE +
									 CHRISTOPH_TRANSIENT_STORAGE_SIZE);

CHRISTOPH::CHRISTOPH(){

}

void CHRISTOPH::RobotInit(){
	
}

void CHRISTOPH::RobotMain(){
	//Init
	LiveWindow* lw = LiveWindow::GetInstance();
	DriverStation* ds = &DriverStation::GetInstance();
	lw->SetEnabled(True);

	Cout("Initializing");

	CHRISTOPHMemory christophMemory = {};
	HLState hlState = {};

#if 1
	//TODO: Check if this works with Just 3 Billion
	void* baseAddress = rcast<void*>(U32((GiB(4) - 1) - 
						((CHRISTOPH_TOTAL_STORAGE_SIZE / getpagesize()) + 1) * getpagesize()));
#else
	void* baseAddress = NULL;
#endif

	christophMemory.permanentStorageSize = CHRISTOPH_PERMANENT_STORAGE_SIZE;
	christophMemory.transientStorageSize = CHRISTOPH_TRANSIENT_STORAGE_SIZE;
	christophMemory.Cout = Cout;
	christophMemory.Cerr = Cerr;
	christophMemory.SystemTime = SystemTime;
	christophMemory.Clamp = Clamp;
	christophMemory.Clamp01 = Clamp01;
	christophMemory.ClampN = ClampN;
	christophMemory.Max = Max;
	christophMemory.Sq = Sq;
	christophMemory.Qu = Qu;
	christophMemory.NormalizeAlpha = NormalizeAlpha;
	christophMemory.Lerp = Lerp;
	christophMemory.Coserp = Coserp;
	christophMemory.SystemMagnitudeInterpolation = SystemMagnitudeInterpolation;
	christophMemory.PrincipleAngleDeg = PrincipleAngleDeg;
	christophMemory.PrincipleAngleRad = PrincipleAngleRad;
	christophMemory.MinDistAngleDeg = MinDistAngleDeg;
	christophMemory.MinDistAngleRad = MinDistAngleRad;
	christophMemory.AngularDistDeg = AngularDistDeg;
	christophMemory.AngularDistRad = AngularDistRad;

	hlState.totalSize = CHRISTOPH_TOTAL_STORAGE_SIZE;
	hlState.totalCHRISTOPHMemoryBlock = mmap(baseAddress, hlState.totalSize, 
											 PROT_READ | PROT_WRITE,
											 MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if(hlState.totalCHRISTOPHMemoryBlock != MAP_FAILED){
		Cout("Total CHRISTOPH memory successfully allocated with size of %lu Bytes", 
			 hlState.totalSize);
	}else{
		S32 err = errno;
		Cerr("Total CHRISTOPH memory allocation failed with size of %lu Bytes", 
			 hlState.totalSize);
		Cerr("Errno: %d", err);
	}

	christophMemory.permanentStorage = hlState.totalCHRISTOPHMemoryBlock;
	christophMemory.transientStorage = ((U8*)christophMemory.permanentStorage + 
									    christophMemory.permanentStorageSize);

	//Input initialization
	Input inputs[2];
	inputs[0] = {};
	inputs[1] = {};
	Input* newInput = &inputs[0];
	Input* oldInput = &inputs[1];

	//Chassis initialization
	InitializeChassis();

	//Shooter initialization
	InitializeShooter();

	//CHRISTOPHEngine startup
	CHRISTOPHEngine engine = LoadCHRISTOPHEngine(CHRISTOPHEngineBinary);
	U32 loadCounter = 0;
	B32 autonomousInit = False;
	B32 teleopInit = False;
	B32 testInit = False;
	B32 disabledInit = False;
	B32 tempInit = False;

	CHRISTOPHState* christophState = scast<CHRISTOPHState*>(christophMemory.permanentStorage);
	ChassisState* chassisState = &(christophState->chassisState);

	F64 dt = 1.0f / CORE_THREAD_HZ;
	F64 targetMSPerFrame = 1000.0 * dt;
	F64 lastTime = SystemTime();
	for(;;) {
		//Reload CHRISTOPHEngine
		U32 newCHRISTOPHEngineWriteTime = GetLastWriteTime(CHRISTOPHEngineBinary);
		//Cout("%d", newCHRISTOPHEngineWriteTime);
		U32 libCHRISTOPHFileSize = GetFileSize(CHRISTOPHEngineBinary);
		U32 libCHRISTOPH_tempFileSize = GetFileSize(CHRISTOPHEngineTempBinary);
		if((newCHRISTOPHEngineWriteTime != engine.lastWriteTime) || (libCHRISTOPHFileSize !=
		   libCHRISTOPH_tempFileSize)){
			UnloadCHRISTOPHEngine(&engine);
			engine = LoadCHRISTOPHEngine(CHRISTOPHEngineBinary);
			loadCounter = 0;
			christophMemory.isInitialized = False;
			tempInit = False;
		}

		UpdateInput(ds, newInput, oldInput);

		//Executing user function based on robot state
		if(IsAutonomous() && IsEnabled()){
			if(!autonomousInit){

				lw->SetEnabled(False);
				ds->InAutonomous(True);
				ds->InOperatorControl(False);
				ds->InTest(False);
				ds->InDisabled(False);
				engine.InitAutonomous(&christophMemory, newInput, dt);
				autonomousInit = True;
				teleopInit = False;
				testInit = False;
				disabledInit = False;

			}
			//Autonomous Iterative Dytor
			engine.AutonomousCallback(&christophMemory, newInput, dt);

		}else if(IsOperatorControl() && IsEnabled()){
			if(!teleopInit){
				lw->SetEnabled(False);
				ds->InAutonomous(False);
				ds->InOperatorControl(True);
				ds->InTest(False);
				ds->InDisabled(False);
				engine.InitTeleop(&christophMemory, newInput, dt);
				autonomousInit = False;
				teleopInit = True;
				testInit = False;
				disabledInit = False;
			}
			//Teleop Iterative Dytor
			engine.TeleopCallback(&christophMemory, newInput, dt);

		}else if(IsTest() && IsEnabled()){

			if(!testInit){
				lw->SetEnabled(True);
				ds->InAutonomous(False);
				ds->InOperatorControl(False);
				ds->InTest(True);
				ds->InDisabled(False);
				engine.InitTest(&christophMemory, newInput, dt);
				autonomousInit = False;
				teleopInit = False;
				testInit = True;
				disabledInit = False;
			}
			//Test Iterative Dytor
			engine.TestCallback(&christophMemory, newInput, dt);

		}else if(IsDisabled()){
			if(!disabledInit){
				lw->SetEnabled(False);
				ds->InAutonomous(False);
				ds->InOperatorControl(False);
				ds->InTest(False);
				ds->InDisabled(True);
				engine.InitDisabled(&christophMemory, newInput, dt);
				autonomousInit = False;
				teleopInit = False;
				testInit = False;
				disabledInit = True;
			}
			//Disabled Iterative Dytor
			engine.DisabledCallback(&christophMemory, newInput, dt);

		}

		UpdateChassis(&christophMemory);
		UpdateShooter(&christophMemory);

		//Time Processing
		F64 elapsedMS = SystemTime() - lastTime;
		if(elapsedMS < targetMSPerFrame) {
			Wait((targetMSPerFrame - elapsedMS) / 1000.0f);
			F64 testElapsedMS = SystemTime() - lastTime;

			if(testElapsedMS > targetMSPerFrame) {
				//Cerr("Core Thread Runtime waited too long.");
			} else {
				do {
					elapsedMS = SystemTime() - lastTime;
				} while(elapsedMS <= targetMSPerFrame);
			}

		} else {
			Cout("Missed last Core Thread Runtime frame.");
		}

		//Flip inputs
		Input* tempInput = newInput;
		newInput = oldInput;
		oldInput = tempInput;

		F64 endTime = SystemTime();
		F64 frameTimeMS = endTime - lastTime;
		lastTime = endTime;
		F64 hz = 1000.0 / frameTimeMS;
		
		//Frame logging
		//Cout("Last Core Thread frame time: %.04fms (%.04fHz).", frameTimeMS, hz);
	}

	//System shutdown
	TerminateChassis();
	TerminateShooter();

	//Memory shutdown
	munmap(hlState.totalCHRISTOPHMemoryBlock, CHRISTOPH_TOTAL_STORAGE_SIZE);

}

CHRISTOPH::~CHRISTOPH(){
	TerminateLogging();
}

int main(){
	//Logging startup
	InitializeLogging();
	
	Cout("Logging Initialized!");
 	if(!HALInitialize()){
 		//TODO: log
 		return -1;
 	}
 	HALReport(HALUsageReporting::kResourceType_Language, 
 		      HALUsageReporting::kLanguage_CPlusPlus);

 	CHRISTOPH* christoph = new CHRISTOPH();
 	RobotBase::robotSetup(christoph);

 	return 0;
}
