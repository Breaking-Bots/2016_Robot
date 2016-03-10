intern void ProcessDigitalButton(U32 buttonBitSet, ButtonState* oldState, 
								 U32 buttonBit, ButtonState* newState){
	newState->endedDown = ((buttonBitSet & buttonBit) == buttonBit);
	newState->halfTransitionCount = (oldState->endedDown != newState->endedDown) ?
									 1 : 0;
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
		x = x / (1 - DEADZONE * nlxFactor * Sgn(x));
		y = y / (1 - DEADZONE * nlyFactor * Sgn(y));
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

		//if(dpad != -1){
			ProcessDigitalButton((dpad >= 315 || dpad <= 45)? 1 : 0, 
								 &oldGamepad->up, 1, 
								 &newGamepad->up);
			ProcessDigitalButton((dpad >= 45 && dpad <= 135)? 1 : 0, 
								 &oldGamepad->right, 1, 
								 &newGamepad->right);
			ProcessDigitalButton((dpad >= 135 && dpad <= 225)? 1 : 0, 
								 &oldGamepad->down, 1, 
								 &newGamepad->down);
			ProcessDigitalButton((dpad >= 225 && dpad <= 315)? 1 : 0,
								 &oldGamepad->left, 1,
								 &newGamepad->left);
		//}

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

		//Cout("%.4f || %.4f || %.4f || %.4f", lx, ly, rx, ry);
	}
}

void ProcessHLInputProtocols(HLState* state, Input* input){
	for(U32 i = 0; i < NUM_GAMEPADS; i++){
		if(input->gamepads[i].start.endedDown && 
		   input->gamepads[i].start.halfTransitionCount){
			
			#if RECORDED_AUTON

			if(state->recordingHandle || state->recordingIndex){
				EndInputRecording(state);
			}
			if(state->playbackHandle || state->playbackIndex){
				EndInputPlayback(state);
			}

			BeginInputRecording(state, 1);

			#endif

		}else if(input->gamepads[i].start.endedDown && 
		   		 input->gamepads[i].start.halfTransitionCount){

			#if RECORDED_AUTON

			if(state->recordingHandle || state->recordingIndex){
				EndInputRecording(state);
			}
			if(state->playbackHandle || state->playbackIndex){
				EndInputPlayback(state);
			}
			
			BeginInputPlayback(state, 1);

			#endif

		}
	}
}