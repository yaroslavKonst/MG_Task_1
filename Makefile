rt: main.cpp Bitmap.o camera.o mesh.o loader.o primitives.o object.o
	g++ -Wall -O3 -fopenmp main.cpp Bitmap.o object.o camera.o mesh.o loader.o primitives.o -o rt -std=gnu++11 -g

%.o: %.cpp %.h base_classes.h light.h rgb.h
	g++ -Wall -O3 -c $< -g
