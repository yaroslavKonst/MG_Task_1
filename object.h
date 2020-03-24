#ifndef OBJECT_H

#define OBJECT_H

#include <vector>
#include "base_classes.h"
#include "base_functions.h"
#include "material.h"
#include "vector_functions.h"
#include "light.h"
#include "texture.h"

class Scene;

class Object {
public:
	struct intersect {
		bool valid;
		double t;
		Vertex3 pos;
		Vector3 normal;
		Vector3 color;
	};

	Texture *texture;

	Material mat;

	Object()
	{
		texture = 0;
	}

	virtual ~Object()
	{ }

	virtual intersect intersect_ray(Scene &scene, Vertex3 origin,
			Vector3 dir, bool shadow) = 0;

	Vector3 calculate_light(Vertex3 pos, Vector3 normal, Vector3 dir,
			Scene &scene, Material mat);
};

class Scene {
public:
	std::vector<Object*> objects;
	std::vector<Light> lights;

	Object::intersect intersect_ray(Vertex3 pos, Vector3 dir, bool shadow);

	void add(Object *obj)
	{
		objects.push_back(obj);
	}

	void add(Light lt)
	{
		lights.push_back(lt);
	}

	void del()
	{
		for (unsigned int i = 0; i < objects.size(); ++i) {
			delete objects[i];
		}
	}
};

#endif
