#pragma once

#include "ALMath.h"
#include <vector>
#include <unordered_map>
class Vec4Hash
{
public:
	std::size_t operator()(const Vec4& v) const
	{
		std::hash<double> hashVal;
		return hashVal(v[0]);
	}
};

class Vec4Comparer
{
public:
	bool operator()(const Vec4& u, const Vec4& v) const
	{
		return (Vec4::Distance3(u, v) < AL_DBL_EPSILON);
	}
};

struct Poly; // Forward Decleration

struct Vertex
{
	Vec4 Pos;
	Vec4 Normal;
	std::vector<Poly*> Polys;

	Vertex() { }
	explicit Vertex(const Vec4& v) : Pos(v) { }
};

struct Poly
{
	std::vector<Vertex*> Vertices;
	Vec4 Normal;
};

class Geometry
{
public:
	std::unordered_map<Vec4, Vertex*, Vec4Hash, Vec4Comparer> Vertices;
	std::vector<Poly*> Polygons;
	Vec4 MinCoord;
	Vec4 MaxCoord;

public:
	Geometry() { }
	~Geometry(); // TODO: complete

	void AddVertex(Vertex* v);
	void AddPolygon(Poly* p);
	void CalculatePolygonNormals();
};
