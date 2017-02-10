#ifndef __MISC_H_INCLUDED__
#define __MISC_H_INCLUDED__

#include <iostream>
#include "linkedlist.h"

class Body;

//void LoadBodiesFromFile(std::string filename, List & bodyList);
void LoadFromFile(std::string filename, std::string & simulationName, int & width, int & height, double & scale, int & frameRate, double & timeStep, List & bodyList);

int NumberLength(int number);
std::string PadWithZeroes(int frameNumber, int maxCount);
int RandomInteger(int min, int max);
double Random(double min, double max);
bool FileExists(std::string filename);
std::string RemoveTrailingZeroes(std::string inputString);

#endif
