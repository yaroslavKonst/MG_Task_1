#ifndef CAMERA_H

#define CAMERA_H

#include <math.h>
#include "base_classes.h"
#include "base_functions.h"

class Camera {
public:
	Vertex3 position;
	double phi;
	double psi;
	double fov;

	Vector3 getDir(double x, double y, double w, double h);
};

#endif
