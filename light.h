#ifndef LIGHT_H

#define LIGHT_H

#include "base_classes.h"

class Light {
public:
	Light(Vertex3 p, double i): pos(p), intensity(i)
	{ }
	Vertex3 pos;
	double intensity;
};

#endif
