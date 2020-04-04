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
#include "material.h"

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

// Image size
	uint32_t width = 512;
	uint32_t height = 512;

// Path tracing flag
	bool path_tr = false;

	bool scene_valid = false;

// Material definition.
	Material blue(Vector3(0, 0, 0.4), Vector3(0.5, 0.5, 0.5),
			300, 1, 40);

	Material green1(Vector3(0, 0.9, 0), Vector3(0.1, 0.1, 0.1),
			700, 1, 40);

	Material mirror(Vector3(0.7, 0.7, 0.7), Vector3(0.2, 0.2, 0.2),
			800, 1, 40);

	Material red(Vector3(0.8, 0, 0), Vector3(0.1, 0.1, 0.1),
			200, 1, 40);

	Material green(Vector3(0, 0.9, 0), Vector3(0.1, 0.1, 0.1),
			300, 1, 40);

	Material transparent(Vector3(0, 0.9, 0), Vector3(0.1, 0.1, 0.1),
			300, 0, 40, 1.5);

	Material diff_green(Vector3(0, 0.7, 0), Vector3(), 0);

	Material diff_red(Vector3(0.7, 0, 0), Vector3(), 0);

	Material diff_blue(Vector3(0, 0, 0.7), Vector3(), 0);

	Material diff_white(Vector3(0.7, 0.7, 0.7), Vector3(), 0);

	Material diff_black(Vector3(0.1, 0.1, 0.1), Vector3(), 0);


	std::cout << "Loading models.\n";

// CAMERA
	Camera cam;
	cam.fov = M_PI / 2;

// Scene setup
	Scene scene;

	if (sceneId == 1) {
		// Objects
		scene.add(new Sphere(Vertex3(0, -20, 300), 50, blue));
		scene.add(new Sphere(Vertex3(0, 50, 400), 40, transparent));
		scene.add(new Sphere(Vertex3(120, -20, 300), 50, blue));
		scene.add(new Sphere(Vertex3(-80, -40, 350), 20, green1));
		scene.add(new Mesh("objects/planeAssembly.obj", -100, 140, 400,
				M_PI, M_PI / 8, -M_PI / 10));
		scene.add(new Sphere(Vertex3(100, 100, 700), 50, mirror));
		scene.add(new Sphere(Vertex3(0, 50, 800), 30, red));
		scene.add(new Sphere(Vertex3(150, 200, 600), 50, green));

		scene.add(new Endless_plane(
				Vertex3(-500, -50, 500),
				Vertex3(500, -50, 500),
				Vertex3(0, -50, 0),
				Vector3(0, 1, 0),
				Vector3(0, 1, 0),
				Vector3(0, 1, 0),
				Material(Vector3(), Vector3(), 100),
				new TexChBoard,
				Vector3(-500, 500, 0),
				Vector3(500, 500, 0),
				Vector3(0, 0, 0)));

		// Lights
		scene.add(Light(Vertex3(-500, 100, 0), 500000));
		scene.add(Light(Vertex3(500, 100, 200), 200000));
		scene.add(Light(Vertex3(400, 0, 400), 20000));
		scene.add(Light(Vertex3(-200, 100, 900), 100000));
		scene.add(Light(Vertex3(400, 100, 900), 100000));

		// Camera options
		cam.position = Vertex3(0, 100, 0);
		cam.phi = 0;
		cam.psi = M_PI / 16;

		// Image size
		width = 1024;
		height = 1024;

		scene_valid = true;

	} else if (sceneId == 2) {

		scene.add(new Endless_plane( // back
				Vertex3(0, 0, -90),
				Vertex3(-100, 100, -90),
				Vertex3(100, 100, -90),
				Vector3(0, 0, 1),
				Vector3(0, 0, 1),
				Vector3(0, 0, 1),
				diff_black));

		scene.add(new Endless_plane( // down
				Vertex3(0, -200, 0),
				Vertex3(-100, -200, 100),
				Vertex3(100, -200, 100),
				Vector3(0, 1, 0),
				Vector3(0, 1, 0),
				Vector3(0, 1, 0),
				diff_white));

		scene.add(new Endless_plane( // left
				Vertex3(200, 0, 0),
				Vertex3(200, 100, 100),
				Vertex3(200, 100, -100),
				Vector3(-1, 0, 0),
				Vector3(-1, 0, 0),
				Vector3(-1, 0, 0),
				diff_green));

		scene.add(new Endless_plane( // right
				Vertex3(-200, 0, 0),
				Vertex3(-200, 100, 100),
				Vertex3(-200, 100, -100),
				Vector3(1, 0, 0),
				Vector3(1, 0, 0),
				Vector3(1, 0, 0),
				diff_red));

		scene.add(new Endless_plane( // up
				Vertex3(0, 200, 0),
				Vertex3(-100, 200, 100),
				Vertex3(100, 200, 100),
				Vector3(0, -1, 0),
				Vector3(0, -1, 0),
				Vector3(0, -1, 0),
				diff_white));

		scene.add(new Endless_plane( // front
				Vertex3(0, 0, 600),
				Vertex3(-100, 100, 600),
				Vertex3(100, 100, 600),
				Vector3(0, 0, -1),
				Vector3(0, 0, -1),
				Vector3(0, 0, -1),
				diff_blue));

//		scene.add(new Sphere(Vertex3(60, -60, 260), 40, mirror));
		scene.add(new Sphere(Vertex3(-140, -140, 400), 60, transparent));

		scene.add(new Mesh("objects/Cube.obj", 0, 0, 480,
				M_PI / 4, M_PI / 4, M_PI / 4));

		scene.add(Light(Vertex3(40, 155, 420), 15800));
		scene.add(Light(Vertex3(40, 155, 400), 15800));
		scene.add(Light(Vertex3(-40, 155, 420), 15800));
		scene.add(Light(Vertex3(-40, 155, 400), 15800));

		cam.position = Vertex3(0, 0, -80);
		cam.phi = 0;
		cam.psi = 0;

		width = 512;
		height = 512;

		path_tr = true;
		scene_valid = true;
	}

