#include "frame.h"
#include "utility.h"

vector<vector<RGBA>> frame::simplify(vector<uint8_t> input, uint width,
									 uint height){

	vector<vector<RGBA>> output;

	uint index = 0;

	for(size_t i=0;i<width;i++){

		vector<RGBA> new_row;

		for(size_t j=0;j<height;j++){

			RGBA new_val;

			for(size_t k=0;k<4;k++){
				new_val[k] = input[index++];
			}

			new_row.push_back(new_val);

		}

		output.push_back(new_row);
	}

	return output;
}

vector<uint8_t> frame::desimplify(vector<vector<RGBA>> input){
	vector<uint8_t> output;
	for(size_t i=0;i<input.size();i++){
		for(size_t j=0;j<input[i].size();j++){
			for(size_t k=0;k<input[i][j].size();k++){
				output.push_back(input[i][j][k]);
			}
		}
	}
	return output;
}

//TODO needs tesing
std::vector<uint8_t> frame::translate_image(std::vector<uint8_t> input, uint width, uint height, 
											int x_offset, int y_offset){
	std::vector<uint8_t> output(width * height * 4, 0);

	vector<vector<RGBA>> simple_input = simplify(input,width,height);
	vector<vector<RGBA>> simple_output = simplify(input,width,height);

	for(size_t i=0;i<width;i++){
		for(size_t j=0;j<height;j++){
			simple_output[i][j] = simple_input[(i+x_offset)%width][(j+y_offset)%height];
		}	
	}

	return desimplify(simple_output);
}

std::vector<uint8_t> frame::hor_osc(std::vector<uint8_t> input, uint width, uint height, double amp, double period, double phase_shift){
	std::vector<uint8_t> output(width * height * 4, 0);

	vector<vector<RGBA>> simple_input = simplify(input,width,height);
	vector<vector<RGBA>> simple_output = simplify(input,width,height);

	for(size_t i=0;i<width;i++){

		int j_shift = int( -cos((i*6.28/period)+phase_shift)*amp );

		for(size_t j=0;j<height;j++){
			uint new_j = (j + j_shift) % height;
			simple_output[i][j] = simple_input[i][new_j];
		}	
	}

	return desimplify(simple_output);
}

std::vector<uint8_t> frame::vert_osc(std::vector<uint8_t> input, uint width, uint height, double amp, double period, double phase_shift){

	std::vector<uint8_t> output(width * height * 4, 0);

	//Rotate 90 degrees
	vector<vector<RGBA>> simple_input = simplify(input,width,height);
	vector<vector<RGBA>> simple_output = simplify(input,width,height);
	for(size_t i=0;i<width;i++){
		for(size_t j=0;j<height;j++){
			simple_output[i][j] = simple_input[j][i];
		}
	}

	std::vector<uint8_t> to_hor_osc = desimplify(simple_output);
	std::vector<uint8_t> hor_osc_output = hor_osc(to_hor_osc,width,height, amp, period, phase_shift);

	//Rotate 90 degrees
	vector<vector<RGBA>> simple_input2 = simplify(hor_osc_output,width,height);
	vector<vector<RGBA>> simple_output2 = simplify(hor_osc_output,width,height);
	for(size_t i=0;i<width;i++){
		for(size_t j=0;j<height;j++){
			simple_output2[i][j] = simple_input2[j][i];
		}
	}

	return desimplify(simple_output2);
}

vector<RGBA> frame::rotate_palette(vector<RGBA> palette, int palette_offset){
	vector<RGBA> output;
	for(size_t i=0;i<palette.size();i++){
		output.push_back(palette[(i+palette_offset)%palette.size()]);
	}
	return output;
}

vector<uint8_t> frame::generate_image(int width, int height,
				 vector<RGBA> palette, vector<vector<double>> heat_map){

	std::vector<uint8_t> output(width * height * 4, 0);

	uint index = 0;

	for(size_t i=0;i<width;i++){
		for(size_t j=0;j<height;j++){

			uint red=0;
			uint green=0;
			uint blue=0;
			uint a=0;

			double intensity = heat_map[i][j];

			for(size_t k=0;k<palette.size();k++){
				if(intensity<double(k+1)/double(palette.size())){
					red = palette[k][0];
					green = palette[k][1];
					blue = palette[k][2];
					a = palette[k][3];
					break;
				}
			}

			for(size_t k=0;k<4;k++){

				switch(k){
					case 0:
						output[index] = red;
						break;
					case 1:
						output[index] = green;
						break;
					case 2:
						output[index] = blue;
						break;
					default:
						output[index] = a;
						break;
				}

				index++;

			}
		}
	}

	return output;
}

frame::frame(vector<uint8_t> &image, vector<vector<double> > heat_map,
			vector<RGBA> palette, int palette_offset,
			int x_offset, int y_offset,
			double x_amp, double x_period, double x_phase_shift,
			double y_amp, double y_period, double y_phase_shift
			){	

	palette = rotate_palette(palette,palette_offset);

	image = generate_image(heat_map.size(),heat_map[0].size(),
							palette,heat_map);

	image = translate_image(image,heat_map.size(),
				heat_map[0].size(),y_offset,x_offset);	

	//del uncomment
	image = hor_osc(image,heat_map.size(),
				heat_map[0].size(),x_amp,x_period,x_phase_shift);
	image = vert_osc(image,heat_map.size(),
				heat_map[0].size(),y_amp,y_period,y_phase_shift);
}
