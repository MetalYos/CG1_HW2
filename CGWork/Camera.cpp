#include "Camera.h"



Camera::Camera() : isPerspective(false)
{
}


Camera::~Camera()
{
}

Mat4 Camera::GetTranform() const
{
	return cTransform;
}

void Camera::SetOrthographic(double left, double right, double top, double bottom, double near, double far)
{
	Mat4 result(Vec4(-2.0 / (right - left), 0.0, 0.0, 0.0),
		Vec4(0.0, -2.0 / (top - bottom), 0.0, 0.0),
		Vec4(0.0, 0.0, 2.0 / (near - far), 0.0),
		Vec4(-(right + left) / (right - left), -(top + bottom) / (top - bottom), (far + near) / (far - near), 1.0));
	projection = result;
	orthographic = result;
	isPerspective = false;
}

void Camera::SetPerspective(double fovy, double aspectR, double z_near, double z_far)
{
	double fovyRad = ToRadians(fovy / 2.0);

	Mat4 result;
	result[0][0] = 1.0 / (tan(fovyRad) * aspectR);
	result[1][1] = 1 / tan(fovyRad);
	result[2][2] = -(z_far + z_near) / (z_far - z_near);
	result[3][3] = 0.0;
	result[3][2] = -1.0;
	result[2][3] = -(2.0 * z_far * z_near) / (z_far - z_near);
	result.Transpose();
	
	projection = result;
	perspective = result;
	isPerspective = true;
}

Mat4 Camera::GetProjection() const
{
	return projection;
}

void Camera::SwitchProjection(bool isPerspective)
{
	projection = isPerspective ? perspective : orthographic;
	this->isPerspective = isPerspective;
}

bool Camera::IsPerspective() const
{
	return isPerspective;
}

void Camera::LookAt(const Vec4 & eye, const Vec4 & at, const Vec4 & up)
{
	Vec4 n = Vec4::Normalize3(eye);
	n[3] = 0;
	Vec4 u = Vec4::Normalize3(Vec4::Cross(up, n));
	u[3] = 0;
	Vec4 v = Vec4::Normalize3(Vec4::Cross(n, u));
	v[3] = 0;
	Mat4 c(u, v, n, Vec4(0.0, 0.0, 0.0, 1.0));
	c.Transpose();
	cTransform = c * Mat4::Translate(-eye);
	cTransform[3][3] = 1.0;

}
