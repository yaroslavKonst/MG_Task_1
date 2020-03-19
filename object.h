#ifndef OBJECT_H

#define OBJECT_H

#include <vector>
#include "base_classes.h"
#include "light.h"
#include "texture.h"

class Object {
public:
	struct intersect {
		bool valid;
		double t;
		Vertex3 pos;
		Vector3 normal;
		Vector3 color;
	};

	struct material {
		Vector3 Kd;
		Vector3 Ks;
		double Ns;
		double d;
		double alpha;
		material(Vector3 kd = Vector3(), Vector3 ks = Vector3(),
			double ns = 0, double D = 0, double a = 20)
		{
			Kd = kd;
			Ks = ks;
			Ns = ns;
			d = D;
			alpha = a;
		}
	};

	Texture *texture;

	Object()
	{
		texture = 0;
	}

	virtual intersect intersect_ray(std::vector<Object*> &objects,
		std::vector<Light> &lights, Vertex3 origin, Vector3 dir,
		bool shadow) = 0;

	Vector3 get_color(Vertex3 pos, Vector3 normal, Vector3 dir,
		std::vector<Object*> &objects, std::vector<Light> &lights,
		material mat);
};

#endif
