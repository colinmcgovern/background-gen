#include "gif_test.h"

vector<vector<double> > generate_grad(int width, int height){

	vector<vector<double> > output;

	for(size_t i=0;i<width;i++){

		vector<double> new_row;

		for(size_t j=0;j<height;j++){

			double width_val = (double(i)-double(width)/2)/double(width)/2;
			if(width_val<0){
				width_val *= -1;
			}

			double height_val = (double(j)-double(height)/2)/double(height)/2;
			if(height_val<0){
				height_val *= -1;
			}

			new_row.push_back(width_val+height_val);
		}
		output.push_back(new_row);
	}

	return output;
}

//TODO finish this
vector<vector<double> > generate_checker(int width, int height, uint size){

	vector<vector<double> > output;

	double dark = 1;
	double light = 0;
	uint ld_iter = 0;

	for(size_t i=0;i<width;i++){

		vector<double> new_row;

		uint size_iter = 0;
		uint is_dark = 0;

		for(size_t j=0;j<height;j++){

			if(is_dark){
				new_row.push_back(dark);
			}else{
				new_row.push_back(light);
			}

			size_iter++;
			if(size_iter>=size){
				is_dark = !is_dark;
				size_iter = 0;
			}

		}

		output.push_back(new_row);

		ld_iter++;
		if(ld_iter>=size){
			double temp = dark;
			dark = light;
			light = temp;
			ld_iter = 0;
		}

	}

	return output;
}

void print(vector<uint8_t> input, uint width, uint height){
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

vector<vector<RGBA>> simplify(vector<uint8_t> input, uint width, uint height){

	vector<vector<RGBA>> output;

	uint index = 0;

	for(size_t i=0;i<width;i++){

		vector<RGBA> new_row;

		for(size_t j=0;j<height;j++){

			RGBA new_val;

			for(size_t k=0;k<4;k++){
				new_val[k] = input[index++];
			}
			//cout << new_val[k][0] << " " << new_val[k][1] << " " << new_val[k][2] << " " << new_val[k][3] << endl; //del

			new_row.push_back(new_val);

		}

		output.push_back(new_row);
	}

	return output;
}

vector<uint8_t> desimplify(vector<vector<RGBA>> input){
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
std::vector<uint8_t> translate_image(std::vector<uint8_t> input, uint width, uint height, uint x_offset, uint y_offset){
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

std::vector<uint8_t> sine_image(std::vector<uint8_t> input, uint width, uint height, double amp, double period, double phase_shift){
	std::vector<uint8_t> output(width * height * 4, 0);

	vector<vector<RGBA>> simple_input = simplify(input,width,height);
	vector<vector<RGBA>> simple_output = simplify(input,width,height);

	for(size_t i=0;i<width;i++){
		for(size_t j=0;j<height;j++){
			simple_output[i][j] = simple_input[uint((sin((i*period)+phase_shift)*amp))%width][(j)%height];
		}	
	}

	return desimplify(simple_output);
}

vector<RGBA> rotate_palette(vector<RGBA> palette, uint palette_offset){
	vector<RGBA> output;
	for(size_t i=0;i<palette.size();i++){
		output.push_back(palette[(i+palette_offset)%palette.size()]);
	}
	return output;
}

vector<uint8_t> generate_image(int width, int height, vector<RGBA> palette, PATTERN pattern){

	std::vector<uint8_t> output(width * height * 4, 0);

	vector<vector<double> > intensity_grid;
	if(pattern==GRAD){
		intensity_grid = generate_grad(width,height);
	}else if(pattern==CHECKER){
		intensity_grid = generate_checker(width,height,16);
	}


	uint index = 0;

	for(size_t i=0;i<width;i++){
		for(size_t j=0;j<height;j++){

			uint red=0;
			uint green=0;
			uint blue=0;
			uint a=0;

			double intensity = intensity_grid[i][j]+0.5;

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

int main()
{
	int width = 128;
	int height = 128;

	vector<RGBA> palette;

	//Fire
	// palette.push_back({128,17,0,255});
	// palette.push_back({182,34,3,255});
	// palette.push_back({215,53,2,255});
	// palette.push_back({252,100,0,255});
	// palette.push_back({255,117,0,255});
	// palette.push_back({250,192,0,255});

	//Gradient
	// for(uint i=0;i<255;i++){
	// 	palette.push_back({i,i,i,i});
	// }

	//Black and white
	palette.push_back({255,255,255,255});
	palette.push_back({0,0,0,255});

	vector<vector<uint8_t>> images;
	for(size_t i=0;i<width;i++){

		// palette = rotate_palette(palette,1);

		images.push_back(generate_image(width,height,palette,CHECKER));
		// images[i] = sine_image(images[i],width,height,32,i,0);
		//images[i] = translate_image(images[i],width,height,i,i);
	}
	
	auto fileName = "bwgif.gif";
	int delay = 2;
	GifWriter g;
	GifBegin(&g, fileName, width, height, delay);
	for(size_t i=0;i<images.size();i++){
		// print(images[i],width,height);
		// cout << endl;
		GifWriteFrame(&g, images[i].data(), width, height, delay);
	}
	GifEnd(&g);

	cout << "done" << endl;

	return 0;
}