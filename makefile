CXX = g++
CXXFLAGS = -Wall
EXEC = sombrero

build/sombrero: build/sombrero.o build/font.o build/body.o build/image.o build/video.o build/output.o build/misc.o
	$(CXX) $(CXXFLAGS) -o build/$(EXEC) build/sombrero.o build/font.o build/body.o build/image.o build/video.o build/output.o build/misc.o

build/sombrero.o: src/sombrero.cpp src/lib/font.h src/lib/body.h src/lib/image.h src/lib/video.h src/lib/output.h src/lib/misc.h
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

clean:
	-rm build/$(EXEC) build/*.o
