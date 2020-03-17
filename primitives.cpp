#include "primitives.h"

Object::intersect Triangle::intersect_ray(std::vector<Object*> &objects,
	std::vector<Light> &lights, Vertex3 origin, Vector3 dir, bool shadow)
{
	intersect res;

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

	double t, u, v;

	if (delta == 0) {
		res.valid = false;
	} else {
		t = delta_t / delta;
		u = delta_u / delta;
		v = delta_v / delta;
		if (u >= 0 && v >= 0 && 1 - u - v >= 0 && t > 0) {
			res.valid = true;
			res.t = t;
			res.pos = Vertex3(
				vertices[0].X() * u +
				vertices[1].X() * v +
				vertices[2].X() * (1 - u - v),
				vertices[0].Y() * u +
				vertices[1].Y() * v +
				vertices[2].Y() * (1 - u - v),
				vertices[0].Z() * u +
				vertices[1].Z() * v +
				vertices[2].Z() * (1 - u - v));
			res.normal = Vector3(
				normals[0] * u +
				normals[1] * v +
				normals[2] * (1 - u - v)).normalize();
			Vector3 light_sum;
			if (!shadow) {
				res.color = get_color(res.pos, res.normal, dir, objects, lights,
					material(Kd, Ks, Ns, d));
			}
		} else {
			res.valid = false;
		}
	}
	return res;
}

Object::intersect Sphere::intersect_ray(std::vector<Object*> &objects,
	std::vector<Light> &lights, Vertex3 origin, Vector3 dir, bool shadow)
{
	dir = dir.normalize();
	double a = pow(dir.X(), 2) + pow(dir.Y(), 2) + pow(dir.Z(), 2);
	double b =
		2*origin.X()*dir.X() - 2*pos.X()*dir.X() +
		2*origin.Y()*dir.Y() - 2*pos.Y()*dir.Y() +
		2*origin.Z()*dir.Z() - 2*pos.Z()*dir.Z();
	double c =
		pow(pos.X(), 2) + pow(pos.Y(), 2) + pow(pos.Z(), 2) - pow(R, 2) +
		pow(origin.X(), 2) + pow(origin.Y(), 2) + pow(origin.Z(), 2) -
		2*pos.X()*origin.X() - 2*pos.Y()*origin.Y() - 2*pos.Z()*origin.Z();

	double t1, t2;

	intersect res;

	if (solve_quad(a, b, c, t1, t2) && (t1 > 0 || t2 > 0)) {
		double t = (t1 < t2 && t1 > 0) ? t1 : t2;
		res.valid = true;
		res.t = t;
		res.pos = origin + (dir * t);
		res.normal = (res.pos - pos).normalize();
		Vector3 light_sum;
		if (!shadow) {
			res.color = get_color(res.pos, res.normal, dir, objects, lights,
				material(Kd, Ks, Ns, d));
		}
	} else {
		res.valid = false;
	}
	return res;
}

Object::intersect Endless_plane::intersect_ray(std::vector<Object*> &objects,
	std::vector<Light> &lights, Vertex3 origin, Vector3 dir, bool shadow)
{
	intersect res;

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

	double t, u, v;

	if (delta == 0) {
		res.valid = false;
	} else {
		t = delta_t / delta;
		u = delta_u / delta;
		v = delta_v / delta;
		if (t > 0.0) {
			res.valid = true;
			res.t = t;
			res.pos = Vertex3(
				vertices[0].X() * u +
				vertices[1].X() * v +
				vertices[2].X() * (1 - u - v),
				vertices[0].Y() * u +
				vertices[1].Y() * v +
				vertices[2].Y() * (1 - u - v),
				vertices[0].Z() * u +
				vertices[1].Z() * v +
				vertices[2].Z() * (1 - u - v));
			res.normal = Vector3(
				normals[0] * u +
				normals[1] * v +
				normals[2] * (1 - u - v)).normalize();
			Vector3 light_sum;
			if (!shadow) {
				res.color = get_color(res.pos, res.normal, dir, objects, lights,
					material(Kd, Ks, Ns, d));
			}
		} else {
			res.valid = false;
		}
	}
	return res;
}
