#ifndef LOADER_H

#define LOADER_H

#include <string>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "base_classes.h"

class Loader {
public:
	enum Item_type {VT, VN, V, F, ERROR};

	struct Material {
		Vector3 Kd;
		Vector3 Ks;
		double Ns;
		double d;

		Material()
		{
			Kd = Vector3();
			Ks = Vector3();
			Ns = d = 0;
		}
	};

	struct Item {
		Item_type type;
		Vertex3 v;
		Vector3 n;
		int v1, v2, v3;
		int n1, n2, n3;
		int t1, t2, t3;
		Material material;
		bool valid;

		Item()
		{
			type = ERROR;
			v = Vertex3();
			n = Vector3();
			v1 = v2 = v3 = n1 = n2 = n3 = t1 = t2 = t3 = 0;
			valid = false;
		}
	};

	Loader(std::string file);
	~Loader();
	Item get();

private:
	std::string obj_filename;
	struct idx_entry {
		int a;
		int b;
		int c;
		int number;
	};
	idx_entry current_material;
	std::vector<idx_entry> index;
	std::vector<Material> mat;
	int fd;
	bool ok;
	bool load_material_library(std::string lib_name);
};

#endif
