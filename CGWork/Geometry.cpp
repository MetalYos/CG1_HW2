#include "Geometry.h"

Geometry::~Geometry()
{
	// free polygons
	while (Polygons.size() > 0)
	{
		Poly* p = Polygons.back();
		Polygons.pop_back();
		delete p;
	}

	// free vertices
	for (auto it = Vertices.begin(); it != Vertices.end(); ++it)
		delete (*it).second;
	Vertices.clear();
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

void Geometry::CalculateVerticesNormals()
{
	for (auto it = Vertices.begin(); it != Vertices.end(); ++it)
	{
		Vertex* v = (*it).second;
		
		Vec4 avg;
		for (Poly* p : v->Polys)
		{
			avg += p->CalcNormal;
		}
		avg /= v->Polys.size();
		avg[3] = 0.0;

		if (abs(avg[0]) < AL_DBL_EPSILON && abs(avg[1]) < AL_DBL_EPSILON && abs(avg[2]) < AL_DBL_EPSILON)
			v->CalcNormal = v->Polys.back()->CalcNormal;
		else
			v->CalcNormal = Vec4::Normalize3(avg);
	}
}
