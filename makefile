CXX = g++
CXXFLAGS = -Wall -std=c++0x -I/usr/include/freetype2 -lpngwriter -lpng -lfreetype
EXEC = sombrero
LIBS = -I/opt/local/include/freetype2 -I/opt/local/include -I/usr/local/include -L/usr/local/lib -L/opt/local/lib -lpng -lpngwriter -lz -lfreetype

# Make sure that there is a build directory, so that the output files can be generated.
MAKEDIR_BUILD = mkdir -p build
MAKEDIR_IMAGE = mkdir -p images
MAKEDIR_INIT = mkdir -p init

sombrero: build/sombrero.o build/font.o build/body.o build/image.o build/video.o build/output.o build/misc.o build/matrix.o build/linkedlist.o
	$(CXX) $(CXXFLAGS) -g -o $(EXEC) build/sombrero.o build/font.o build/body.o build/image.o build/video.o build/output.o build/misc.o build/matrix.o build/linkedlist.o $(LIBS)

build/sombrero.o: src/sombrero.cpp src/lib/font.h src/lib/body.h src/lib/image.h src/lib/video.h src/lib/output.h src/lib/misc.h src/lib/units.h
	$(CXX) $(CXXFLAGS) -c src/sombrero.cpp -o build/sombrero.o

build/font.o: src/lib/font.cpp src/lib/font.h
	$(CXX) $(CXXFLAGS) -c src/lib/font.cpp -o build/font.o

build/body.o: src/lib/body.cpp src/lib/body.h
	$(CXX) $(CXXFLAGS) -c src/lib/body.cpp -o build/body.o

build/image.o: src/lib/image.cpp src/lib/image.h src/lib/body.h src/lib/font.h
	$(CXX) $(CXXFLAGS) -c src/lib/image.cpp -o build/image.o

build/video.o: src/lib/video.cpp src/lib/video.h
	$(CXX) $(CXXFLAGS) -c src/lib/video.cpp -o build/video.o

build/output.o: src/lib/output.cpp src/lib/output.h src/lib/body.h
	$(CXX) $(CXXFLAGS) -c src/lib/output.cpp -o build/output.o

build/misc.o: src/lib/misc.cpp src/lib/misc.h src/lib/body.h
	$(CXX) $(CXXFLAGS) -c src/lib/misc.cpp -o build/misc.o

build/matrix.o: src/lib/matrix.cpp src/lib/matrix.h
	$(CXX) $(CXXFLAGS) -c src/lib/matrix.cpp -o build/matrix.o

build/linkedlist.o: src/lib/linkedlist.cpp src/lib/linkedlist.h
	$(CXX) $(CXXFLAGS) -c src/lib/linkedlist.cpp -o build/linkedlist.o

clean:
	-rm build/*.o

# Run make setup for development and testing purposes.
setup:
	$(MAKEDIR_BUILD)
	$(MAKEDIR_IMAGE)
	$(MAKEDIR_INIT)
	make

# Run make install for everything else.
