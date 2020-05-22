#ifndef TEXTURE_H

#define TEXTURE_H

#include "base_classes.h"

class Texture {
public:
	virtual Vector3 get_color(Vector3 pos) = 0;
};

class TexChBoard: public Texture {
public:
	virtual Vector3 get_color(Vector3 pos)
	{
		if (pos.X() < 0) {
			pos = Vector3(-pos.X() + 20, pos.Y(), pos.Z());
		}
		if (pos.Y() < 0) {
			pos = Vector3(pos.X(), -pos.Y() + 20, pos.Z());
		}
		if (((int)pos.X()) % 40 < 20) {
			if (((int)pos.Y()) % 40 < 20) {
				return Vector3(1, 1, 1);
			} else {
				return Vector3(0, 0, 0);
			}
		} else {
			if (((int)pos.Y()) % 40 < 20) {
				return Vector3(0, 0, 0);
			} else {
				return Vector3(1, 1, 1);
			}
		}
	}
};

#endif
