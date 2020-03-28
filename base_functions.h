#ifndef BASE_FUNCTIONS_H

#define BASE_FUNCTIONS_H

#include <math.h>

template<class T>
inline T max(T a, T b)
{
	return a > b ? a : b;
}

template<class T>
inline T min(T a, T b)
{
	return a < b ? a : b;
}

template<class T>
inline T module(T a)
{
	return a >= 0 ? a : -a;
}

template<class T>
bool solve_quad(T a, T b, T c, T &r1, T &r2)
{
	T D = b * b - a * c * 4;
	if (D > 0) {
		r1 = (-b - sqrt(D)) / (2 * a);
		r2 = (-b + sqrt(D)) / (2 * a);
		return true;
	} else if (D == 0) {
		r1 = r2 = -b / (2 * a);
		return true;
	} else {
		return false;
	}
}

#endif
