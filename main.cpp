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
	const char* input_file = "images/landscape.jpg";

	int width, height, channels;
	uint8_t* rgb_image = stbi_load(input_file, &width, &height, &channels, 3);

	const uint num_frames = 1;
	const uint delay = 3;

	const uint pallete_num = 11;
	const double pallete_movement_per_frame = 0.1;

	const double x_movement_per_frame = 0;
	const double y_movement_per_frame = 0;

	const double x_amp = 0;
	const double x_period = width;
	const double x_phase_shift_per_frame = double(width)/double(num_frames);

	const double y_amp = 0;
	const double y_period = height;
	const double y_phase_shift_per_frame = double(height)/double(num_frames);
	//INPUTS

	vector<vector<double> > heat_map;

    uint iter = 0;

    for(int i=0;i<width;i++){
    	vector<double> row;
    	for(int j=0;j<height;j++){
    		int sum = 0;
    		for(int k=0;k<channels;k++){
	    		sum += rgb_image[iter++];
	    	}
	    	row.push_back(double(sum)/(256*3));
    	}
    	heat_map.push_back(row);
    }
    stbi_image_free(rgb_image);

	vector<RGBA> palette;
	switch(pallete_num){

		//Gradient
		case 0:
			for(uint i=0;i<255;i++){
				palette.push_back({i,i,i,i});
			}
		break;

		//Black and white
		case 1:
			palette.push_back({255,255,255,255});
			palette.push_back({0,0,0,255});
		break;

		//Fire
		case 2:
			palette.push_back({128,17,0,255});
			palette.push_back({182,34,3,255});
			palette.push_back({215,53,2,255});
			palette.push_back({252,100,0,255});
			palette.push_back({255,117,0,255});
			palette.push_back({250,192,0,255});
		break;

		//Melon
		case 3:
			palette.push_back({243,85,136,255});
			palette.push_back({255,187,180,255});
			palette.push_back({113,169,90,255});
			palette.push_back({0,121,68,255});
		break;

		//Matrix
		case 4:
			palette.push_back({01,07,01,255});
			palette.push_back({02,32,02,255});
			palette.push_back({10,66,11,255});
			palette.push_back({18,95,19,255});
			palette.push_back({41,172,43,255});
		break;

		//h i b a s c u s
		case 5:
			palette.push_back({255,188,66,255});
			palette.push_back({216,17,89,255});
			palette.push_back({143,45,86,255});
			palette.push_back({33,131,121,255});
			palette.push_back({115,210,222,255});
		break;

		//blue
		case 6:
			for(uint i=0;i<255;i++){
				palette.push_back({0,0,i,i});
			}
		break;

		//camo
		case 7:
			palette.push_back({167,123,78,255});
			palette.push_back({186,155,119,255});
			palette.push_back({192,186,169,255});
			palette.push_back({204,202,188,255});
		break;

		//pulse
		case 8:
			for(uint i=0;i<32;i++){
			palette.push_back({i*8,0,0,255});
			}
			for(uint i=0;i<32;i++){
			palette.push_back({(32-i)*8,0,0,255});
			}
		break;

		//highway lights
		case 9:
			for(uint i=0;i<64;i++){
			palette.push_back({0,0,0,255});
			}
			for(uint i=0;i<32;i++){
			palette.push_back({i*4,i*4,0,255});
			}
			for(uint i=0;i<32;i++){
			palette.push_back({(32-i)*4,(32-i)*4,0,255});
			}
		break;

		//blue in and out
		case 10:
			for(uint i=0;i<32;i++){
			palette.push_back({0,0,i*4,255});
			}
			for(uint i=0;i<32;i++){
			palette.push_back({0,0,(32-i)*4,255});
			}
		break;

		//rainbow
		case 11:
			palette.push_back({255,0,0,255});
			palette.push_back({255,255,0,255});
			palette.push_back({0,255,0,255});
			palette.push_back({0,255,255,255});
			palette.push_back({0,0,255,255});
			palette.push_back({255,0,255,255});
		break;
	}

	double palette_offset = 0;

	double x_offset = 0;
	double y_offset = 0;

	double x_phase_shift = 0;
	double y_phase_shift = 0;
	
	auto fileName = "output.gif";
	GifWriter g;
	GifBegin(&g, fileName, heat_map.size(),
				heat_map[0].size(), delay);
	for(uint i=0;i<num_frames;i++){

		vector<uint8_t> image;

		palette_offset += pallete_movement_per_frame;

		x_phase_shift += x_phase_shift_per_frame;
		y_phase_shift += y_phase_shift_per_frame;

		x_offset += x_movement_per_frame;
		y_offset += y_movement_per_frame;

		eb_frame(image,heat_map,palette,int(palette_offset),
			int(x_offset),int(y_offset),
			x_amp,x_period,x_phase_shift,
			y_amp,y_period,y_phase_shift);

		GifWriteFrame(&g, image.data(), heat_map.size(),
				heat_map[0].size(), delay);
	}
	GifEnd(&g);

	cout << "done" << endl;

	return 0;
}