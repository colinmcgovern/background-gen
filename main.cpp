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
	uint8_t* image = stbi_load(input_file, &width, &height, &channels, 3);

	vector<RGBA> palette;

	for(int i=0;i<255;i++){
		palette.push_back({i,i,i,0});
	}
	
	int palette_offset = 0;
	double palette_cycles_per_round = 1;
	int x_offset = 0;
	double x_cycles_per_round = 0;
	int y_offset = 0;
	double y_cycles_per_round = 0;
	double x_amp_min = 0;
	double x_amp_max = 0;
	double x_amp_cycles_per_round = 0;
	double x_period_min = 0;
	double x_period_max = 0;
	double x_period_cycles_per_round = 0;
	double x_phase_offset = 0;
	double x_phase_cycles_per_round = 0;
	double y_amp_min = 0;
	double y_amp_max = 0;
	double y_amp_cycles_per_round = 0;
	double y_period_min = 0;
	double y_period_max = 0;
	double y_period_cycles_per_round = 0;
	double y_phase_offset = 0;
	double y_phase_cycles_per_round = 0;
    
	const uint NUM_FRAMES = 100;
	const uint DELAY = 3;

	GifWriter g;
	GifBegin(&g, output_file, width, height, DELAY);

	for(uint i=0;i<NUM_FRAMES;i++){


		eb_frame(	image,
		 			width, 
		 			height, 
		 			double(i) / double(NUM_FRAMES),
		 			palette,
					palette_offset,
					palette_cycles_per_round,
					x_offset,
					x_cycles_per_round,
					y_offset,
					y_cycles_per_round,
					x_amp_min,
					x_amp_max,
					x_amp_cycles_per_round,
					x_period_min,
					x_period_max,
					x_period_cycles_per_round,
					x_phase_offset,
					x_phase_cycles_per_round,
					y_amp_min,
					y_amp_max,
					y_amp_cycles_per_round,
					y_period_min,
					y_period_max,
					y_period_cycles_per_round,
					y_phase_offset,
					y_phase_cycles_per_round
		 		);

		GifWriteFrame(&g, image, width, height, DELAY);
	}
	GifEnd(&g);

	stbi_image_free(image);

	cout << "done" << endl;

	return 0;
}