#include "Geometry.h"

Geometry::~Geometry()
{
	// free polygons
	for (Poly* p : Polygons)
		delete p;
	// free vertices
	for (auto it = Vertices.begin(); it != Vertices.end(); ++it)
		delete (*it).second;
}

void Geometry::AddVertex(Vertex* v)
{
	Vertices[v->Pos] = v;
	// Calculate new min and max
	if (Vertices.size() == 1)
	{
		MinCoord = v->Pos;
		MaxCoord = v->Pos;
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			if (v->Pos[i] < MinCoord[i])
				MinCoord[i] = v->Pos[i];
			if (v->Pos[i] > MaxCoord[i])
				MaxCoord[i] = v->Pos[i];
		}
	}
}

void Geometry::AddPolygon(Poly * p)
{
	Polygons.push_back(p);
}

void Geometry::CalculatePolygonNormals()
{
	Vec4 zero;
	for (auto it = Vertices.begin(); it != Vertices.end(); ++it)
	{
		Vertex* v = (*it).second;
		if (v->Normal == zero)
		{
			Vec4 avg;
			for (Poly* p : v->Polys)
			{
				avg += p->Normal;
			}
			avg /= v->Polys.size();
			avg[3] = 1.0;
			v->Normal = Vec4::Normalize3(avg);
		}
	}
}
