#ifndef REFR_INFO_H

#define REFR_INFO_H

#include "base_classes.h"

class RefrInfo {
public:
	struct info {
		Vertex3 origin;
		double n;

		info(Vertex3 o, double N)
		{
			origin = o;
			n = N;
		}
	};

	Stack<info> st;

	RefrInfo() {
		st.push(info(Vertex3(), 1));
	}

	void reset()
	{
		st.clear();
		st.push(info(Vertex3(), 1));
	}
};

#endif
