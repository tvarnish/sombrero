#ifndef __MISC_H_INCLUDED__
#define __MISC_H_INCLUDED__

#include <iostream>

class Body;

void LoadParametersFromFile(std::string filename, int & bodyCount, int & width, int & height, double & scale);
void LoadBodiesFromFile(std::string filename, Body * bodyArray []);
void CleanUpBodyArray(Body * bodyArray [], int bodyCount);
int NumberLength(int number);
std::string PadWithZeroes(int frameNumber, int maxCount);
int RandomInteger(int min, int max);
double Random(double min, double max);
bool FileExists(std::string filename);

#endif
