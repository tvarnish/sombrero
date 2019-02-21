CXX = g++
# -Wall useful for development (shows all warnings)
CXXFLAGS = -Wall -std=c++11
EXEC = sombrero

# Make sure that there is a build directory, so that the output files can be generated.
MAKEDIR_BUILD = mkdir -p build
MAKEDIR_IMAGE = mkdir -p images
MAKEDIR_DATA = mkdir -p data

# Build executable
sombrero: build/sombrero.o build/body.o build/output.o build/misc.o build/matrix.o build/position_vector.o build/linkedlist.o
	$(CXX) $(CXXFLAGS) -g -o $(EXEC) build/sombrero.o build/body.o build/output.o build/misc.o build/matrix.o build/position_vector.o build/linkedlist.o

# Build sombrero.o
build/sombrero.o: src/sombrero.cpp src/sombrero.h src/lib/body.h src/lib/output.h src/lib/misc.h src/lib/units.h src/lib/position_vector.h src/lib/matrix.h
	$(CXX) $(CXXFLAGS) -c src/sombrero.cpp -o build/sombrero.o

# Build body.o
build/body.o: src/lib/body.cpp src/lib/body.h
	$(CXX) $(CXXFLAGS) -c src/lib/body.cpp -o build/body.o

# Build output.o
build/output.o: src/lib/output.cpp src/lib/output.h src/lib/body.h
	$(CXX) $(CXXFLAGS) -c src/lib/output.cpp -o build/output.o

# Build misc.o
build/misc.o: src/lib/misc.cpp src/lib/misc.h
	$(CXX) $(CXXFLAGS) -c src/lib/misc.cpp -o build/misc.o

# Build position_vector.o
build/position_vector.o: src/lib/position_vector.cpp src/lib/position_vector.h src/lib/matrix.h src/lib/misc.h
	$(CXX) $(CXXFLAGS) -c src/lib/position_vector.cpp -o build/position_vector.o

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
	$(MAKEDIR_DATA)
	make
