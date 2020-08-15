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

#include "gif.h"

using namespace std;

typedef array<int, 4> RGBA;

class frame{ 

	private:

		//Transformers
		vector<uint8_t> translate_image(std::vector<uint8_t> input, uint width, uint height, int x_offset, int y_offset);
		
		vector<uint8_t> hor_osc(std::vector<uint8_t> input, uint width, uint height, double amp, double period, double phase_shift);
		vector<uint8_t> vert_osc(std::vector<uint8_t> input, uint width, uint height, double amp, double period, double phase_shift);
		
		vector<RGBA> rotate_palette(vector<RGBA> palette, int palette_offset);

		//Helper Functions
		vector<vector<RGBA>> simplify(vector<uint8_t> input, uint width, uint height);
		vector<uint8_t> desimplify(vector<vector<RGBA>> input);

		vector<uint8_t> generate_image(int width, int height, vector<RGBA> palette, vector<vector<double>> heat_map);

	public: 
		frame(vector<uint8_t> &image, vector<double> heat_map,
			vector<RGBA> palette, int palette_offset,
			int x_offset, int y_offset,
			double x_amp, double x_period, double x_phase_shift,
			double y_amp, double y_period, double y_phase_shift
			);

	//Utitity
	void print(vector<uint8_t> input, uint width, uint height);
	void print(vector<vector<double> > v);

};




