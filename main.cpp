#include <vector>
#include <cstdint>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <array>
#include <cstdio>
#include <cmath>
#include <math.h>
#include <cstdlib>
#include <functional>   // std::less
#include <algorithm>    // std::sort, std::includes
#include <string>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "gif.h"

#include "eb_frame.h"


using namespace std;

int main(int argc, char** argv){

	//INPUTS
	const char* input_file = "images/double_grad.jpg";
	auto output_file = "output.gif";

	int width, height, channels;
	uint8_t* image = stbi_load(input_file, &width, &height, &channels, 4);

	vector<RGBA> palette;

	for(int i=0;i<255;i++){
		palette.push_back({i,i,i,0});
	}
    
	const uint NUM_FRAMES = 10;
	const uint DELAY = 3;

	GifWriter g;
	GifBegin(&g, output_file, width, height, DELAY);

	for(uint i=0;i<NUM_FRAMES;i++){

		eb_frame f = eb_frame(image, width, height, double(i) / double(NUM_FRAMES));
		//f.apply_palette({{0,0,0,0},{255,255,255,255}},0,1);


		GifWriteFrame(&g, image, width, height, DELAY);
	}
	GifEnd(&g);

	stbi_image_free(image);

	cout << "done" << endl;

	return 0;
}