#ifndef MATERIAL_H

#define MATERIAL_H

#include "base_classes.h"

struct Material {
	Vector3 Kd;
	Vector3 Ks;
	double Ns;
	double d;
	double N;
	double alpha;

	Material(Vector3 kd = Vector3(), Vector3 ks = Vector3(), double ns = 0,
			double D = 0, double a = 20, double n = 1)
	{
		Kd = kd;
		Ks = ks;
		Ns = ns;
		d = D;
		alpha = a;
		N = n;
	}
};

#endif
