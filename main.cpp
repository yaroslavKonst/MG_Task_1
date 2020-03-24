#include <iostream>
#include <cstdint>

#include <string>
#include <vector>
#include <unordered_map>
#include <omp.h>

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

	Scene scene;

	std::string outFilePath = "zout.bmp";
	if (cmdLineParams.find("-out") != cmdLineParams.end())
		outFilePath = cmdLineParams["-out"];

	int sceneId = 0;
	if (cmdLineParams.find("-scene") != cmdLineParams.end())
		sceneId = atoi(cmdLineParams["-scene"].c_str());

	int threadNum = 0;
	if (cmdLineParams.find("-threads") != cmdLineParams.end())
		threadNum = atoi(cmdLineParams["-threads"].c_str());

	if (threadNum > 0) {
		omp_set_num_threads(threadNum);
	}

// Material definition.
	Material blue(Vector3(0, 0, 0.4), Vector3(0.5, 0.5, 0.5),
			300, 1, 40);

	Material green1(Vector3(0, 0.9, 0), Vector3(0.1, 0.1, 0.1),
			500, 1, 40);

	Material mirror(Vector3(0.7, 0.7, 0.7), Vector3(0.2, 0.2, 0.2),
			800, 1, 40);

	Material red(Vector3(0.8, 0, 0), Vector3(0.1, 0.1, 0.1),
			200, 1, 40);

	Material green(Vector3(0, 0.9, 0), Vector3(0.1, 0.1, 0.1),
			300, 1, 40);


	std::cout << "Loading models.\n";

// SCENE 0
	Sphere sp_bl(Vertex3(0, -20, 300), 50, blue);
	Sphere sp_bl3(Vertex3(120, -20, 300), 50, blue);

	Sphere sp_bl1(Vertex3(-80, -40, 350), 20, green1);

	Mesh plane("objects/planeAssembly.obj", -100, 140, 400, M_PI, M_PI / 8,
			-M_PI / 10);

	Sphere sp(Vertex3(100, 100, 700), 50, mirror);

	Sphere sp1(Vertex3(0, 50, 800), 30, red);

	Sphere sp_gr(Vertex3(150, 200, 600), 50, green);

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

// SCENE 1
	Endless_plane back;
	back.vertices[0] = Vertex3(0, 0, -100);
	back.vertices[1] = Vertex3(-100, 100, -100);
	back.vertices[2] = Vertex3(100, 100, -100);
	back.normals[0] = Vector3(0, 0, 1);
	back.normals[1] = Vector3(0, 0, 1);
	back.normals[2] = Vector3(0, 0, 1);
	back.mat.Kd = Vector3(0, 0, 0);
	back.mat.Ks = Vector3();
	back.mat.Ns = 0;

	Endless_plane floor;
	floor.vertices[0] = Vertex3(0, -100, 0);
	floor.vertices[1] = Vertex3(-100, -100, 100);
	floor.vertices[2] = Vertex3(100, -100, 100);
	floor.normals[0] = Vector3(0, 1, 0);
	floor.normals[1] = Vector3(0, 1, 0);
	floor.normals[2] = Vector3(0, 1, 0);
	floor.mat.Kd = Vector3(0.7, 0.7, 0.7);
	floor.mat.Ks = Vector3();
	floor.mat.Ns = 0;

	Endless_plane left;
	left.vertices[0] = Vertex3(100, 0, 0);
	left.vertices[1] = Vertex3(100, 100, 100);
	left.vertices[2] = Vertex3(100, 100, -100);
	left.normals[0] = Vector3(-1, 0, 0);
	left.normals[1] = Vector3(-1, 0, 0);
	left.normals[2] = Vector3(-1, 0, 0);
	left.mat.Kd = Vector3(0, 0.7, 0);
	left.mat.Ks = Vector3();
	left.mat.Ns = 0;

	Endless_plane right;
	right.vertices[0] = Vertex3(-100, 0, 0);
	right.vertices[1] = Vertex3(-100, 100, 100);
	right.vertices[2] = Vertex3(-100, 100, -100);
	right.normals[0] = Vector3(1, 0, 0);
	right.normals[1] = Vector3(1, 0, 0);
	right.normals[2] = Vector3(1, 0, 0);
	right.mat.Kd = Vector3(0.7, 0, 0);
	right.mat.Ks = Vector3();
	right.mat.Ns = 0;

	Endless_plane up;
	up.vertices[0] = Vertex3(0, 100, 0);
	up.vertices[1] = Vertex3(-100, 100, 100);
	up.vertices[2] = Vertex3(100, 100, 100);
	up.normals[0] = Vector3(0, -1, 0);
	up.normals[1] = Vector3(0, -1, 0);
	up.normals[2] = Vector3(0, -1, 0);
	up.mat.Kd = Vector3(0.7, 0.7, 0.7);
	up.mat.Ks = Vector3();
	up.mat.Ns = 0;

	Endless_plane front;
	front.vertices[0] = Vertex3(0, 0, 300);
	front.vertices[1] = Vertex3(-100, 100, 300);
	front.vertices[2] = Vertex3(100, 100, 300);
	front.normals[0] = Vector3(0, 0, -1);
	front.normals[1] = Vector3(0, 0, -1);
	front.normals[2] = Vector3(0, 0, -1);
	front.mat.Kd = Vector3(0.7, 0.7, 0.7);
	front.mat.Ks = Vector3();
	front.mat.Ns = 0;

	Sphere sp3(Vertex3(60, -60, 260), 40, mirror);

