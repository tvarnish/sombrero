// misc.h
// A collection of miscellaneous functions required by a number of other functions/objects

#ifndef __MISC_H_INCLUDED__
#define __MISC_H_INCLUDED__

#include <iostream>

using namespace std;

int GetLengthOfNumber(int number);
string PadWithZeroes(int frameNumber, int maxFrames);
string RemoveTrailingZeroes(string inputString);
string ToStandardForm(double value);

double Random(double min, double max);

bool FileExists(string fileName);

double ToRadians(double angle);

#endif
