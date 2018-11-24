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

inline Vec4 LinePlaneCollision(const Vec4& p1, const Vec4& p2, double A, double B, double C, double D)
{
	Vec4 result;
	Vec4 diff(p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2]);
	double numerator = A * p1[0] + B * p1[1] + C * p1[2] + D;
	double denominator = A * diff[0] + B * diff[1] + C * diff[2];
	for (int i = 0; i < 3; i++)
		result[i] = p1[i] - (diff[i] * numerator) / denominator;
	return result;
}