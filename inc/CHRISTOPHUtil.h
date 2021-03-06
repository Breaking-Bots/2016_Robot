#ifndef CHRISTOPH_UTIL_H
#define CHRISTOPH_UTIL_H

#include <cstdint>
#include <cfloat>
#include <cmath>
#include <string>

#ifdef __cplusplus
#define uul_extern extern "C"
#else
#define uul_extern extern
#endif

#define lscope static
#define glob static
#define intern static
#define scast static_cast
#define rcast reinterpret_cast

typedef uint32_t B32;

typedef int8_t S8;
typedef int16_t S16;
typedef int32_t S32;
typedef int64_t S64;

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

typedef float F32;
typedef double F64;

#define S8_MIN INT8_MIN
#define S16_MIN INT16_MIN
#define S32_MIN INT32_MIN
#define S64_MIN INT64_MIN
#define F32_MIN FLT_MIN
#define F64_MIN DBL_MIN

#define S8_MAX INT8_MAX
#define S16_MAX INT16_MAX
#define S32_MAX INT32_MAX
#define S64_MAX INT64_MAX
#define U8_MAX UINT8_MAX
#define U16_MAX UINT16_MAX
#define U32_MAX UINT32_MAX
#define U64_MAX UINT64_MAX
#define F32_MAX FLT_MAX
#define F64_MAX DBL_MAX
#define PI (3.1415926535897932f)
#define TAU (6.2831853071795865f)
#define E (2.71828182845904523526f)
#define RAD_TO_DEG (57.2957795f)
#define DEG_TO_RAD (0.0174532925f)
#define True (0xFFFFFFFF)
#define False (0x00000000)
#define EPSILON (0.00001f)
#define EZERO (0.000000001f)

#define sizeofArr(arr) (sizeof((arr)) / sizeof((arr)[0]))

#define AlignPo2(value, alignment) ((value + ((alignment) - 1)) & ~((alignment) - 1))
#define Align4(value) ((value + 3) & ~3)
#define Align8(value) ((value + 7) & ~7)
#define Align16(value) ((value + 15) & ~15)

uul_extern {

#define F32_CALLBACK_F32_F32_F32_F32(name) F32 name(F32 a, F32 b, F32 c, F32 d)
typedef F32_CALLBACK_F32_F32_F32_F32(F32CallbackF32F32F32F32);

#define F32_CALLBACK_F32_F32_F32(name) F32 name(F32 a, F32 b, F32 c)
typedef F32_CALLBACK_F32_F32_F32(F32CallbackF32F32F32);

#define F32_CALLBACK_F32_F32(name) F32 name(F32 a, F32 b)
typedef F32_CALLBACK_F32_F32(F32CallbackF32F32);

#define F32_CALLBACK_F32(name) F32 name(F32 x)
typedef F32_CALLBACK_F32(F32CallbackF32);

#define S32_CALLBACK_F32(name) S32 name(F32 x)
typedef S32_CALLBACK_F32(S32CallbackF32);

#define U64_CALLBACK_U32(name) U64 name(U32 x)
typedef U64_CALLBACK_U32(U64CallbackU32);

/**
 * Logging dytor
 */
#define LOGGING_CALLBACK(name) S32 name(const std::string& format, ...)
typedef LOGGING_CALLBACK(LoggingCallback);

/**
 * System Time dytor
 */
#define SYSTEM_TIME_CALLBACK(name) F64 name()
typedef SYSTEM_TIME_CALLBACK(SystemTimeCallback);

struct CHRISTOPHMemory;

typedef struct CHRISTOPHTimer{
	U64 startTime;
	F32 targetSeconds;
	B32 isStarted;
	B32 endPulse;
} CHRISTOPHTimer;

void StartTimer(CHRISTOPHMemory* memory, U32 timerIndex, F32 targetSeconds);

B32 SufficientTimeElapsed(CHRISTOPHMemory* memory, U32 timerIndex);

B32 TimerEnded(CHRISTOPHMemory* memory, U32 timerIndex);

}

#endif