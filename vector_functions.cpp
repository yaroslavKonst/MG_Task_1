#include "vector_functions.h"

bool triangle_ray_intersection(Vertex3 *vertices, Vertex3 origin, Vector3 dir,
		double &t, double &u, double &v)
{
	Vector3 CA = vertices[2] - vertices[0];
	Vector3 CB = vertices[2] - vertices[1];
	Vector3 CP = vertices[2] - origin;
	Vector3 D(dir);

	double delta = Matrix3x3<double>(
			D.X(), CA.X(), CB.X(),
			D.Y(), CA.Y(), CB.Y(),
			D.Z(), CA.Z(), CB.Z()).det();

	double delta_t = Matrix3x3<double>(
			CP.X(), CA.X(), CB.X(),
			CP.Y(), CA.Y(), CB.Y(),
			CP.Z(), CA.Z(), CB.Z()).det();

	double delta_u = Matrix3x3<double>(
			D.X(), CP.X(), CB.X(),
			D.Y(), CP.Y(), CB.Y(),
			D.Z(), CP.Z(), CB.Z()).det();

	double delta_v = Matrix3x3<double>(
			D.X(), CA.X(), CP.X(),
			D.Y(), CA.Y(), CP.Y(),
			D.Z(), CA.Z(), CP.Z()).det();

	if (delta == 0) {
		return false;
	} else {
		t = delta_t / delta;
		u = delta_u / delta;
		v = delta_v / delta;
		return true;
	}
}
