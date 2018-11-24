#pragma once

#include "ALMath.h"

struct PerspectiveParams
{
	double Left;
	double Right;
	double Top;
	double Bottom;
	double Near;
	double Far;
	double FOV;
	double AspectRatio;
};

struct OrthographicParams
{
	double Left;
	double Right;
	double Top;
	double Bottom;
	double Near;
	double Far;
};

class Camera
{
private:
	Mat4 cTransform;
	Mat4 projection;

	Mat4 perspective;
	Mat4 orthographic;
	bool isPerspective;

	PerspectiveParams perspectiveParams;
	OrthographicParams orthographicParams;

public:
	Camera();
	~Camera();

	void Translate(Mat4& T);
	void Scale(Mat4& S);
	void Rotate(Mat4& R);
	Mat4 GetTranform() const;

	void SetOrthographic(double left, double right, double top, double bottom, double z_near, double z_far);
	void SetPerspective(double left, double right, double top, double bottom, double z_near, double z_far);
	void SetPerspective(double fovy, double aspectR, double z_near, double z_far);
	Mat4 GetProjection() const;

	const PerspectiveParams& GetPerspectiveParameters() const;
	const OrthographicParams& GetOrthographicParameters() const;

	void SwitchProjection(bool isPerspective);
	bool IsPerspective() const;

	void LookAt(const Vec4& eye, const Vec4& at, const Vec4& up);
};

