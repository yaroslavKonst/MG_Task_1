#include "mesh.h"

Mesh::Mesh(std::string file, double x, double y, double z, double phi,
		double psi, double theta)
{
	X = x;
	Y = y;
	Z = z;
	Phi = phi;
	Psi = psi;
	Theta = theta;

	if (file == "") {
		return;
	}

	Loader loader(file);
	Loader::Item item;
	while ((item = loader.get()).valid) {
		switch (item.type) {
		case Loader::F:
			{
				polygon pol;
				pol.vertices[0] = vertices[item.v1 - 1];
				pol.vertices[1] = vertices[item.v2 - 1];
				pol.vertices[2] = vertices[item.v3 - 1];
				pol.normals[0] = normals[item.n1 - 1];
				pol.normals[1] = normals[item.n2 - 1];
				pol.normals[2] = normals[item.n3 - 1];
				pol.textures[0] = textures[item.t1 - 1];
				pol.textures[1] = textures[item.t2 - 1];
				pol.textures[2] = textures[item.t3 - 1];
				pol.Kd = item.material.Kd;
				pol.Ks = item.material.Ks;
				pol.Ns = item.material.Ns;
				pol.d = item.material.d;
				polygons.push_back(pol);
			}
			break;
		case Loader::VT:
			textures.push_back(Vector3(item.n));
			break;
		case Loader::VN:
			normals.push_back(Vector3(item.n));
			break;
		case Loader::V:
			vertices.push_back(Vertex3(item.v));
			break;
		case Loader::ERROR:
			throw "Object loading error\n";
			break;
		}
	}

	max_x = min_x = vertices[0].X();
	max_y = min_y = vertices[0].Y();
	max_z = min_z = vertices[0].Z();

	int sz = vertices.size();

	for (int i = 1; i < sz; ++i) {
		Vertex3 v = vertices[i];
		if (max_x < v.X()) {
			max_x = v.X();
		}
		if (min_x > v.X()) {
			min_x = v.X();
		}

		if (max_y < v.Y()) {
			max_y = v.Y();
		}
		if (min_y > v.Y()) {
			min_y = v.Y();
		}

		if (max_z < v.Z()) {
			max_z = v.Z();
		}
		if (min_z > v.Z()) {
			min_z = v.Z();
		}
	}

	vertices.clear();
	normals.clear();
	textures.clear();

	sz = polygons.size();

	tree_root = new tree_elem;
	for (int i = 0; i < sz; ++i) {
		tree_root->polygons.push_back(i);
	}
	tree_root->max_x = max_x;
	tree_root->min_x = min_x;
	tree_root->max_y = max_y;
	tree_root->min_y = min_y;
	tree_root->max_z = max_z;
	tree_root->min_z = min_z;
	tree_root->build(polygons);
}

Mesh::Mesh(const Mesh &mesh)
{
	X = mesh.X;
	Y = mesh.Y;
	Z = mesh.Z;

	Phi = mesh.Phi;
	Psi = mesh.Psi;
	Theta = mesh.Theta;

	max_x = mesh.max_x;
	min_x = mesh.min_x;
	max_y = mesh.max_y;
	min_y = mesh.min_y;
	max_z = mesh.max_z;
	min_z = mesh.min_z;

	vertices = mesh.vertices;
	normals = mesh.normals;
	textures = mesh.textures;

	polygons = mesh.polygons;

	tree_root = new tree_elem(*mesh.tree_root);
}

