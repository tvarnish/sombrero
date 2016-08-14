CC=g++
SOURCES=sombrero.cpp libs/font.cpp libs/body.cpp libs/image.cpp libs/video.cpp libs/output.cpp libs/misc.cpp
TARGET=sombrero

all:
	$(CC) $(SOURCES) -o $(TARGET)
