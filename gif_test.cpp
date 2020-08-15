#include "gif_test.h"

void frame::print(vector<vector<double> > v){
	for(auto &x: v){
		for(auto &y: x){
			cout << y << " ";
		}
		cout << endl;
	}
}

void frame::print(vector<uint8_t> input, uint width, uint height){
	uint index = 0;
	for(size_t i=0;i<width;i++){
		for(size_t j=0;j<height;j++){
			for(size_t k=0;k<4;k++){
				cout << int(input[index++]);
				cout << ",";
			}
			cout << " ";
		}
		cout << endl;
	}
}

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

		int i_shift = int( -cos((i*period)+phase_shift)*amp );

		cout << i_shift << " "; //del

		for(size_t j=0;j<height;j++){
			uint new_i = (i + i_shift) % width;
			simple_output[i][j] = simple_input[new_i][j];
		}	
	}

	cout << endl; //del

	return desimplify(simple_output);
}

std::vector<uint8_t> frame::vert_osc(std::vector<uint8_t> input, uint width, uint height, double amp, double period, double phase_shift){
	std::vector<uint8_t> output(width * height * 4, 0);

	vector<vector<RGBA>> simple_input = simplify(input,width,height);
	vector<vector<RGBA>> simple_output = simplify(input,width,height);

	for(size_t i=0;i<width;i++){

		int i_shift = int( sin((i*period)+phase_shift)*amp );

		cout << i_shift << " "; //del

		for(size_t j=0;j<height;j++){
			uint new_i = (i + i_shift) % width;
			simple_output[i][j] = simple_input[new_i][j];
		}	
	}

	cout << endl; //del

	return desimplify(simple_output);
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

frame::frame(vector<uint8_t> &image, vector<double> heat_map,
			vector<RGBA> palette, int palette_offset,
			int x_offset, int y_offset,
			double x_amp, double x_period, double x_phase_shift,
			double y_amp, double y_period, double y_phase_shift
			){	

	palette = rotate_palette(palette,palette_offset);

	image = generate_image(heat_map.size(),heat_map[0].size(),
							palette,heat_map);

	image = translate_image(image,width,height,x_offset,y_offset);	

	image = vert_osc(image,heat_map.size(),
				heat_map[0].size(),x_amp,x_period,x_phase_shift);
	image = vert_osc(image,heat_map.size(),
				heat_map[0].size(),y_amp,y_period,y_phase_shift);
}

int main(int argc, char** argv){ 
  
	const string heat_map_file = string(argv[2]);

	const uint pallete_num = uint(argv[3]);
	const uint pallete_movement_per_frame = uint(argv[4]);

	const double x_movement_per_frame = double(argv[5]);
	const double y_movement_per_frame = double(argv[6]);

	const double x_amp = double(argv[7]);
	const double x_period = double(argv[8]);
	const double x_phase_shift_per_frame = double(argv[9]);

	const double y_amp = double(argv[10]);
	const double y_period = double(argv[11]);
	const double y_phase_shift_per_frame = double(argv[12]);

	const uint num_frames = uint(argv[13]);
	const uint delay = double(argv[14]);

	//loading heatmap from csv
	//TODO finish this
	vector<vector<double> > heat_map;
	// while(getline(heat_map_file, ID, ',')) {
	//     cout << ID << endl;
	// }
	
	vector<RGBA> palette;
	switch(pallete_num){

		//Fire
		case 0:
			palette.push_back({128,17,0,255});
			palette.push_back({182,34,3,255});
			palette.push_back({215,53,2,255});
			palette.push_back({252,100,0,255});
			palette.push_back({255,117,0,255});
			palette.push_back({250,192,0,255});
		break;

		//Melon
		case 1:
			palette.push_back({243,85,136,255});
			palette.push_back({255,187,180,255});
			palette.push_back({113,169,90,255});
			palette.push_back({0,121,68,255});
		break;

		//Gradient
		case 2:
			for(uint i=0;i<255;i++){
				palette.push_back({i,i,i,i});
			}
		break;

		//Black and white
		default:
			palette.push_back({255,255,255,255});
			palette.push_back({0,0,0,255});
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

		frame(image,heat_map,palette,int(palette_offset),
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