#include "object.h"

std::mt19937 Object::random_engine = std::mt19937();

Vector3 Object::calculate_light(Vertex3 pos, Vector3 normal, Vector3 dir,
		Scene &scene, Material mat, int depth, bool path)
{
	if (mat.d == 1.0) {
		Vector3 light_sum;
		Vertex3 start = pos + normal * 0.001;

		for (unsigned int l = 0; l < scene.lights.size(); ++l) {
			Vector3 lt = scene.lights[l].pos - start;
			if (normal.dot(lt) > 0 && mat.Ns < 1000) {
				bool f = true;
				for (unsigned int obj = 0; obj < scene.objects.size(); ++obj) {
					intersect info = scene.objects[obj]->intersect_ray(
							scene, start, lt.normalize(), true, depth,
							path);
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

		if (path && depth > 0 && mat.Ns < 1000) {
			std::uniform_real_distribution<double> distr(-M_PI / 2, M_PI / 2);

			Vector3 e1(1, 0, 0);
			if (module(e1.dot(normal)) > 0.9) {
				e1 = Vector3(0, 1, 0);
			}

			e1 = (e1 - (normal * e1.dot(normal))).normalize();
			Vector3 e2 = (e1*normal).normalize();

			double ang1 = distr(random_engine);
			double ang2 = distr(random_engine);

			Vector3 path_dir = (normal * cos(ang1) * cos(ang2) +
					e1 * sin(ang1) + e2 * sin(ang2)).normalize();

			intersect info = scene.intersect_ray(start, path_dir, false,
					depth - 1, path);

			double ang_cos = path_dir.dot(normal);

			if (info.valid) {
				light_sum += Vector3(mat.Kd.X() * info.color.X(),
						mat.Kd.Y() * info.color.Y(),
						mat.Kd.Z() * info.color.Z()) * ang_cos;
			}
		}

		Vector3 ref_dir = mirror(dir * (-1), normal).normalize();
		intersect info;
		info.valid = false;
		info.t = 0;
		if (mat.Ns > 0 && depth > 0) {
			info = scene.intersect_ray(start, ref_dir, false, depth - 1,
					path);
		}
		if (info.valid) {
			light_sum = (light_sum * (1 - (mat.Ns / 1000))) +
					(info.color * (mat.Ns / 1000));
		} else {
			light_sum = light_sum * (1 - (mat.Ns / 1000));
		}

		return light_sum;
	} else {
		if (depth == 0) {
			return Vector3();
		}
		Vertex3 start;
		Vertex3 start_refl;
		double n1, n2, refl_coeff;
		Vector3 refr_dir;
		bool refr_s;
		if (dir.dot(normal) > 0) {
			n1 = mat.N;
			n2 = 1;
			refr_dir = Snell_refr(dir.normalize(), normal * -1, n1, n2, refr_s);
			if (refr_s) {
				start = pos + normal * 0.1;
				start_refl = pos + normal * -0.1;
				refl_coeff = Fresnel_coeff(n1, n2, (dir).dot(normal));
			} else {
				start = pos + normal * -0.1;
			}
		} else {
			n1 = 1;
			n2 = mat.N;
			refr_dir = Snell_refr(dir.normalize(), normal, n1, n2, refr_s);
			if (refr_s) {
				start = pos + normal * -0.1;
				start_refl = pos + normal * 0.1;
				refl_coeff = Fresnel_coeff(n1, n2, (dir*-1).dot(normal));
			} else {
				start = pos + normal * 0.1;
			}
		}
		intersect info;
		info.valid = false;
		info.t = 0;

		info = scene.intersect_ray(start, refr_dir, false, depth,
				path);

		intersect info_refl;
		info_refl.valid = false;
		info_refl.t = 0;

		if (refr_s) {
			info_refl = scene.intersect_ray(start_refl, mirror(dir * -1,
					normal).normalize(), false, depth - 1, path);
			Vector3 light;
			if (info.valid) {
				light += info.color * (1 - refl_coeff);
			}
			if (info_refl.valid) {
				light += info_refl.color * refl_coeff;
			}
			info.color = light;
			info.valid = true;
		}

		if (info.valid) {
			return info.color;
		} else {
			return Vector3();
		}
	}
}

Object::intersect Scene::intersect_ray(Vertex3 pos, Vector3 dir, bool shadow,
		int depth, bool path)
{
	Object::intersect info;
	info.valid = false;
	info.t = 0;
	Object *targ_o = 0;

	for (unsigned int i = 0; i < objects.size(); ++i) {
		Object::intersect info_t = objects[i]->intersect_ray(*this,
				pos, dir.normalize(), true, depth, path);
		if (!info.valid || (info_t.valid && (info_t.t < info.t))) {
			info = info_t;
			targ_o = objects[i];
		}
	}

	if (info.valid && !shadow) {
		info = targ_o->intersect_ray(*this, pos, dir.normalize(), shadow,
				depth, path);
		info.object = targ_o;
	}

	return info;
}

Object::intersect Scene::trace_path(Vertex3 pos, Vector3 dir, int depth,
		int rays)
{
	Object::intersect info;

	info = intersect_ray(pos, dir, false, depth, true);

	if (info.valid) {

		for (int k = 1; k < rays; ++k) {
			Object::intersect info_loc = info.object->intersect_ray(
					*this, pos, dir, false, depth, true);
			if (info_loc.valid) {
				info.color += info_loc.color;
			}
		}

		info.color = info.color * (1.0 / (double)rays);
	}

	return info;
}
