#pragma once

#include "ALMath.h"
#include "Geometry.h"

class Model
{
private:
	std::vector<Geometry*> geos;
	Mat4 transform;
	Mat4 normalTransform;
	Vec4 color;
	Vec4 normalColor;

	// BBox parameters
	bool isBBoxOn;
	std::vector<Poly*> bbox;
	Vec4 minCoord;
	Vec4 maxCoord;

	// Normal parameters
	bool vertexNormals;
	bool polyNormals;

public:
	Model() : color(AL_WHITE), isBBoxOn(false), vertexNormals(false), polyNormals(false),
			normalColor(AL_RED) { }
	~Model();

	void AddGeometry(Geometry* geo);

	void SetTranform(const Mat4& T);
	const Mat4& GetTransform() const;
	const Mat4& GetNormalTransform() const;
	void Translate(const Mat4& T);
	void Scale(const Mat4& S);
	void Rotate(const Mat4& R);

	const std::vector<Geometry*>& GetGeometries() const;
	const std::vector<Poly*>& GetBBox() const;

	void SetColor(int r, int g, int b);
	void SetColor(const Vec4& color);
	const Vec4& GetColor() const;

	void SetNormalColor(int r, int g, int b);
	void SetNormalColor(const Vec4& color);
	const Vec4& GetNormalColor() const;

	void SetBBox(bool isBBoxOn);
	bool IsBBoxOn() const;
	void BuildBoundingBox();

	void SetNormals(bool vertexNormals, bool polyNormals);
	bool AreVertexNormalsOn() const;
	bool ArePolyNormalsOn() const;
	
	//void Draw();
};

