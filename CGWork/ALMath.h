#pragma once

#include "Vec4.h"
#include "Mat4.h"
#include <vector>

#define AL_DBL_EPSILON 0.000001
#define AL_PI 3.14159265359

#define AL_WHITE Vec4(255, 255, 255)
#define AL_BLACK Vec4(0, 0, 0)
#define AL_RED Vec4(255, 0, 0)
#define AL_GREEN Vec4(0, 255, 0)
#define AL_BLUE Vec4(0, 0, 255)
#define AL_RAINBOW Vec4((BYTE)rand() % 255, (BYTE)rand() % 255, (BYTE)rand() % 255)

#define AL_YELLO_CREF	RGB(255, 255, 0)
#define AL_RAINBOW_CREF RGB((BYTE)rand() % 255, (BYTE)rand() % 255, (BYTE)rand() % 255)

inline bool MinDbl(double a, double b)
{
	return a < b;
}

inline bool MaxDbl(double a, double b)
{
	return a >= b;
}

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

inline bool LineLineIntersection(const std::vector< std::vector<int> >& lineA,
	const std::vector< std::vector<int> >& lineB)
{
	// Poly points
	double x1 = lineA[0][0];
	double y1 = lineA[0][1];
	double x2 = lineA[1][0];
	double y2 = lineA[1][1];

	// Semi infinite line points
	double x3 = lineB[0][0];
	double y3 = lineB[0][1];
	double x4 = lineB[1][0];
	double y4 = lineB[1][1];

	double slope1 = (y2 - y1) / (x2 - x1);
	double slope2 = (y4 - y3) / (x4 - x3);
	if (abs(slope1 - slope2) < AL_DBL_EPSILON)
		return false;

	double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / 
		((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
	double u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / 
		((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));

	if (t < 0.0 || t > 1.0 || u < 0.0 || u > 1.0)
		return false;

	double x1_ans = x1 + (x2 - x1) * t;
	double y1_ans = y1 + (y2 - y1) * t;
	double x2_ans = x3 + (x4 - x3) * u;
	double y2_ans = y3 + (y4 - y3) * u;

	Vec4 p1_ans(x1_ans, y1_ans, 0.0, 0.0);
	Vec4 p2_ans(x2_ans, y2_ans, 0.0, 0.0);

	if (Vec4::Distance3(p1_ans, p2_ans) < AL_DBL_EPSILON)
		return true;

	return false;
}

inline bool PointPolyIntersection(const std::vector<int>& point, const std::vector< std::vector<int> >& poly)
{
	int counter = 0;
	for (unsigned int i = 0; i < poly.size(); i++)
	{
		std::vector< std::vector<int> > line_x;
		line_x.push_back(point);
		std::vector<int> point_far;
		point_far.push_back(100000);
		point_far.push_back(point[1]);
		line_x.push_back(point_far);

		std::vector< std::vector<int> > edge;
		edge.push_back(poly[i]);
		edge.push_back(poly[(i + 1) % poly.size()]);
		
		if (LineLineIntersection(edge, line_x))
			counter++;
	}
	
	if (counter % 2 == 0)
		return false;
	
	return true;
}
