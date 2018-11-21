#pragma once

#include "ALMath.h"

class Camera
{
private:
	Mat4 cTransform;
	Mat4 projection;

	Mat4 perspective;
	Mat4 orthographic;
	bool isPerspective;

public:
	Camera();
	~Camera();

	void Translate(const Mat4& T);
	void Scale(const Mat4& S);
	void RotateX(const Mat4& R);
	void RotateY(const Mat4& R);
	void RotateZ(const Mat4& R);
	Mat4 GetTranform() const;

	void SetOrthographic(double left, double right, double top, double bottom, double z_near, double z_far);
	void SetPerspective(double fovy, double aspectR, double z_near, double z_far);
	Mat4 GetProjection() const;

	void SwitchProjection(bool isPerspective);
	bool IsPerspective() const;

	void LookAt(const Vec4& eye, const Vec4& at, const Vec4& up);
};