// CAMERA
	Camera cam;

	if (sceneId == 0) {
		scene.add(&sp_bl);
		scene.add(&sp_bl1);
		scene.add(&sp_bl3);
		scene.add(&plane);
		scene.add(&sp);
		scene.add(&sp1);
		scene.add(&sp_gr);
		scene.add(&plain);
		scene.add(Light(Vertex3(-500, 100, 0), 500000));
		scene.add(Light(Vertex3(500, 100, 200), 100000));
		scene.add(Light(Vertex3(400, 0, 400), 20000));
		scene.add(Light(Vertex3(-200, 100, 900), 100000));
		scene.add(Light(Vertex3(400, 100, 900), 100000));

		cam.position = Vertex3(0, 100, 0);
		cam.phi = 0;
		cam.psi = M_PI / 16;
		cam.fov = M_PI / 2;
	} else if (sceneId == 1) {
		scene.add(&front);
		scene.add(&back);
		scene.add(&floor);
		scene.add(&up);
		scene.add(&left);
		scene.add(&right);
		scene.add(&sp3);
		scene.add(Light(Vertex3(0, 80, 100), 40000));

		cam.position = Vertex3(0, 0, -80);
		cam.phi = 0;
		cam.psi = 0;
		cam.fov = M_PI / 2;
	} else if (sceneId == 2) {
		scene.add(&plain);
		scene.add(Light(Vertex3(0, 120, 500), 400000));
		cam.position = Vertex3(0, 100, 0);
		cam.phi = M_PI / 3;
		cam.psi = 0;
		cam.fov = M_PI / 2;
	}

	uint32_t width = 1000;
	uint32_t height = 1000;

	std::cout << "Rendering.\n";

	std::vector<uint32_t> image(width * height);

	#pragma omp parallel for shared(image, width, height) schedule(dynamic)
	for (uint32_t i = 0; i < width; ++i) {
		if (i % 100 == 0) {
			#pragma omp critical
			{
				std::cout << i << std::endl;
			}
		}
		for (uint32_t j = 0; j < height; ++j) {
			Vector3 dir = cam.getDir(i, j, width, height);

			Object::intersect info = scene.intersect_ray(cam.position,
					dir.normalize(), false);

			uint32_t color;

			if (info.valid) {
				color = to_RGB(info.color);
			} else {
				color = 0; // RED | GREEN | BLUE;
			}

			#pragma omp critical
			{
				image[j*height + width - i - 1] = color;
			}
		}
	}

/*	// Filtering
	std::cout << "Filtering.\n";
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
