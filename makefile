CC=g++
SOURCES=sombrero.cpp libs/font.cpp
TARGET=sombrero

all:
	$(CC) $(SOURCES) -o $(TARGET)
