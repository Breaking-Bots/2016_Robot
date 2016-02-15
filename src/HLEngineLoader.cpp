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
