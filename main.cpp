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
	const char* input_file = "images/banana.jpg";
	auto output_file = "output.gif";

	int width, height, channels;
	uint8_t* rgb_image = stbi_load(input_file, &width, &height, &channels, 3);
    
	
	GifWriter g;
	GifBegin(&g, fileName, heat_map.size(),
				heat_map[0].size(), delay);
	for(uint i=0;i<num_frames;i++){

		eb_frame(image)

		GifWriteFrame(&g, image.data(), heat_map.size(),
				heat_map[0].size(), delay);
	}
	GifEnd(&g);

	stbi_image_free(rgb_image);

	cout << "done" << endl;

	return 0;
}