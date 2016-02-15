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