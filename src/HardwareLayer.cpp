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


uul_extern{

/* Util */

#include "HLUtil.cpp"

/* CHRISTOPH Engine Management */

#include "HLEngineLoader.cpp"

/* Subsystems */

#include "HLSubsystems.cpp"

/* Input */

#include "HLInput.cpp"

/* Fast Thread */

Task fastThread;

#if DISABLE_FAST_THREAD

void StartFastThread(CHRISTOPHMemory* memory){

}

#else

void StartFastThread(CHRISTOPHMemory* memory){
	cam->SetFPS(60);
	cam->SetBrightness(0);
	cam->SetSize(320, 240);
	cam->OpenCamera();
	cam->UpdateSettings();

	cam->StartCapture();

	F64 dt = 1.0f / FAST_THREAD_HZ;
	F64 targetMSPerFrame = 1000.0 * dt;
	F64 lastTime = SystemTime();
	for(;;) {

		//UpdateChassis(&christophMemory);
		//UpdateShooter(&christophMemory);

		//Time Processing
		F64 elapsedMS = SystemTime() - lastTime;
		if(elapsedMS < targetMSPerFrame) {
			Wait((targetMSPerFrame - elapsedMS) / 1000.0f);
			F64 testElapsedMS = SystemTime() - lastTime;

			if(testElapsedMS > targetMSPerFrame) {
				//Cerr("Fast Thread Runtime waited too long.");
			} else {
				do {
					elapsedMS = SystemTime() - lastTime;
				} while(elapsedMS <= targetMSPerFrame);
			}

		} else {
			//Cout("Missed last Fast Thread Runtime frame.");
		}


		F64 endTime = SystemTime();
		F64 frameTimeMS = endTime - lastTime;
		lastTime = endTime;
		F64 hz = 1000.0 / frameTimeMS;
		
		//Frame logging
		//Cout("Last Fast Thread frame time: %.04fms (%.04fHz).", frameTimeMS, hz);
	}

}

#endif

/* Recording */

#include "HLRecording.cpp"


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

	fastThread = Task("FastThread", &StartFastThread, &christophMemory);

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
		ProcessHLInputProtocols(&hlState, newInput);

		//Executing user function based on robot state
		if(IsAutonomous() && IsEnabled()){
			if(!autonomousInit){

				#if RECORDED_AUTON
				if(hlState.recordingHandle || hlState.recordingIndex){
					EndInputRecording(&hlState);
				}
				if(hlState.playbackHandle || hlState.playbackIndex){
					EndInputPlayback(&hlState);
				}
			
				BeginInputPlayback(&hlState, 1);
				#endif

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

			#if RECORDED_AUTON
			PlayBackInput(&ehlState, newInput);
			#endif

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