void Mesh::tree_elem::build(const std::vector<polygon> &pol)
{
	if (polygons.size() > 1000) {
		lv[0] = new tree_elem;
		lv[0]->max_x = max_x;
		lv[0]->min_x = (max_x + min_x) / 2;
		lv[0]->max_y = max_y;
		lv[0]->min_y = (max_y + min_y) / 2;
		lv[0]->max_z = max_z;
		lv[0]->min_z = (max_z + min_z) / 2;

		lv[1] = new tree_elem;
		lv[1]->max_x = max_x;
		lv[1]->min_x = (max_x + min_x) / 2;
		lv[1]->max_y = max_y;
		lv[1]->min_y = (max_y + min_y) / 2;
		lv[1]->max_z = (max_z + min_z) / 2;
		lv[1]->min_z = min_z;

		lv[2] = new tree_elem;
		lv[2]->max_x = max_x;
		lv[2]->min_x = (max_x + min_x) / 2;
		lv[2]->max_y = (max_y + min_y) / 2;
		lv[2]->min_y = min_y;
		lv[2]->max_z = max_z;
		lv[2]->min_z = (max_z + min_z) / 2;

		lv[3] = new tree_elem;
		lv[3]->max_x = max_x;
		lv[3]->min_x = (max_x + min_x) / 2;
		lv[3]->max_y = (max_y + min_y) / 2;
		lv[3]->min_y = min_y;
		lv[3]->max_z = (max_z + min_z) / 2;
		lv[3]->min_z = min_z;

		lv[4] = new tree_elem;
		lv[4]->max_x = (max_x + min_x) / 2;
		lv[4]->min_x = min_x;
		lv[4]->max_y = max_y;
		lv[4]->min_y = (max_y + min_y) / 2;
		lv[4]->max_z = max_z;
		lv[4]->min_z = (max_z + min_z) / 2;

		lv[5] = new tree_elem;
		lv[5]->max_x = (max_x + min_x) / 2;
		lv[5]->min_x = min_x;
		lv[5]->max_y = max_y;
		lv[5]->min_y = (max_y + min_y) / 2;
		lv[5]->max_z = (max_z + min_z) / 2;
		lv[5]->min_z = min_z;

		lv[6] = new tree_elem;
		lv[6]->max_x = (max_x + min_x) / 2;
		lv[6]->min_x = min_x;
		lv[6]->max_y = (max_y + min_y) / 2;
		lv[6]->min_y = min_y;
		lv[6]->max_z = max_z;
		lv[6]->min_z = (max_z + min_z) / 2;

		lv[7] = new tree_elem;
		lv[7]->max_x = (max_x + min_x) / 2;
		lv[7]->min_x = min_x;
		lv[7]->max_y = (max_y + min_y) / 2;
		lv[7]->min_y = min_y;
		lv[7]->max_z = (max_z + min_z) / 2;
		lv[7]->min_z = min_z;

		int sz = polygons.size();

		for (int i = 0; i < sz; ++i) {
			for (int j = 0; j < 8; ++j) {
				if (pol[polygons[i]].in(lv[j]->max_x, lv[j]->min_x,
						lv[j]->max_y, lv[j]->min_y, lv[j]->max_z, lv[j]->min_z)
				) {
					lv[j]->polygons.push_back(polygons[i]);
				}
			}
		}
		for (int i = 0; i < 8; ++i) {
			if (lv[i]->polygons.size() > 0) {
				lv[i]->build(pol);
			} else {
				delete lv[i];
				lv[i] = 0;
			}
		}
	} else {
		for (int i = 0; i < 8; ++i) {
			lv[i] = 0;
		}
	}
}

Mesh::~Mesh()
{
	if (tree_root) {
		delete tree_root;
	}
}

list<Mesh::tree_elem*> Mesh::tree_elem::check_ray(Vertex3 origin, Vector3 dir)
{
	double lx = (1 / dir.X()) * (min_x - origin.X());
	double hx = (1 / dir.X()) * (max_x - origin.X());
	double ly = (1 / dir.Y()) * (min_y - origin.Y());
	double hy = (1 / dir.Y()) * (max_y - origin.Y());
	double lz = (1 / dir.Z()) * (min_z - origin.Z());
	double hz = (1 / dir.Z()) * (max_z - origin.Z());

	double tmin = max(max(min(lx, hx), min(ly, hy)), min(lz, hz));
	double tmax = min(min(max(lx, hx), max(ly, hy)), max(lz, hz));

	list<tree_elem*> lst;

	if (tmin > 0 && tmin <= tmax) {
		bool f = true;
		for (int i = 0; i < 8; ++i) {
			if (lv[i]) {
				lst += lv[i]->check_ray(origin, dir);
			} else if (f) {
				f = false;
				lst.push_back(this);
			}
		}

	}
	return lst;
}

Vertex3 Mesh::to_local(Vertex3 vertex)
{
	Vertex3 local(vertex);
	local += Vector3(-X, -Y, -Z);
	// rotation around Z
	local = Vertex3(
			local.X() * cos(-Theta) - local.Y() * sin(-Theta),
			local.X() * sin(-Theta) + local.Y() * cos(-Theta),
			local.Z());
	// rotation around X
	local = Vertex3(
			local.X(),
			local.Y() * cos(-Psi) - local.Z() * sin(-Psi),
			local.Y() * sin(-Psi) + local.Z() * cos(-Psi));
	// rotation around Y
	local = Vertex3(
			local.X() * cos(-Phi) + local.Z() * sin(-Phi),
			local.Y(),
			-local.X() * sin(-Phi) + local.Z() * cos(-Phi));
	return local;
}

