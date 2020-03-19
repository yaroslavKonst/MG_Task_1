#ifndef PRIMITIVES_H

#define PRIMITIVES_H

#include <vector>
#include <iostream>
#include <math.h>
#include "object.h"
#include "base_classes.h"
#include "texture.h"

class Triangle: public Object {
public:
	virtual intersect intersect_ray(std::vector<Object*> &objects,
		std::vector<Light> &lights, Vertex3 origin, Vector3 dir, bool shadow);

	Vertex3 vertices[3];
	Vector3 normals[3];
	Vector3 textures[3];

	material mat;
};

class Sphere: public Object {
public:
	virtual intersect intersect_ray(std::vector<Object*> &objects,
		std::vector<Light> &lights, Vertex3 origin, Vector3 dir, bool shadow);

	Vertex3 pos;
	double R;

	material mat;

	Sphere(Vertex3 p, double r, Vector3 kd, Vector3 ks, double ns, double D)
	{
		pos = p;
		R = r;
		mat.Kd = kd;
		mat.Ks = ks;
		mat.Ns = ns;
		mat.d = D;
	}
};

class Endless_plane: public Triangle {
public:
	virtual intersect intersect_ray(std::vector<Object*> &objects,
		std::vector<Light> &lights, Vertex3 origin, Vector3 dir, bool shadow);
};

#endif
