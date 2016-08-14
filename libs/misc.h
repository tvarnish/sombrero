#ifndef __MISC_H_INCLUDED__
#define __MISC_H_INCLUDED__

#include <iostream>

class Body;

void LoadParametersFromFile(std::string filename, int & bodyCount, int & width, int & height, std::string & positionUnits, double & scale, std::string & massUnits);
void LoadBodiesFromFile(std::string filename, Body * bodyArray []);
void CleanUpBodyArray(Body * bodyArray [], int bodyCount);
int NumberLength(int number);
std::string PadWithZeroes(int frameNumber, int maxCount);

#endif
