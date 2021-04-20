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
#include <memory>

using namespace std;

typedef array<int, 4> RGBA;

class eb_frame{ 

	private:

		//Helper Functions
		// static vector<vector<RGBA>> simplify(uint8_t* image, int width,
		// 							 int height);
		// static uint8_t* desimplify(vector<vector<RGBA>> input);

		uint calc_index(int x, int y);

		static RGBA convert_to_RGBA(string s);
		static vector<RGBA> convert_to_RGBA(vector<string> s);
		
		uint8_t* image;
		const int width;
		const int height;
		const double round_progress;

	public: 

		eb_frame(uint8_t* input, int width, int height, double round_progress);

		//Transforms
		void apply_palette(vector<RGBA> palette , int palette_offset, double palette_cycles_per_round);
		void apply_hor_osc(double x_amp_min, double x_amp_max, double x_amp_cycles_per_round,
			double x_period_min, double x_period_max, double x_period_cycles_per_round,
			double x_phase_offset, double x_phase_cycles_per_round);
		void apply_vert_osc(double y_amp_min, double y_amp_max, double y_amp_cycles_per_round,
			double y_period_min, double y_period_max, double y_period_cycles_per_round,
			double y_phase_offset, double y_phase_cycles_per_round);
		void apply_translation(int x_offset, double x_cycles_per_round, int y_offset, double y_cycles_per_round);


		//eb_frame operator+(const eb_frame& lhs, const eb_frame& rhs);

		//uint8_t* get_image() const {return image;}
};




