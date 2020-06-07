#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <png.h>
#include <vector>
#include <array>

#include <cstdio>
#include <cmath>
#include <cstdlib>

using namespace std;

//png variables
typedef std::array<int, 4> RGBA;
int width, height;
png_byte color_type;
png_byte bit_depth;
png_bytep *row_pointers = NULL;

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

void read_png_file(char *filename) {
  FILE *fp = fopen(filename, "rb");

  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if(!png){
  	cout << "error on png create" << endl;
  	//abort();
  } 

  png_infop info = png_create_info_struct(png);
  if(!info){
  	cout << "error on info create" << endl;
  	//abort();
  } 

  if(setjmp(png_jmpbuf(png)))if(!info){
  	cout << "error on set png" << endl;
  	//abort();
  } 

  png_init_io(png, fp);

  png_read_info(png, info);

  width      = png_get_image_width(png, info);
  height     = png_get_image_height(png, info);
  color_type = png_get_color_type(png, info);
  bit_depth  = png_get_bit_depth(png, info);

  // Read any color_type into 8bit depth, RGBA format.
  // See http://www.libpng.org/pub/png/libpng-manual.txt

  if(bit_depth == 16)
    png_set_strip_16(png);

  if(color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png);

  // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
  if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8(png);

  if(png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);

  // These color_type don't have an alpha channel then fill it with 0xff.
  if(color_type == PNG_COLOR_TYPE_RGB ||
     color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

  if(color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png);

  png_read_update_info(png, info);

  if (row_pointers){
  	cout << "error on row_pointers" << endl;
  	//abort();
  }

  row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
  for(int y = 0; y < height; y++) {
    row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
  }

  png_read_image(png, row_pointers);

  fclose(fp);

  png_destroy_read_struct(&png, &info, NULL);
}

void write_png_file(char *filename) {
  int y;

  FILE *fp = fopen(filename, "wb");
  if(!fp) abort();

  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) abort();

  png_infop info = png_create_info_struct(png);
  if (!info) abort();

  if (setjmp(png_jmpbuf(png))) abort();

  png_init_io(png, fp);

  // Output is 8bit depth, RGBA format.
  png_set_IHDR(
    png,
    info,
    width, height,
    8,
    PNG_COLOR_TYPE_RGBA,
    PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_DEFAULT,
    PNG_FILTER_TYPE_DEFAULT
  );
  png_write_info(png, info);

  // To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
  // Use png_set_filler().
  //png_set_filler(png, 0, PNG_FILLER_AFTER);

  if (!row_pointers) abort();

  png_write_image(png, row_pointers);
  png_write_end(png, NULL);

  for(int y = 0; y < height; y++) {
    free(row_pointers[y]);
  }
  free(row_pointers);

  fclose(fp);

  png_destroy_write_struct(&png, &info);
}

void process_png_file() {

	vector<vector<RGBA > > image_info;

	//Filling the image vector
	for(int y = 0; y < height; y++){

		png_bytep row = row_pointers[y];

		vector<RGBA> image_info_row;

		for(int x = 0; x < width; x++) {

			png_bytep px = &(row[x * 4]);

			RGBA new_element;
			new_element[0] = px[0];
			new_element[1] = px[1];
			new_element[2] = px[2];
			new_element[3] = px[3];

			image_info_row.push_back(new_element);
		}

		image_info.push_back(image_info_row);
	}

	//Adding effect to image vector
	vector<vector<RGBA > > new_image_info;
	for(int y = 0; y < height; y++){
		vector<RGBA> image_info_row;
    	for(int x = 0; x < width; x++) {

    		RGBA new_element;
    		new_element[0] = 0;
    		new_element[1] = 0;
    		new_element[2] = 0;
    		new_element[3] = 0;

    		image_info_row.push_back(new_element);
    	}
    	new_image_info.push_back(image_info_row);
    }

    vector<vector<double> > perlin = perlin_noise(width,height,0.5,5);

    //del
    double largest_perlin = 0;
    double smallest_perlin = 0;
    //del

	for(int y = 0; y < height; y++){
    	for(int x = 0; x < width; x++) {



    		new_image_info[y][x][0] = (perlin[y][x]+0.5)*2*255;
    		new_image_info[y][x][1] = (perlin[y][x]+0.5)*255;
    		new_image_info[y][x][2] = 0;
    		new_image_info[y][x][3] = image_info[y][x][3];

    		if(new_image_info[y][x][0]>255){
    			new_image_info[y][x][0] = 255;
    		}
    		if(new_image_info[y][x][1]>255){
    			new_image_info[y][x][1] = 255;
    		}
    		if(new_image_info[y][x][2]>255){
    			new_image_info[y][x][2] = 255;
    		}
    		if(new_image_info[y][x][3]>255){
    			new_image_info[y][x][3] = 255;
    		}

    		//del 
    		if(perlin[y][x]<smallest_perlin){
    			smallest_perlin=perlin[y][x];
    		}
    		if(perlin[y][x]>largest_perlin){
    			largest_perlin=perlin[y][x];
    		}
    		//del
    		//cout << perlin[y][x] << " " << new_image_info[y][x][0] << endl; //del
    	}
    }

  	//Applying the image vector
	for(int y = 0; y < height; y++) {
	    png_bytep row = row_pointers[y];
	    for(int x = 0; x < width; x++) {
	      png_bytep px = &(row[x * 4]);

	      px[0] = new_image_info[y][x][0]; 
	      px[1] = new_image_info[y][x][1]; 
	      px[2] = new_image_info[y][x][2]; 
	      px[3] = new_image_info[y][x][3]; 
	    }
	  }
}

int main(int argc, char *argv[]) {

	char* input = argv[1];
	char* output = argv[2];

	read_png_file(input);
	process_png_file();
	write_png_file(output);


  for(int i=1;i<128;i++){

	read_png_file(output);
	process_png_file();
	write_png_file(c_str(i));
  }

  cout << "done" << endl;

  return 0;
}