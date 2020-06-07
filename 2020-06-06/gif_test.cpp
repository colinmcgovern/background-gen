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

typedef std::array<int, 4> RGBA;

#define maxPrimeIndex 10
int primeIndex = 0;

int primes[maxPrimeIndex][3] = {
  { 995615039, 600173719, 701464987 },
  { 831731269, 162318869, 136250887 },
  { 174329291, 946737083, 245679977 },
  { 362489573, 795918041, 350777237 },
  { 457025711, 880830799, 909678923 },
  { 787070341, 177340217, 593320781 },
  { 405493717, 291031019, 391950901 },
  { 458904767, 676625681, 424452397 },
  { 531736441, 939683957, 810651871 },
  { 997169939, 842027887, 423882827 }
};

double Noise(int i, int x, int y) {
  int n = x + y * 57;
  n = (n << 13) ^ n;
  int a = primes[i][0], b = primes[i][1], c = primes[i][2];
  int t = (n * (n * n * a + b) + c) & 0x7fffffff;
  return 1.0 - (double)(t)/1073741824.0;
}

double SmoothedNoise(int i, int x, int y) {
  double corners = (Noise(i, x-1, y-1) + Noise(i, x+1, y-1) +
                    Noise(i, x-1, y+1) + Noise(i, x+1, y+1)) / 16,
         sides = (Noise(i, x-1, y) + Noise(i, x+1, y) + Noise(i, x, y-1) +
                  Noise(i, x, y+1)) / 8,
         center = Noise(i, x, y) / 4;
  return corners + sides + center;
}

double Interpolate(double a, double b, double x) {  // cosine interpolation
  double ft = x * 3.1415927,
         f = (1 - cos(ft)) * 0.5;
  return  a*(1-f) + b*f;
}

double InterpolatedNoise(int i, double x, double y) {
  int integer_X = x;
  double fractional_X = x - integer_X;
  int integer_Y = y;
  double fractional_Y = y - integer_Y;

  double v1 = SmoothedNoise(i, integer_X, integer_Y),
         v2 = SmoothedNoise(i, integer_X + 1, integer_Y),
         v3 = SmoothedNoise(i, integer_X, integer_Y + 1),
         v4 = SmoothedNoise(i, integer_X + 1, integer_Y + 1),
         i1 = Interpolate(v1, v2, fractional_X),
         i2 = Interpolate(v3, v4, fractional_X);
  return Interpolate(i1, i2, fractional_Y);
}

double ValueNoise_2D(double x, double y, double persistence, int numOctaves) {
  double total = 0,
         frequency = pow(2, numOctaves),
         amplitude = 1;
  for (int i = 0; i < numOctaves; ++i) {
    frequency /= 2;
    amplitude *= persistence;
    total += InterpolatedNoise((primeIndex + i) % maxPrimeIndex,
        x / frequency, y / frequency) * amplitude;
  }
  return total / frequency;
}

vector<vector<double> > perlin_noise(int width, int height, double persistence, int numOctaves){

	vector<vector<double> > output;

	for (int y = 0; y < height; ++y) {
		vector<double> row; 
    	for (int x = 0; x < width; ++x) {
    		row.push_back(ValueNoise_2D(x,y,persistence,numOctaves));
    	}
    	output.push_back(row);
  	}

  return output;
} 

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

std::vector<uint8_t> generate_image(int width, int height, vector<RGBA> palette, uint palette_offset){

	std::vector<uint8_t> output(width * height * 4, 0);

	vector<vector<double> > intensity_grid  = generate_grad(width,height);

	//Rotating palette
	vector<RGBA> new_palette;
	for(size_t i=0;i<palette.size();i++){
		new_palette.push_back(palette[(i+palette_offset)%palette.size()]);
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
	for(uint i=0;i<255;i++){
		palette.push_back({i,i,i,i});
	}

	//Black and white
	// palette.push_back({255,255,255,255});
	// palette.push_back({0,0,0,255});

	vector<vector<uint8_t>> images;
	for(size_t i=0;i<width;i++){
		images.push_back(generate_image(width,height,palette,0));
		//images[i] = sine_image(images[i],width,height,32,i,0);
		images[i] = translate_image(images[i],width,height,i,i);
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