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
	if (sceneId == 1)
		color = RED;
	else if (sceneId == 2)
		color = RED | GREEN;
	else if (sceneId == 3)
		color = BLUE;

	std::cout << "Loading models.\n";

/*	Mesh sp_bl("objects/sphere_blue.obj", -20, 0, 300, 0, 0, 0); */
	Sphere sp_bl(Vertex3(-20, 0, 300), 50, Vector3(0, 0, 0.9), Vector3(),
		300, 1);

	Mesh plane("objects/planeAssembly.obj", 140, 220, 600, 3*M_PI/4, 0,
		-M_PI / 2);

/*	Mesh sp("objects/sphere.obj", 100, -100, 700, 0, 0, 0); */
	Sphere sp(Vertex3(100, -100, 700), 50, Vector3(0.7, 0.7, 0.7), Vector3(),
		800, 1);

/*	Mesh sp_gr("objects/sphere_green.obj", 200, -150, 600, 0, 0, 0); */
	Sphere sp_gr(Vertex3(200, -150, 600), 50, Vector3(0, 0.9, 0), Vector3(),
		300, 1);

	Endless_plane plain;
	plain.vertices[0] = Vertex3(-50, -500, 500);
	plain.vertices[1] = Vertex3(-50, 500, 500);
	plain.vertices[2] = Vertex3(-50, 0, 0);
	plain.normals[0] = Vector3(1, 0, 0);
	plain.normals[1] = Vector3(1, 0, 0);
	plain.normals[2] = Vector3(1, 0, 0);
	plain.Kd = Vector3(0.7, 0.7, 0.7);
	plain.Ks = Vector3(0, 0, 0);
	plain.Ns = 100;
	plain.texture = new TexChBoard;
	plain.textures[0] = Vector3(-500, 500, 0);
	plain.textures[1] = Vector3(500, 500, 0);
	plain.textures[2] = Vector3(0, 0, 0);


	Endless_plane back;
	back.vertices[0] = Vertex3(1000, 2000, 900);
	back.vertices[1] = Vertex3(1000, -2000, 900);
	back.vertices[2] = Vertex3(-500, 0, 900);
	back.normals[0] = Vector3(0, 0, -1);
	back.normals[1] = Vector3(0, 0, -1);
	back.normals[2] = Vector3(0, 0, -1);
	back.Kd = Vector3(0.7, 0.7, 0.7);
	back.Ks = Vector3(0, 0, 0);
	back.Ns = 100;

	lights.push_back(Light(Vertex3(100, 500, 0), 500000));
	lights.push_back(Light(Vertex3(100, -500, 200), 100000));
	lights.push_back(Light(Vertex3(0, -400, 400), 20000));

	objects.push_back(&sp_bl);
	objects.push_back(&plane);
	objects.push_back(&sp);
	objects.push_back(&sp_gr);
	objects.push_back(&plain);
	objects.push_back(&back);

	Camera cam;
	cam.position = Vertex3(0, 0, 0);
	cam.phi = 0;
	cam.psi = 0;
	cam.fov = M_PI / 2;

	uint32_t width = 1400;
	uint32_t height = 1400;

	std::cout << "Rendering.\n";

	std::vector<uint32_t> image(width * height);

	#pragma omp parallel for private(color) shared(image, width, height) schedule(dynamic)
	for (uint32_t i = 0; i < width; ++i) {
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
				image[i*height + j] = color;
			}
		}
	}

	SaveBMP(outFilePath.c_str(), image.data(), width, height);

	std::cout << "end." << std::endl;
	return 0;
}
