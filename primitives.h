#ifndef PRIMITIVES_H

#define PRIMITIVES_H

#include <vector>
#include <iostream>
#include <math.h>
#include "object.h"
#include "base_classes.h"
#include "vector_functions.h"
#include "material.h"
#include "texture.h"

class Triangle: public Object {
public:
	virtual intersect intersect_ray(Scene &scene, Vertex3 origin, Vector3 dir,
			bool shadow);

	Triangle(
			Vertex3 v1 = Vertex3(),
			Vertex3 v2 = Vertex3(),
			Vertex3 v3 = Vertex3(),
			Vector3 n1 = Vector3(),
			Vector3 n2 = Vector3(),
			Vector3 n3 = Vector3(),
			Material m = Material(),
			Texture *tx = 0,
			Vector3 t1 = Vector3(),
			Vector3 t2 = Vector3(),
			Vector3 t3 = Vector3())
	{
		vertices[0] = v1;
		vertices[1] = v2;
		vertices[2] = v3;
		normals[0] = n1;
		normals[1] = n2;
		normals[2] = n3;
		textures[0] = t1;
		textures[1] = t2;
		textures[2] = t3;
		mat = m;
		texture = tx;
	}

	Vertex3 vertices[3];
	Vector3 normals[3];
	Vector3 textures[3];
};

class Sphere: public Object {
public:
	virtual intersect intersect_ray(Scene &scene, Vertex3 origin, Vector3 dir,
			bool shadow);

	Vertex3 pos;
	double R;

	Sphere(Vertex3 p, double r, const Material &m)
	{
		pos = p;
		R = r;
		mat = m;
	}
};

class Endless_plane: public Triangle {
public:
	virtual intersect intersect_ray(Scene &scene, Vertex3 origin, Vector3 dir,
			bool shadow);

	Endless_plane(
			Vertex3 v1 = Vertex3(),
			Vertex3 v2 = Vertex3(),
			Vertex3 v3 = Vertex3(),
			Vector3 n1 = Vector3(),
			Vector3 n2 = Vector3(),
			Vector3 n3 = Vector3(),
			Material m = Material(),
			Texture *tx = 0,
			Vector3 t1 = Vector3(),
			Vector3 t2 = Vector3(),
			Vector3 t3 = Vector3())
		:
			Triangle(v1, v2, v3, n1, n2, n3, m, tx, t1, t2, t3)
	{ }
};

#endif
