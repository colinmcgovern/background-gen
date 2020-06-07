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

#include "gif.h"

using namespace std;

typedef array<int, 4> RGBA;

typedef vector<vector<double> >  HEAT_MAP;

enum PATTERN {
  GRAD,
  CHECKER
};

class frame{ 

	private:
		//Heat Map Generators
		HEAT_MAP generate_grad(int width, int height);
		HEAT_MAP generate_checker(int width, int height, uint size);

		//Transformers
		vector<uint8_t> translate_image(std::vector<uint8_t> input, uint width, uint height, uint x_offset, uint y_offset);
		vector<uint8_t> sine_image(std::vector<uint8_t> input, uint width, uint height, double amp, double period, double phase_shift);
		vector<RGBA> rotate_palette(vector<RGBA> palette, uint palette_offset);
		void print(vector<uint8_t> input, uint width, uint height);

		//Helper Functions
		vector<vector<RGBA>> simplify(vector<uint8_t> input, uint width, uint height);
		vector<uint8_t> desimplify(vector<vector<RGBA>> input);

		vector<uint8_t> generate_image(int width, int height, vector<RGBA> palette, HEAT_MAP heatmap);

	public: 
		frame(int width, int height,
			vector<RGBA> palette, uint palette_offset,
			uint x_offset, uint y_offset,
			double amp, double period, double phase_shift,
			PATTERN pattern, uint checker_size)

}


