CXX = g++
CXXFLAGS = -Wall
EXEC = sombrero

sombrero: sombrero.o font.o body.o image.o video.o output.o misc.o
	$(CXX) $(CXXFLAGS) -o $(EXEC) sombrero.o font.o body.o image.o video.o output.o misc.o

sombrero.o: sombrero.cpp libs/font.h libs/body.h libs/image.h libs/video.h libs/output.h libs/misc.h
	$(CXX) $(CXXFLAGS) -c sombrero.cpp

font.o: libs/font.cpp libs/font.h
	$(CXX) $(CXXFLAGS) -c libs/font.cpp

body.o: libs/body.cpp libs/body.h
	$(CXX) $(CXXFLAGS) -c libs/body.cpp

image.o: libs/image.cpp libs/image.h libs/body.h libs/font.h
	$(CXX) $(CXXFLAGS) -c libs/image.cpp

video.o: libs/video.cpp libs/video.h
	$(CXX) $(CXXFLAGS) -c libs/video.cpp

output.o: libs/output.cpp libs/output.h libs/body.h
	$(CXX) $(CXXFLAGS) -c libs/output.cpp

misc.o: libs/misc.cpp libs/misc.h libs/body.h
	$(CXX) $(CXXFLAGS) -c libs/misc.cpp

clean:
	-rm $(EXEC) *.o
