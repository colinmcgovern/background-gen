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
		vector<vector<RGBA>> simplify(uint8_t* image);
		uint8_t* image desimplify(vector<vector<RGBA>> input);
		RGBA convert_to_RGBA(string s);

		//Transforms
		void apply_palette();
		void apply_hor_osc();
		void apply_vert_osc();
		void apply_translation();

		uint8_t* image;
		int width;
		int height;
		double round_progress;
		vector<RGBA> palette;
		int palette_offset;
		double palette_cycles_per_round;
		int x_offset;
		double x_cycles_per_round;
		int y_offset;
		double y_cycles_per_round;
		double x_amp_min;
		double x_amp_max;
		double x_amp_cycles_per_round;
		double x_period_min;
		double x_period_max;
		double x_period_cycles_per_round;
		double x_phase_offset;
		double x_phase_cycles_per_round;
		double y_amp_min;
		double y_amp_max;
		double y_amp_cycles_per_round;
		double y_period_min;
		double y_period_max;
		double y_period_cycles_per_round;
		double y_phase_offset;
		double y_phase_cycles_per_round;

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

		eb_frame(
			uint8_t* input, int width, int height,
			double round_progress = 0.0,
			vector<string> palette = {}, int palette_offset=0, double palette_cycles_per_round=0,
			int x_offset=0, double x_cycles_per_round=0,
			int y_offset=0, double y_cycles_per_round=0,
			double x_amp_min=0, double x_amp_max=0, double x_amp_cycles_per_round=0,
			double x_period_min=0, double x_period_max=0, double x_period_cycles_per_round=0,
			double x_phase_offset=0, double x_phase_cycles_per_round=0,
			double y_amp_min=0, double y_amp_max=0, double y_amp_cycles_per_round=0,
			double y_period_min=0, double y_period_max=0, double y_period_cycles_per_round=0,
			double y_phase_offset=0, double y_phase_cycles_per_round=0
			);

		eb_frame operator+(const eb_frame& lhs, const eb_frame& rhs);

		uint8_t* get_image() const {return image;}
};




