CXX = g++
CXXFLAGS = -Wall -std=c++11 `freetype-config --cflags`
LIBS = -lpng -lpngwriter -lz -lfreetype
EXEC = sombrero

# Make sure that there is a build directory, so that the output files can be generated.
MAKEDIR_BUILD = mkdir -p build
MAKEDIR_IMAGE = mkdir -p images

# Build the executable
#sombrero: build/sombrero.o build/font.o build/body.o build/image.o build/video.o build/output.o build/misc.o build/matrix.o build/linkedlist.o
#	$(CXX) $(CXXFLAGS) -g -o $(EXEC) build/sombrero.o build/font.o build/body.o build/image.o build/video.o build/output.o build/misc.o build/matrix.o build/linkedlist.o $(LIBS)

# Build testing executable
sombrero: build/testing.o build/sombrero.o build/font.o build/body.o build/image.o build/video.o build/output.o build/misc.o build/matrix.o build/linkedlist.o
	$(CXX) $(CXXFLAGS) -g -o $(EXEC) build/testing.o build/sombrero.o build/font.o build/body.o build/image.o build/video.o build/output.o build/misc.o build/matrix.o build/linkedlist.o $(LIBS)

# Build testing.o
build/testing.o: src/testing.cpp src/sombrero.h
	$(CXX) $(CXXFLAGS) -c src/testing.cpp -o build/testing.o

# Build sombrero.o
build/sombrero.o: src/sombrero.cpp src/sombrero.h src/lib/font.h src/lib/body.h src/lib/image.h src/lib/video.h src/lib/output.h src/lib/misc.h src/lib/units.h
	$(CXX) $(CXXFLAGS) -c src/sombrero.cpp -o build/sombrero.o

# Build font.o
build/font.o: src/lib/font.cpp src/lib/font.h
	$(CXX) $(CXXFLAGS) -c src/lib/font.cpp -o build/font.o

# Build body.o
build/body.o: src/lib/body.cpp src/lib/body.h
	$(CXX) $(CXXFLAGS) -c src/lib/body.cpp -o build/body.o

# Build image.o
build/image.o: src/lib/image.cpp src/lib/image.h src/lib/body.h src/lib/font.h
	$(CXX) $(CXXFLAGS) -c src/lib/image.cpp -o build/image.o

# Build video.o
build/video.o: src/lib/video.cpp src/lib/video.h
	$(CXX) $(CXXFLAGS) -c src/lib/video.cpp -o build/video.o

# Build output.o
build/output.o: src/lib/output.cpp src/lib/output.h src/lib/body.h
	$(CXX) $(CXXFLAGS) -c src/lib/output.cpp -o build/output.o

# Build misc.o
build/misc.o: src/lib/misc.cpp src/lib/misc.h src/lib/body.h
	$(CXX) $(CXXFLAGS) -c src/lib/misc.cpp -o build/misc.o

# Build matrix.o
build/matrix.o: src/lib/matrix.cpp src/lib/matrix.h
	$(CXX) $(CXXFLAGS) -c src/lib/matrix.cpp -o build/matrix.o

# Build linkedlist.o
build/linkedlist.o: src/lib/linkedlist.cpp src/lib/linkedlist.h
	$(CXX) $(CXXFLAGS) -c src/lib/linkedlist.cpp -o build/linkedlist.o

clean:
	-rm build/*.o

# Run make setup for development and testing purposes.
setup:
	$(MAKEDIR_BUILD)
	$(MAKEDIR_IMAGE)
	make

# Run make install for everything else.