// Rendering
	if (scene_valid) {
		std::cout << "Rendering.\n";

		std::vector<uint32_t> image(width * height);

		#pragma omp parallel for shared(scene, image, width, height) schedule(dynamic)
		for (uint32_t i = 0; i < width; ++i) {
			#pragma omp critical
			{
				if (i % 10 == 0) {
					std::cout << i << std::endl;
				}
			}
			for (uint32_t j = 0; j < height; ++j) {
				Vector3 dir = cam.getDir(i, j, width, height);

				Vector3 color;

				if (!path_tr) {
					// ray tracing
					Object::intersect info = scene.intersect_ray(cam.position,
							dir.normalize(), false, 5, false);

					if (info.valid) {
						color = info.color;
					} else {
						color = Vector3(); // RED | GREEN | BLUE;
					}
				} else {
					// path tracing
					color = Vector3();

					Object::intersect info = scene.trace_path(cam.position,
							dir.normalize(), 3, 100);
					if (info.valid) {
						color = info.color;
					}
				}

				double coeff = max(max(color.X(), color.Y()), color.Z());

				if (coeff > 1.0) {
					color = color * (1.0 / coeff);
				}

				#pragma omp critical
				{
					image[j*height + width - i - 1] = to_RGB(color);
				}
			}
		}

		scene.del();

	// Filtering
		if (path_tr) {
			for (int iter = 0; iter < 10; ++iter) {
				std::vector<uint32_t> image_fin(width * height);
				std::cout << "Filtering: " << iter << std::endl;
				for (uint32_t i = 1; i < width - 1; ++i) {
					for (uint32_t j = 1; j < height - 1; ++j) {
						Vector3 center = from_RGB(image[i*height + j]);
						Vector3 ng[4];
						ng[0] = from_RGB(image[i*height + j + 1]);
						ng[1] = from_RGB(image[i*height + j - 1]);
						ng[2] = from_RGB(image[(i + 1)*height + j]);
						ng[3] = from_RGB(image[(i - 1)*height + j]);

						double sum = 1;
						Vector3 v_sum = center;

						for (int k = 0; k < 4; ++k) {
							if ((ng[k] - center).length() < 0.05 || iter <= 3) {
								sum += 1.0;
								v_sum += ng[k];
							}
						}

						image_fin[i*height + j] = to_RGB(v_sum * (1.0 / sum));
					}
				}
				image = image_fin;
			}
		}

		std::cout << "Saving image to " << outFilePath << "." << std::endl;

		SaveBMP(outFilePath.c_str(), image.data(), width, height);
	}
	std::cout << "end." << std::endl;
	return 0;
}
