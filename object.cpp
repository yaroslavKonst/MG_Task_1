#include "object.h"

Vector3 Object::get_color(Vertex3 pos, Vector3 normal, Vector3 dir,
	std::vector<Object*> &objects, std::vector<Light> &lights, material mat)
{
	Vector3 light_sum;
	Vertex3 start = pos + normal * 0.001;
	for (unsigned int l = 0; l < lights.size(); ++l) {
		Vector3 lt = lights[l].pos - start;
		if (normal.dot(lt) > 0) {
			bool f = true;
			for (unsigned int obj = 0; obj < objects.size(); ++obj) {
				intersect info = objects[obj]->intersect_ray(objects, lights,
					start, lt.normalize(), true);
				if (info.valid && info.t < lt.length()) {
					f = false;
					break;
				}
			}
			if (f) {
				light_sum += get_Phong_light(mat.Kd, mat.Ks, mat.Ns, mat.alpha,
					normal, dir, lt, lights[l].intensity / pow(lt.length(), 2));
			}
		}
	}
	Vector3 ref_dir = mirror(dir * (-1), normal).normalize();
	intersect info;
	info.valid = false;
	info.t = 0;
	if (mat.Ns > 0) {
		for (unsigned int i = 0; i < objects.size(); ++i) {
			intersect info1 = objects[i]->intersect_ray(objects, lights, start,
				ref_dir, false);
			if (!info.valid || (info1.valid && info1.t < info.t)) {
				info = info1;
			}
		}
	}
	if (info.valid) {
		light_sum = (light_sum * (1 - (mat.Ns / 1000))) +
			(info.color * (mat.Ns / 1000));
	} else {
		light_sum = light_sum * (1 - (mat.Ns / 1000));
	}
	light_sum = Vector3(
		min(light_sum.X(), 1.0),
		min(light_sum.Y(), 1.0),
		min(light_sum.Z(), 1.0));
	return light_sum;
}
