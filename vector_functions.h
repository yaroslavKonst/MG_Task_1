#ifndef VECTOR_FUNCTIONS_H

#define VECTOR_FUNCTIONS_H

#include <math.h>
#include "base_classes.h"

inline Vector3 mirror(Vector3 obj, Vector3 base)
{
	Vector3 base_n = base.normalize();
	Vector3 obj_n = obj.normalize();
	double ang_cos = base_n.dot(obj_n);
	Vector3 delta = (base_n * ang_cos) - obj_n;
	return ((base_n * ang_cos) + delta) * obj.length();
}

inline Vector3 get_Phong_light(Vector3 Kd, Vector3 Ks, double Ns, double alpha,
	Vector3 normal, Vector3 dir, Vector3 light, double intensity)
{
	Vector3 light_n = light.normalize();
	Vector3 sum =
		(Kd * light_n.dot(normal) * intensity) +
		(Ks * pow(mirror(light_n, normal).dot((dir*(-1)).normalize()), alpha) *
		intensity * 10);
	return sum;
}

#endif
