#ifndef MESH_H

#define MESH_H

#include <string>
#include <vector>
#include <iostream>
#include "object.h"
#include "base_classes.h"
#include "base_functions.h"
#include "loader.h"

class Mesh: public Object {
public:
	Mesh(std::string file = std::string(""), double x = 0, double y = 0,
			double z = 0, double phi = 0, double psi = 0, double theta = 0);
	Mesh(const Mesh& mesh);
	~Mesh();

	virtual intersect intersect_ray(Scene &scene, Vertex3 origin, Vector3 dir,
			bool shadow, int depth, bool path);

private:
	double X;
	double Y;
	double Z;

	double Phi;   // Y axis
	double Psi;   // X axis
	double Theta; // Z axis

	double max_x;
	double min_x;
	double max_y;
	double min_y;
	double max_z;
	double min_z;

	std::vector<Vertex3> vertices;
	std::vector<Vector3> normals;
	std::vector<Vector3> textures;

	Vertex3 to_local(Vertex3 vertex);
	Vertex3 to_global(Vertex3 vertex);

	Vector3 to_local(Vector3 vector);
	Vector3 to_global(Vector3 vector);

	struct polygon {
		Vertex3 vertices[3];
		Vector3 normals[3];
		Vector3 textures[3];
		Vector3 Kd; // color
		Vector3 Ks; // reflection color
		double Ns;  // reflection coeff 0 - 1000
		double d;   // transparency coeff

		polygon()
		{
			for (int i = 0; i < 3; ++i) {
				vertices[i] = Vertex3();
				normals[i] = Vector3();
				textures[i] = Vector3();
			}

			Kd = Vector3();
			Ks = Vector3();
			Ns = 0;
			d = 0;
		}

		bool in(double max_x, double min_x, double max_y, double min_y,
				double max_z, double min_z) const
		{
			for (int i = 0; i < 3; ++i) {
				double x = vertices[i].X();
				double y = vertices[i].Y();
				double z = vertices[i].Z();
				if (min_x <= x && x <= max_x &&
						min_y <= y && y <= max_y &&
						min_z <= z && z <= max_z
				) {
					return true;
				}
			}
			return false;
		}
	};

	std::vector<polygon> polygons;

	struct tree_elem {
		std::vector<int> polygons;
		tree_elem *lv[8];
		double max_x;
		double min_x;
		double max_y;
		double min_y;
		double max_z;
		double min_z;

		tree_elem()
		{
			for (int i = 0; i < 8; ++i) {
				lv[i] = 0;
			}
		}

		tree_elem(const tree_elem &te)
		{
			polygons = te.polygons;
			for (int i = 0; i < 8; ++i) {
				if (te.lv[i]) {
					lv[i] = new tree_elem(*te.lv[i]);
				} else {
					lv[i] = 0;
				}
			}
			max_x = te.max_x;
			min_x = te.min_x;
			max_y = te.max_y;
			min_y = te.min_y;
			max_z = te.max_z;
			min_z = te.min_z;
		}

		void build(const std::vector<polygon> &pol);

		~tree_elem()
		{
			for (int i = 0; i < 8; ++i) {
				if (lv[i]) {
					delete lv[i];
				}
			}
		}

		list<tree_elem*> check_ray(Vertex3 origin, Vector3 dir);
	};

	tree_elem *tree_root;

	void operator=(const Mesh &m)
	{ }
};

#endif
