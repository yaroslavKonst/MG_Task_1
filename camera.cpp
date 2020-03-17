#include "camera.h"

Vector3 Camera::getDir(double x, double y, double w, double h)
{
	Vector3 dir(x - w/2, y - h/2, max(w, h)/tan(fov/2));
	dir = dir.normalize();
	// rotation around Y
	dir = Vector3(
		dir.X() * cos(phi) + dir.Z() * sin(phi),
		dir.Y(),
		-dir.X() * sin(phi) + dir.Z() * cos(phi)
	);
	// rotation around X
	dir = Vector3(
		dir.X(),
		dir.Y() * cos(psi) - dir.Z() * sin(psi),
		dir.Y() * sin(psi) + dir.Z() * cos(psi)
	);
	return dir;
}
