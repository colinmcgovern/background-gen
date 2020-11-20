#ifndef UTILITY_H
#define UTILITY_H

#include <array>

#include "gif.h"
#include "frame.h"

typedef array<int, 4> RGBA;

vector<string> split(const string& str, const string& delim);
RGBA convert_to_RGBA(string s);
void print(vector<vector<double> > v);
void print(vector<uint8_t> input, uint width, uint height);

class utility
{
public:

};

#endif // UTILITY_H
