#include "primitives.h"

Object::intersect Triangle::intersect_ray(Scene &scene, Vertex3 origin,
		Vector3 dir, bool shadow, int depth, bool path)
{
	intersect res;

	double t, u, v;

	if (!triangle_ray_intersection(vertices, origin, dir, t, u, v)) {
		res.valid = false;
	} else {
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
				res.color = calculate_light(res.pos, res.normal, dir, scene,
						mat, depth, path);
			}
		} else {
			res.valid = false;
		}
	}
	return res;
}

Object::intersect Sphere::intersect_ray(Scene &scene, Vertex3 origin,
		Vector3 dir, bool shadow, int depth, bool path)
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
			res.color = calculate_light(res.pos, res.normal, dir, scene, mat,
					depth, path);
		}
	} else {
		res.valid = false;
	}
	return res;
}

Object::intersect Endless_plane::intersect_ray(Scene &scene, Vertex3 origin,
		Vector3 dir, bool shadow, int depth, bool path)
{
	intersect res;

	double t, u, v;

	if (!triangle_ray_intersection(vertices, origin, dir, t, u, v)) {
		res.valid = false;
	} else {
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
				if (!texture) {
					res.color = calculate_light(res.pos, res.normal, dir,
							scene, mat, depth, path);
				} else {
					res.color = calculate_light(res.pos, res.normal, dir,
							scene,
							Material(texture->get_color(textures[0] * u +
							textures[1] * v + textures[2] * (1-u-v)), mat.Ks,
							mat.Ns, mat.d, mat.alpha), depth, path);
				}
			}
		} else {
			res.valid = false;
		}
	}
	return res;
}
