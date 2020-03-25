#include "loader.h"

Loader::Loader(std::string file)
{
	fd = open(file.c_str(), O_RDONLY);
	if (fd == -1) {
		ok = false;
	} else {
		ok = true;
	}
	obj_filename = file;
}

Loader::~Loader()
{
	if (ok) {
		close(fd);
	}
}

Loader::Item Loader::get()
{
	int len = 200;
	int sz = 0;
	char c = 0;
	char *s = new char[len];
	if (!ok) {
		Item i;
		i.valid = true;
		i.type = ERROR;
		delete[] s;
		return i;
	}

	while ((read(fd, &c, 1) == 1) && (c != '\n')) {
		if (sz >= len) {
			Item i;
			i.valid = true;
			i.type = ERROR;
			delete[] s;
			return i;
		}
		s[sz] = c;
		++sz;
	}

	if (c != '\n') {
		Item i;
		i.valid = false;
		i.type = ERROR;
		delete[] s;
		return i;
	}
	s[sz] = 0;

	std::string obj_line(s);
	delete[] s;

	Item i;
	i.valid = true;

	if (obj_line.size() == 0 || obj_line[0] == '#') {
		return get();
	} else if (obj_line.find("v ") != std::string::npos) {
		i.type = V;
		double x, y, z;
		sscanf(obj_line.c_str(), "v %lf %lf %lf", &x, &y, &z);
		i.v = Vertex3(x, y, z);
	} else if (obj_line.find("vt ") != std::string::npos) {
		i.type = VT;
		double x, y, z;
		sscanf(obj_line.c_str(), "vt %lf %lf %lf", &x, &y, &z);
		i.n = Vector3(x, y, z);
	} else if (obj_line.find("vn ") != std::string::npos) {
		i.type = VN;
		double x, y, z;
		sscanf(obj_line.c_str(), "vn %lf %lf %lf", &x, &y, &z);
		i.n = Vector3(x, y, z);
	} else if (obj_line.find("f ") != std::string::npos) {
		i.type = F;
		i.material = mat[current_material.number];
		int v1, v2, v3;
		int n1, n2, n3;
		int t1, t2, t3;
		sscanf(obj_line.c_str(), "f %u/%u/%u %u/%u/%u %u/%u/%u",
				&v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3);
		i.v1 = v1;
		i.v2 = v2;
		i.v3 = v3;
		i.n1 = n1;
		i.n2 = n2;
		i.n3 = n3;
		i.t1 = t1;
		i.t2 = t2;
		i.t3 = t3;
	} else if (obj_line.find("mtllib ") != std::string::npos) {
		if (load_material_library(
				obj_filename.substr(0, obj_filename.rfind("/") + 1) +
				obj_line.substr(obj_line.find(" ") + 1))
		) {
			return get();
		} else {
			throw "Material library error";
			i.type = ERROR;
		}
	} else if (obj_line.find("usemtl ") != std::string::npos) {
		int a, b, c;
		sscanf(obj_line.c_str(), "usemtl %u,%u,%u", &a, &b, &c);
		for (unsigned int i = 0; i < index.size(); ++i) {
			if (index[i].a == a && index[i].b == b && index[i].c == c) {
				current_material = index[i];
				break;
			}
		}
		return get();
	} else {
		return get();
	}
	return i;
}

bool Loader::load_material_library(std::string lib_name)
{
	int fd = open(lib_name.c_str(), O_RDONLY);
	if (fd == -1) {
		return false;
	}
	int len = 200;
	char c;
	char *s = new char[len];
	int sz;
	bool rd = true;

	bool nw = false;
	Material material;
	idx_entry idx;
	int rdst;

	while (rd) {
		sz = 0;
		while (((rdst = read(fd, &c, 1)) == 1) && (c != '\n')) {
			if (sz >= len) {
				delete[] s;
				close(fd);
				return false;
			}
			s[sz] = c;
			++sz;
		}

		s[sz] = 0;

		if (rdst != 1) {
			rd = false;
			break;
		}

		std::string mtl_line(s);

		if (mtl_line.find("newmtl ") != std::string::npos) {
			if (nw) {
				idx.number = mat.size();
				mat.push_back(material);
				index.push_back(idx);
				nw = false;
			}
			int a, b, c;
			sscanf(mtl_line.c_str(), "newmtl %u,%u,%u", &a, &b, &c);
			nw = true;
			idx.a = a;
			idx.b = b;
			idx.c = c;
		} else if (mtl_line.find("Kd ") != std::string::npos) {
			double x, y, z;
			sscanf(mtl_line.c_str(), "Kd %lf %lf %lf", &x, &y, &z);
			material.Kd = Vector3(x, y, z);
		} else if (mtl_line.find("Ks ") != std::string::npos) {
			double x, y, z;
			sscanf(mtl_line.c_str(), "Ks %lf %lf %lf", &x, &y, &z);
			material.Ks = Vector3(x, y, z);
		} else if (mtl_line.find("d ") != std::string::npos) {
			double x;
			sscanf(mtl_line.c_str(), "d %lf", &x);
			material.d = x;
		} else if (mtl_line.find("Ns ") != std::string::npos) {
			double x;
			sscanf(mtl_line.c_str(), "Ns %lf", &x);
			material.Ns = x;
		}
	}
	if (nw) {
		idx.number = mat.size();
		mat.push_back(material);
		index.push_back(idx);
		nw = false;
	}
	delete[] s;
	close(fd);
	return true;
}
