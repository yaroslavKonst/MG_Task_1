#include <iostream>
#include <cstdint>

#include <string>
#include <vector>
#include <unordered_map>

#include "base_classes.h"
#include "object.h"
#include "Bitmap.h"
#include "mesh.h"
#include "camera.h"
#include "light.h"
#include "primitives.h"
#include "rgb.h"
#include "texture.h"

const uint32_t RED   = 0x000000FF;
const uint32_t GREEN = 0x0000FF00;
const uint32_t BLUE  = 0x00FF0000;


int main(int argc, const char **argv)
{
	std::unordered_map<std::string, std::string> cmdLineParams;

	for (int i = 0; i < argc; i++) {
		std::string key(argv[i]);

		if (key.size() > 0 && key[0] == '-') {
			if (i != argc-1) { // not last argument
				cmdLineParams[key] = argv[i+1];
				i++;
			} else
				cmdLineParams[key] = "";
		}
	}

	std::vector<Object*> objects;
	std::vector<Light> lights;

	std::string outFilePath = "zout.bmp";
	if (cmdLineParams.find("-out") != cmdLineParams.end())
		outFilePath = cmdLineParams["-out"];

	int sceneId = 0;
	if (cmdLineParams.find("-scene") != cmdLineParams.end())
		sceneId = atoi(cmdLineParams["-scene"].c_str());

	uint32_t color = 0;

	std::cout << "Loading models.\n";

	Sphere sp_bl(Vertex3(0, -20, 300), 50, Vector3(0, 0, 0.4),
		Vector3(0.5, 0.5, 0.5), 300, 1, 40);

	Sphere sp_bl1(Vertex3(-80, -40, 350), 20, Vector3(0, 0.9, 0),
		Vector3(0.1, 0.1, 0.1), 500, 1, 40);

	Mesh plane("objects/planeAssembly.obj", -100, 140, 400, M_PI, M_PI / 8,
		-M_PI / 10);

	Sphere sp(Vertex3(100, 100, 700), 50, Vector3(0.7, 0.7, 0.7),
		Vector3(0.2, 0.2, 0.2), 800, 1, 40);

	Sphere sp1(Vertex3(0, 50, 800), 30, Vector3(0.8, 0, 0),
		Vector3(0.1, 0.1, 0.1), 200, 1, 40);

	Sphere sp_gr(Vertex3(150, 200, 600), 50, Vector3(0, 0.9, 0),
		Vector3(0.1, 0.1, 0.1), 300, 1, 40);

	Endless_plane plain;
	plain.vertices[0] = Vertex3(-500, -50, 500);
	plain.vertices[1] = Vertex3(500, -50, 500);
	plain.vertices[2] = Vertex3(0, -50, 0);
	plain.normals[0] = Vector3(0, 1, 0);
	plain.normals[1] = Vector3(0, 1, 0);
	plain.normals[2] = Vector3(0, 1, 0);
	plain.mat.Kd = Vector3();
	plain.mat.Ks = Vector3();
	plain.mat.Ns = 100;
	plain.mat.alpha = 1400;
	plain.texture = new TexChBoard;
	plain.textures[0] = Vector3(-500, 500, 0);
	plain.textures[1] = Vector3(500, 500, 0);
	plain.textures[2] = Vector3(0, 0, 0);

	lights.push_back(Light(Vertex3(-500, 100, 0), 500000));
	lights.push_back(Light(Vertex3(500, 100, 200), 100000));
	lights.push_back(Light(Vertex3(400, 0, 400), 20000));
	lights.push_back(Light(Vertex3(-200, 100, 900), 100000));
	lights.push_back(Light(Vertex3(400, 100, 900), 100000));

	objects.push_back(&sp_bl);
	objects.push_back(&sp_bl1);
	objects.push_back(&plane);
	objects.push_back(&sp);
	objects.push_back(&sp1);
	objects.push_back(&sp_gr);
	objects.push_back(&plain);

	Camera cam;
	cam.position = Vertex3(0, 100, 0);
	cam.phi = 0;
	cam.psi = M_PI / 16;
	cam.fov = M_PI / 2;

	uint32_t width = 512;
	uint32_t height = 512;

	std::cout << "Rendering.\n";

	std::vector<uint32_t> image(width * height);

	#pragma omp parallel for private(color) shared(image, width, height) schedule(dynamic)
	for (uint32_t i = 0; i < width; ++i) {
		if (i % 100 == 0) {
			#pragma omp critical
			{
				std::cout << i << std::endl;
			}
		}
		for (uint32_t j = 0; j < height; ++j) {
			Vector3 dir = cam.getDir(i, j, width, height);

			Object::intersect info;
			info.valid = false;
			info.t = 0;

			for (unsigned int k = 0; k < objects.size(); ++k) {
				Object::intersect info1 = objects[k]->intersect_ray(objects,
					lights, cam.position, dir.normalize(), false);
				if (!info.valid || (info1.valid && (info1.t < info.t))) {
					info = info1;
				}
			}

			if (info.valid) {
				color = to_RGB(info.color);
			} else {
				color = 0; // RED | GREEN | BLUE;
			}

			#pragma omp critical
			{
				image[j*height + width - i] = color;
			}
		}
	}

/*	// Sampling
	std::cout << "Sampling.\n";
	std::vector<uint32_t> image_fin(width * height);

	for (uint32_t i = 1; i < width - 1; ++i) {
		for (uint32_t j = 1; j < height - 1; ++j) {
			image_fin[i*height + j] = to_RGB(
				(from_RGB(image[i*height + j]) +
				from_RGB(image[i*height + j + 1]) +
				from_RGB(image[i*height + j - 1]) +
				from_RGB(image[(i + 1)*height + j]) +
				from_RGB(image[(i - 1)*height + j])) * (1.0 / 5.0));
		}
	}
*/
	std::cout << "Saving image to " << outFilePath << "." << std::endl;

	SaveBMP(outFilePath.c_str(), image.data(), width, height);

	std::cout << "end." << std::endl;
	return 0;
}
