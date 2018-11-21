#pragma once

#include "ALMath.h"
#include "Geometry.h"

class Model
{
private:
	std::vector<Geometry*> geos;
	Mat4 transform;
	Vec4 color;

	bool isBBoxOn;
	std::vector<Poly*> bbox;
	Vec4 minCoord;
	Vec4 maxCoord;

public:
	Model() : color(AL_WHITE), isBBoxOn(false) { }
	~Model();

	void AddGeometry(Geometry* geo);

	void SetTranform(const Mat4& T);
	const Mat4& GetTransform() const;
	const std::vector<Geometry*>& GetGeometries() const;
	const std::vector<Poly*>& GetBBox() const;

	void SetColor(int r, int g, int b);
	void SetColor(const Vec4& color);
	const Vec4& GetColor() const;

	void SetBBox(bool isBBoxOn);
	bool IsBBoxOn() const;
	void BuildBoundingBox();
	
	//void Draw();
};

