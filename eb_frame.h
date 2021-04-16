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
		static vector<vector<RGBA>> simplify(uint8_t* image, int width,
									 int height);
		static uint8_t* desimplify(vector<vector<RGBA>> input);
		static RGBA convert_to_RGBA(string s);
		static vector<RGBA> convert_to_RGBA(vector<string> s);

		//Transforms
		void apply_palette();
		void apply_hor_osc();
		void apply_vert_osc();
		void apply_translation();

		uint8_t* image;
		const int width;
		const int height;
		const double round_progress;
		const vector<RGBA> palette;
		const int palette_offset;
		const double palette_cycles_per_round;
		const int x_offset;
		const double x_cycles_per_round;
		const int y_offset;
		const double y_cycles_per_round;
		const double x_amp_min;
		const double x_amp_max;
		const double x_amp_cycles_per_round;
		const double x_period_min;
		const double x_period_max;
		const double x_period_cycles_per_round;
		const double x_phase_offset;
		const double x_phase_cycles_per_round;
		const double y_amp_min;
		const double y_amp_max;
		const double y_amp_cycles_per_round;
		const double y_period_min;
		const double y_period_max;
		const double y_period_cycles_per_round;
		const double y_phase_offset;
		const double y_phase_cycles_per_round;

	public: 

		eb_frame(
			uint8_t* input, int width, int height,
			double round_progress = 0.0,
			vector<RGBA> palette = {}, int palette_offset=0, double palette_cycles_per_round=0,
			int x_offset=0, double x_cycles_per_round=0,
			int y_offset=0, double y_cycles_per_round=0,
			double x_amp_min=0, double x_amp_max=0, double x_amp_cycles_per_round=0,
			double x_period_min=0, double x_period_max=0, double x_period_cycles_per_round=0,
			double x_phase_offset=0, double x_phase_cycles_per_round=0,
			double y_amp_min=0, double y_amp_max=0, double y_amp_cycles_per_round=0,
			double y_period_min=0, double y_period_max=0, double y_period_cycles_per_round=0,
			double y_phase_offset=0, double y_phase_cycles_per_round=0
			);

		//eb_frame operator+(const eb_frame& lhs, const eb_frame& rhs);

		//uint8_t* get_image() const {return image;}
};




