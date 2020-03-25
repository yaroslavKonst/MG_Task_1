#include "object.h"

Vector3 Object::calculate_light(Vertex3 pos, Vector3 normal, Vector3 dir,
		Scene &scene, Material mat, int depth)
{
	Vector3 light_sum;
	Vertex3 start = pos + normal * 0.001;
	for (unsigned int l = 0; l < scene.lights.size(); ++l) {
		Vector3 lt = scene.lights[l].pos - start;
		if (normal.dot(lt) > 0 && mat.Ns < 1000) {
			bool f = true;
			for (unsigned int obj = 0; obj < scene.objects.size(); ++obj) {
				intersect info = scene.objects[obj]->intersect_ray(
						scene, start, lt.normalize(), true, depth);
				if (info.valid && info.t < lt.length()) {
					f = false;
					break;
				}
			}
			if (f) {
				light_sum += get_Phong_light(mat, normal, dir, lt,
						scene.lights[l].intensity / pow(lt.length(), 2));
			}
		}
	}
	Vector3 ref_dir = mirror(dir * (-1), normal).normalize();
	intersect info;
	info.valid = false;
	info.t = 0;
	if (mat.Ns > 0 && depth > 0) {
		info = scene.intersect_ray(start, ref_dir, false, depth - 1);
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

Object::intersect Scene::intersect_ray(Vertex3 pos, Vector3 dir, bool shadow,
		int depth)
{
	Object::intersect info;
	info.valid = false;
	info.t = 0;

	for (unsigned int i = 0; i < objects.size(); ++i) {
		Object::intersect info_t = objects[i]->intersect_ray(*this,
				pos, dir.normalize(), shadow, depth);
		if (!info.valid || (info_t.valid && (info_t.t < info.t))) {
			info = info_t;
		}
	}

	return info;
}
