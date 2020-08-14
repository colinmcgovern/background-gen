#include <vector>
#include <cstdint>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <array>
#include <cstdio>
#include <cmath>
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
			new_row.push_back(double(i)/double(width)/2 + double(j)/double(height)/2);

			
		}
		output.push_back(new_row);	
	}

	return output;
}


std::vector<uint8_t> generate_image(int width, int height, vector<RGBA> palette, uint palette_offset){

	std::vector<uint8_t> output(width * height * 4, 0);

	//vector<vector<double> > noise = perlin_noise(width,height,0.5,5);
	vector<vector<double> > noise  = generate_grad(width,height);

	//Rotating palette
	vector<RGBA> new_palette;
	for(size_t i=0;i<palette.size();i++){
		new_palette.push_back(palette[(i+palette_offset)%palette.size()]);
	}
	palette = new_palette;

	uint index = 0;

	for(size_t i=0;i<width;i++){
		for(size_t j=0;j<height;j++){

			uint red=0;
			uint green=0;
			uint blue=0;
			uint a=0;

			double intensity = noise[i][j]+0.5;

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
	int width = 10;
	int height = 10;

	vector<RGBA> palette;
	palette.push_back({128,17,0,255});
	palette.push_back({182,34,3,255});
	palette.push_back({215,53,2,255});
	palette.push_back({252,100,0,255});
	palette.push_back({255,117,0,255});
	palette.push_back({250,192,0,255});

	vector<vector<uint8_t>> images;
	for(size_t i=0;i<palette.size();i++){
		images.push_back(generate_image(width,height,palette,i));
	}
	
	auto fileName = "bwgif.gif";
	int delay = 25;
	GifWriter g;
	GifBegin(&g, fileName, width, height, delay);
	for(size_t i=0;i<images.size();i++){
		GifWriteFrame(&g, images[i].data(), width, height, delay);
	}
	GifEnd(&g);

	cout << "done" << endl;

	return 0;
}