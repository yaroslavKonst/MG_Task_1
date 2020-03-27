rt: main.cpp vector_functions.o Bitmap.o camera.o mesh.o loader.o primitives.o object.o
	g++ -std=gnu++11 -Wall -O3 -fopenmp main.cpp Bitmap.o object.o vector_functions.o camera.o mesh.o loader.o primitives.o -o rt -g

%.o: %.cpp %.h base_classes.h base_functions.h light.h rgb.h texture.h material.h vector_functions.h refr_info.h
	g++ -std=gnu++11 -Wall -O3 -c $< -g