Vertex3 Mesh::to_global(Vertex3 vertex)
{
	Vertex3 global(vertex);
	// rotation around Y
	global = Vertex3(
			global.X() * cos(Phi) + global.Z() * sin(Phi),
			global.Y(),
			-global.X() * sin(Phi) + global.Z() * cos(Phi));
	// rotation around X
	global = Vertex3(
			global.X(),
			global.Y() * cos(Psi) - global.Z() * sin(Psi),
			global.Y() * sin(Psi) + global.Z() * cos(Psi));
	// rotation around Z
	global = Vertex3(
			global.X() * cos(Theta) - global.Y() * sin(Theta),
			global.X() * sin(Theta) + global.Y() * cos(Theta),
			global.Z());
	global += Vector3(X, Y, Z);
	return global;
}

Vector3 Mesh::to_local(Vector3 vector)
{
	Vector3 local(vector);
	// rotation around Z
	local = Vector3(
			local.X() * cos(-Theta) - local.Y() * sin(-Theta),
			local.X() * sin(-Theta) + local.Y() * cos(-Theta),
			local.Z());
	// rotation around X
	local = Vector3(
			local.X(),
			local.Y() * cos(-Psi) - local.Z() * sin(-Psi),
			local.Y() * sin(-Psi) + local.Z() * cos(-Psi));
	// rotation around Y
	local = Vector3(
			local.X() * cos(-Phi) + local.Z() * sin(-Phi),
			local.Y(),
			-local.X() * sin(-Phi) + local.Z() * cos(-Phi));
	return local;
}

Vector3 Mesh::to_global(Vector3 vector)
{
	Vector3 global(vector);
	// rotation around Y
	global = Vector3(
			global.X() * cos(Phi) + global.Z() * sin(Phi),
			global.Y(),
			-global.X() * sin(Phi) + global.Z() * cos(Phi));
	// rotation around X
	global = Vector3(
			global.X(),
			global.Y() * cos(Psi) - global.Z() * sin(Psi),
			global.Y() * sin(Psi) + global.Z() * cos(Psi));
	// rotation around Z
	global = Vector3(
			global.X() * cos(Theta) - global.Y() * sin(Theta),
			global.X() * sin(Theta) + global.Y() * cos(Theta),
			global.Z());
	return global;
}

Object::intersect Mesh::intersect_ray(Scene &scene, Vertex3 origin,
	Vector3 dir, bool shadow, int depth)
{
	Vertex3 local_origin = to_local(origin);
	Vector3 local_dir = to_local(dir);

	local_dir = local_dir.normalize();

	intersect stat;

	list<tree_elem*> lst;
	double Gu = 0;
	double Gv = 0;
	double Gt = -1;
	polygon pol_res;
	lst += tree_root->check_ray(local_origin, local_dir);
	while (!lst.empty()) {
		tree_elem *cur = lst.pop_front();
		for (unsigned int i = 0; i < cur->polygons.size(); ++i) {
			polygon cur_pol = polygons[cur->polygons[i]];

			double t, u, v;

			if (!triangle_ray_intersection(cur_pol.vertices, local_origin,
					local_dir, t, u, v)
			) {
				continue;
			}

			if (u >= 0 && v >= 0 && 1 - u - v >= 0 && t > 0 &&
					(t < Gt || Gt < 0)
			) {
				Gt = t;
				Gu = u;
				Gv = v;
				pol_res = cur_pol;
			}
		}
	}
	if (Gt < 0) {
		stat.valid = false;
	} else {
		stat.valid = true;

		Vector3 norm = (
				pol_res.normals[0] * Gu +
				pol_res.normals[1] * Gv +
				pol_res.normals[2] * (1 - Gu - Gv)).normalize();

		Vertex3 pos(
				pol_res.vertices[0].X() * Gu +
				pol_res.vertices[1].X() * Gv +
				pol_res.vertices[2].X() * (1 - Gu - Gv),
				pol_res.vertices[0].Y() * Gu +
				pol_res.vertices[1].Y() * Gv +
				pol_res.vertices[2].Y() * (1 - Gu - Gv),
				pol_res.vertices[0].Z() * Gu +
				pol_res.vertices[1].Z() * Gv +
				pol_res.vertices[2].Z() * (1 - Gu - Gv));

		Vertex3 gl_pos = to_global(pos);
		Vector3 gl_norm = to_global(norm);

		if (!shadow) {
			stat.color = calculate_light(gl_pos, gl_norm, dir, scene,
					Material(pol_res.Kd, pol_res.Ks, pol_res.Ns, pol_res.d),
					depth);
		}
		stat.t = Gt;
	}
	return stat;
}
