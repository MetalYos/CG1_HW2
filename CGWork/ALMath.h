#pragma once

#include "Vec4.h"
#include "Mat4.h"

#define AL_DBL_EPSILON 0.000001
#define AL_PI 3.14159265359

#define AL_WHITE Vec4(255, 255, 255)
#define AL_BLACK Vec4(0, 0, 0)
#define AL_RED Vec4(255, 0, 0)
#define AL_GREEN Vec4(0, 255, 0)
#define AL_BLUE Vec4(0, 0, 255)
#define AL_RAINBOW Vec4((BYTE)rand() % 255, (BYTE)rand() % 255, (BYTE)rand() % 255)

#define AL_RAINBOW_CREF RGB((BYTE)rand() % 255, (BYTE)rand() % 255, (BYTE)rand() % 255)

inline double ToRadians(double angleInDegrees)
{
	return (angleInDegrees / 180.0) * AL_PI;
}

inline double ToDegrees(double angleInRadians)
{
	return (angleInRadians * 180.0) / AL_PI;
}