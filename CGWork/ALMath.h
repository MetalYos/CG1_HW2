#pragma once

#include "Vec4.h"
#include "Mat4.h"

#define AL_DBL_EPSILON 0.000001
#define AL_PI 3.14159265359

#define AL_WHITE Vec4(255, 255, 255)
#define AL_BLACK Vec4(255, 255, 255)

inline double ToRadians(double angleInDegrees)
{
	return (angleInDegrees / 180.0) * AL_PI;
}

inline double ToDegrees(double angleInRadians)
{
	return (angleInRadians * 180.0) / AL_PI;
}