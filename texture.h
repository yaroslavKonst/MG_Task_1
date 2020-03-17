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
		if (((int)pos.X()) % 30 < 15) {
			if (((int)pos.Y()) % 30 < 15) {
				return Vector3(1, 1, 1);
			} else {
				return Vector3(0, 0, 0);
			}
		} else {
			if (((int)pos.Y()) % 30 < 15) {
				return Vector3(0, 0, 0);
			} else {
				return Vector3(1, 1, 1);
			}
		}
	}
};

#endif
