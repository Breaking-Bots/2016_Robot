

void BeginInputRecording(HLState* state, U32 recordingIndex){
	if(!state->recordingIndex && !state->recordingHandle){

		state->recordingIndex = recordingIndex;
		char* filename = RECORDED_AUTON_FILENAME;

		state->recordingHandle = open(filename, O_WRONLY | O_CREAT | O_TRUNC,
									  S_IRWXU | S_IRWXG | S_IRWXO);
		Cout("Recording: [%s] from index %d", filename, recordingIndex);
	}
}

void EndInputRecording(HLState* state){
	if(state->recordingHandle){
		close(state->recordingHandle);
	}
	Cout("Stopped recording of index %d", state->recordingIndex);
	state->recordingIndex = 0;
}

void BeginInputPlayback(HLState* state, U32 playbackIndex){
	if(!state->playbackIndex && !state->playbackHandle){

		state->playbackIndex = playbackIndex;
		char* filename = RECORDED_AUTON_FILENAME;

		state->playbackHandle = open(filename, O_RDONLY, S_IRWXU | S_IRWXG | S_IRWXO);

		Cout("Playing back: [%s] from index %d", filename, playbackIndex);
	}
}

void EndInputPlayback(HLState* state){
	if(state->playbackHandle){
		close(state->playbackHandle);
	}
	Cout("Stopped playback of index %d", state->playbackIndex);
	state->playbackIndex = 0;
}

void RecordInput(HLState* state, Input* input){
	if(state->recordingHandle && state->recordingIndex){
		write(state->recordingHandle, input, sizeof(*input));
	}
}

void PlayBackInput(HLState* state, Input* input){
	if(state->playbackHandle && state->playbackIndex){
		if(read(state->playbackHandle, input, sizeof(*input)) < 1){
			#if LOOP_AUTON_PLAYBACK
				S32 err = errno;
				U32 playbackIndex = state->playbackIndex;
				EndInputPlayback(state);
				BeginInputPlayback(state, playbackIndex);
				read(state->playbackHandle, input, sizeof(*input));
				if(err != 32){
					Cerr("Read Error: %d", err);
				}
			#endif
		}
	}
}