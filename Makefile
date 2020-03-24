rt: main.cpp vector_functions.o Bitmap.o camera.o mesh.o loader.o primitives.o object.o
	g++ -Wall -O3 -fopenmp main.cpp Bitmap.o object.o vector_functions.o camera.o mesh.o loader.o primitives.o -o rt -std=gnu++11 -g

%.o: %.cpp %.h base_classes.h base_functions.h light.h rgb.h texture.h material.h
	g++ -Wall -O3 -c $< -g
