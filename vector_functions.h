#ifndef VECTOR_FUNCTIONS_H

#define VECTOR_FUNCTIONS_H

#include <math.h>
#include "base_classes.h"
#include "base_functions.h"
#include "material.h"

inline Vector3 mirror(Vector3 obj, Vector3 base)
{
	Vector3 base_n = base.normalize();
	Vector3 obj_n = obj.normalize();
	double ang_cos = base_n.dot(obj_n);
	Vector3 delta = (base_n * ang_cos) - obj_n;
	return ((base_n * ang_cos) + delta) * obj.length();
}

inline Vector3 get_Phong_light(Material mat, Vector3 normal,
	Vector3 dir, Vector3 light, double intensity)
{
	Vector3 light_n = light.normalize();
	return Vector3(
			(mat.Kd * light_n.dot(normal) * intensity) +
			(mat.Ks * pow((light_n + (dir * -1)).normalize().dot(normal),
			mat.alpha) * intensity));
}

bool triangle_ray_intersection(Vertex3 *vertices, Vertex3 origin, Vector3 dir,
		double &t, double &u, double &v);

inline Vector3 Snell_refr(Vector3 dir, Vector3 normal, double n1, double n2,
		bool &refr_out)
{
	double ang_cos = (dir * -1).normalize().dot(normal);
	Vector3 delta = ((normal * ang_cos) - (dir * -1)) * (n1 / n2);
	ang_cos = sqrt(1 - pow(ang_cos, 2)) * n1 / n2;
	ang_cos = sqrt(1 - pow(ang_cos, 2));
	Vector3 res = ((normal * -ang_cos) + delta).normalize();
	if (res.dot(normal) <= 0) {
		refr_out = true;
		return res;
	} else {
		return mirror(dir * -1, normal).normalize();
	}
}

inline double Fresnel_coeff(double n1, double n2, double ang_cos)
{
	double ang_sin = sqrt(1 - pow(ang_cos, 2));
	double r1 = pow(module((n1*ang_cos - n2*sqrt(1 - pow(n1/n2*ang_sin, 2))) /
			(n1*ang_cos + n2*sqrt(1 - pow(n1/n2*ang_sin, 2)))), 2);
	double r2 = pow(module((n1*sqrt(1 - pow(n1/n2*ang_sin, 2)) - n2*ang_cos) /
			(n1*sqrt(1 - pow(n1/n2*ang_sin, 2)) + n2*ang_cos)), 2);
	return (r1 + r2) / 2;
}

#endif
